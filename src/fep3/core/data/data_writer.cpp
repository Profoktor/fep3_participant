/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <fep3/base/data_registry/data_registry.h>
#include <fep3/base/stream_type/default_stream_type.h>
#include <fep3/core/data/data_writer.h>
#include <fep3/native_components/clock/variant_handling/clock_service_handling.h>

namespace fep3 {
namespace core {
namespace arya {

DataWriter::DataWriter()
    : _stream_type(fep3::base::arya::meta_type_raw), _queue_size(DATA_WRITER_QUEUE_SIZE_DYNAMIC)
{
}

DataWriter::DataWriter(std::string name, const base::StreamType& stream_type)
    : _name(std::move(name)), _stream_type(stream_type), _queue_size(DATA_WRITER_QUEUE_SIZE_DYNAMIC)
{
}

DataWriter::DataWriter(std::string name, const base::StreamType& stream_type, size_t queue_size)
    : _name(std::move(name)), _stream_type(stream_type), _queue_size(queue_size)
{
}

fep3::Result DataWriter::addToDataRegistry(IDataRegistry& data_registry)
{
    if (DATA_WRITER_QUEUE_SIZE_DYNAMIC == _queue_size) {
        return base::addDataOut(data_registry, _connected_writer, _name.c_str(), _stream_type);
    }
    else {
        return base::addDataOut(
            data_registry, _connected_writer, _name.c_str(), _stream_type, _queue_size);
    }
}

fep3::Result DataWriter::addClockTimeGetter(
    std::function<fep3::Timestamp()> clock_service_time_getter)
{
    _clock_service_time_getter = clock_service_time_getter;
    return {};
}

fep3::Result DataWriter::removeFromDataRegistry()
{
    _connected_writer.reset();
    return {};
}

fep3::Result DataWriter::removeFromDataRegistry(IDataRegistry& data_registry)
{
    _connected_writer.reset();
    return base::removeDataOut(data_registry, _name);
}

fep3::Result DataWriter::removeClockTimeGetter()
{
    _clock_service_time_getter = {};
    return {};
}

struct SampleWithAddedTimeAndCounter : public IDataSample {
    const IDataSample& _data_sample;
    const uint32_t _counter;
    fep3::Optional<Timestamp> _time;
    explicit SampleWithAddedTimeAndCounter(std::function<fep3::Timestamp()> time_getter,
                                           const IDataSample& data_sample,
                                           const uint32_t counter)
        : _data_sample(data_sample), _counter(counter)
    {
        if (time_getter) {
            _time = time_getter();
        }
    }

    Timestamp getTime() const override
    {
        if (_time) {
            if (_data_sample.getTime().count() == 0) {
                return *_time;
            }
        }
        return _data_sample.getTime();
    }

    size_t getSize() const override
    {
        return _data_sample.getSize();
    }

    uint32_t getCounter() const override
    {
        return _counter;
    }

    size_t read(IRawMemory& writeable_memory) const override
    {
        return _data_sample.read(writeable_memory);
    }

    void setTime(const Timestamp&) override
    {
        // invalid call
    }

    void setCounter(uint32_t) override
    {
        // invalid call
    }

    size_t write(const IRawMemory&) override
    {
        // invalid call
        return 0;
    }
};

fep3::Result DataWriter::write(const IDataSample& data_sample)
{
    if (_connected_writer) {
        SampleWithAddedTimeAndCounter sample_wrap(
            _clock_service_time_getter, data_sample, _counter++);
        return _connected_writer->write(sample_wrap);
    }
    else {
        RETURN_ERROR_DESCRIPTION(ERR_NOT_CONNECTED, "not connected");
    }
}

fep3::Result DataWriter::write(const IStreamType& stream_type)
{
    if (_connected_writer) {
        _stream_type = stream_type;
        return _connected_writer->write(_stream_type);
    }
    else {
        RETURN_ERROR_DESCRIPTION(ERR_NOT_CONNECTED, "not connected");
    }
}

fep3::Result DataWriter::write(Timestamp time, const void* data, size_t data_size)
{
    base::DataSampleRawMemoryRef ref_sample(time, data, data_size);
    return write(ref_sample);
}

fep3::Result DataWriter::flushNow(Timestamp)
{
    return _connected_writer->flush();
}

std::string DataWriter::getName() const
{
    return _name;
}

fep3::Result addToComponents(DataWriter& writer, const IComponents& components)
{
    auto data_registry = components.getComponent<IDataRegistry>();
    if (data_registry) {
        auto res = writer.addToDataRegistry(*data_registry);
        if (res) {
            auto [result, clock] = fep3::native::getClockServiceAdapter(components, nullptr);
            return result ? writer.addClockTimeGetter(clock->getTimeGetter()) : fep3::Result{};
        }
        else {
            return res;
        }
    }
    else {
        RETURN_ERROR_DESCRIPTION(ERR_NOT_FOUND,
                                 "%s is not part of the given component registry",
                                 getComponentIID<IDataRegistry>().c_str());
    }
}

fep3::Result removeFromComponents(DataWriter& writer, const IComponents& components)
{
    auto data_registry = components.getComponent<IDataRegistry>();
    if (data_registry) {
        return writer.removeFromDataRegistry(*data_registry);
    }
    else {
        RETURN_ERROR_DESCRIPTION(ERR_NOT_FOUND,
                                 "%s is not part of the given component registry",
                                 getComponentIID<IDataRegistry>().c_str());
    }
}

} // namespace arya
} // namespace core
} // namespace fep3
