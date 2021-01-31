#include "command_set_range.h"

namespace proto
{
    CommandSetRange::CommandSetRange(int channel,
                                     int range,
                                     CommandSetRange::Callback callback)
        : Command(CommandId::SetRange, channel)
    {
        this->range = range;
        this->callback = callback;
    }

    void CommandSetRange::ProcessCallback()
    {
        if (callback == nullptr)
        {
            response_result = Command::Result::Fail;
            return;
        }
        int output_range;
        bool result = callback(channel, range, &output_range);
        if (result)
        {
            result = range == output_range;
        }
        response_result = result ? Command::Result::Ok : Command::Result::Fail;
    }

    std::string CommandSetRange::GetResponse()
    {
        std::string response = Command::GetResponse();
        if (response_result == Result::Ok)
        {
            response.append(", ");
            response.append(PREFIX_RANGE);
            response.append(std::to_string(range));
        }
        response.append("\n");
        return response;
    }
} // namespace proto