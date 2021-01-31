#include "meas_channel.h"
#include "logger.h"
#include <unistd.h>

MeasChannel::MeasChannel(int id)
{
    this->id = id;
    status = Status::Idle;
    range = NUM_RANGES - 1;
    result = 0.0f;
    fake_signal = 1.0f;
    noise_percent = 10;
    thread_handler_enable = true;
    thread_handler = new std::thread(&MeasChannel::Handler, this);
    // if(thread_handler == nullptr) throw ...
}

MeasChannel::~MeasChannel()
{
    thread_handler_enable = false;
    thread_handler->join();
    delete thread_handler;
}

MeasChannel::Status MeasChannel::GetStatus() const
{
    return status;
}

bool MeasChannel::StartMeasure()
{
    Logger::Log(Logger::Level::INF,
                "Device: start measure chan = %d",
                id);
    if (status == MeasChannel::Status::Idle)
    {
        status = MeasChannel::Status::Measure;
        return true;
    }
    return false;
}

bool MeasChannel::StopMeasure()
{
    Logger::Log(Logger::Level::INF,
                "Device: stop measure chan = %d",
                id);

    status = MeasChannel::Status::Idle;
    return true;
}

bool MeasChannel::SetRange(int input_range, int *output_range)
{
    Logger::Log(Logger::Level::INF,
                "Device: SetRange chan = %d range = %d",
                id,
                input_range);
    if (input_range < 0 || input_range >= NUM_RANGES)
    {
        *output_range = range;
        return false;
    }
    range = input_range;
    *output_range = range;
    return true;
}

bool MeasChannel::GetResult(float *result) const
{
    *result = this->result;
    return true;
}

void MeasChannel::Handler()
{
    while (thread_handler_enable)
    {
        switch (status)
        {
        case MeasChannel::Status::Error:
            status = MeasChannel::Status::Idle;
            break;
        case MeasChannel::Status::Idle:
            if (std::rand() % 100 < 20)
            {
                status = MeasChannel::Status::Error;
            }
            break;
        case MeasChannel::Status::Measure:
            status = MeasChannel::Status::Busy;
            break;
        case MeasChannel::Status::Busy:
            status = MeasChannel::Status::Idle;
            result = Measure();
            break;
        default:
            break;
        }
        Logger::Log(Logger::Level::INF,
                    "Handler measure chan[%d] status=%s, result=%f",
                    id,
                    StatusToStr(status),
                    result);
        sleep(5);
    }
}

float MeasChannel::Measure()
{
    const float RANGE_MAX[NUM_RANGES] = {
        0.001,
        1.0,
        1000,
        1000000};
    int percent = (std::rand() % noise_percent - noise_percent / 2);
    float noise = fake_signal * percent / 100.f;
    float value = fake_signal + noise;
    if (value > RANGE_MAX[range])
    {
        value = RANGE_MAX[range];
    }
    return value;
}

const char *MeasChannel::StatusToStr(Status status)
{
    switch (status)
    {
    case MeasChannel::Status::Error:
        return "Error";
    case MeasChannel::Status::Idle:
        return "Idle";
    case MeasChannel::Status::Measure:
        return "Measure";
    case MeasChannel::Status::Busy:
        return "Busy";
    default:
        return "?";
    }
}