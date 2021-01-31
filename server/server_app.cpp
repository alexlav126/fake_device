#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "logger.h"
#include "server.h"
#include "meas_channel.h"
#include "../proto/proto.h"
#include <iostream>

Server *server;
const int NUM_CHANNELS = 1;
MeasChannel *meas_channel[NUM_CHANNELS];

inline static bool IsChannelValid(int channel)
{
    return (channel >= 0 && channel < NUM_CHANNELS);
}

static bool StartMeasureCallback(int channel)
{
    if (!IsChannelValid(channel))
    {
        return false;
    }
    return meas_channel[channel]->StartMeasure();
}

static bool StopMeasureCallback(int channel)
{
    if (!IsChannelValid(channel))
    {
        return false;
    }
    return meas_channel[channel]->StopMeasure();
}

static bool SetRangeCallback(int channel, int input_range, int *output_range)
{
    if (!IsChannelValid(channel))
    {
        return false;
    }
    return meas_channel[channel]->SetRange(input_range, output_range);
}

static bool GetStatusCallback(int channel,
                              proto::CommandGetStatus::Status *output_status)
{
    if (!IsChannelValid(channel))
    {
        return false;
    }
    auto status = meas_channel[channel]->GetStatus();
    switch (status)
    {
    case MeasChannel::Status::Idle:
        *output_status = proto::CommandGetStatus::Status::Idle;
        break;
    case MeasChannel::Status::Error:
        *output_status = proto::CommandGetStatus::Status::Error;
        break;
    case MeasChannel::Status::Busy:
        *output_status = proto::CommandGetStatus::Status::Busy;
        break;
    case MeasChannel::Status::Measure:
        *output_status = proto::CommandGetStatus::Status::Measure;
        break;
    }
    return true;
}

static bool GetResultCallback(int channel, float *output_result)
{
    if (!IsChannelValid(channel))
    {
        return false;
    }
    return meas_channel[channel]->GetResult(output_result);
}

int main(int argc, char *argv[])
{
    Logger::SetLevel(Logger::Level::DBG);
    Logger::Log(Logger::Level::DBG, "Starting server app");

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        meas_channel[i] = new MeasChannel(i);
        if (meas_channel[i] == nullptr)
        {
            Logger::Log(Logger::Level::ERR, "Can't allocate meas_channel");
            exit(EXIT_FAILURE);
        }
        meas_channel[i]->StartMeasure();
    }

    proto::Proto::Callback cb;
    cb.start_measure = StartMeasureCallback;
    cb.stop_measure = StopMeasureCallback;
    cb.set_range = SetRangeCallback;
    cb.get_status = GetStatusCallback;
    cb.get_result = GetResultCallback;

    server = new Server(cb);
    if (server == nullptr)
    {
        Logger::Log(Logger::Level::ERR, "Can't allocate server");
        exit(EXIT_FAILURE);
    }

    if (!server->Start())
    {
        Logger::Log(Logger::Level::ERR, "Can't start server");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        // sleep(1);
        printf("press q, for exit\n");
        int c = std::cin.get();
        if (c == 'q')
        {
            break;
        }
    }
    delete server;
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        delete meas_channel[i];
    }
}