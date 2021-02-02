#ifndef __COMMAND_H_
#define __COMMAND_H_

#include <string>
#include "command_id.h"

namespace proto
{
    class Command
    {
    public:
        enum class Result
        {
            Ok,
            Fail
        };

        Command(CommandId id, int channel);
        virtual ~Command() = default;
        CommandId GetId() const;
        virtual void ProcessCallback() = 0;
        virtual std::string GetResponse();
        virtual std::string GetRequest();
        Result CheckResponseResult(const std::string &response);
        static std::string IdToStr(CommandId id);
        static CommandId StrToId(std::string id);

        static constexpr char PREFIX_CHANNEL[] = "channel";

    protected:
        CommandId id;
        int channel;
        Result response_result;

        struct IdName
        {
            CommandId id;
            const char *name;
        };
        static constexpr int NUM_COMMANDS = 6;
        static constexpr IdName ID_NAME[NUM_COMMANDS] = {
            {CommandId::Unknown, "unknown"},
            {CommandId::StartMeasure, "start_measure"},
            {CommandId::StopMeasure, "stop_measure"},
            {CommandId::SetRange, "set_range"},
            {CommandId::GetStatus, "get_status"},
            {CommandId::GetResult, "get_result"},
        };
    };
} // namespace proto
#endif // __COMMAND_H_