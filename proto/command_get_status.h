#ifndef __COMMAND_GET_STATUS_H_
#define __COMMAND_GET_STATUS_H_

#include <string>
#include "command.h"

namespace proto
{
    class CommandGetStatus : public Command
    {
    public:
        enum class Status
        {
            Error,
            Idle,
            Measure,
            Busy
        };
        using Callback = bool (*)(int channel,
                                  Status *output_status);
        CommandGetStatus(int channel, Callback callback);
        void ProcessCallback() override;
        std::string GetResponse() override;
        std::string GetRequest() override;
        Result CheckResponse(const std::string &response, Status *status);

    private:
        struct StatusName
        {
            Status status;
            const char *name;
        };
        static constexpr int NUM_STATUS = 4;
        static constexpr StatusName STATUS_NAME[NUM_STATUS] = {
            {Status::Error, "error_state"},
            {Status::Idle, "idle_state"},
            {Status::Measure, "measure_state"},
            {Status::Busy, "busy_state"},
        };
        static std::string StatusToStr(Status status);

        Callback callback;
        Status status;
    };
} // namespace proto
#endif // __COMMAND_GET_STATUS_H_