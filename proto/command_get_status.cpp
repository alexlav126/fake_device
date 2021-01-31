#include "command_get_status.h"

namespace proto
{
    CommandGetStatus::CommandGetStatus(int channel,
                                       CommandGetStatus::Callback callback)
        : Command(CommandId::GetStatus, channel)
    {
        this->callback = callback;
    }

    void CommandGetStatus::ProcessCallback()
    {
        if (callback == nullptr)
        {
            response_result = Command::Result::Fail;
            return;
        }
        bool result = callback(channel, &status);
        response_result = result ? Command::Result::Ok : Command::Result::Fail;
    }

    std::string CommandGetStatus::GetResponse()
    {
        std::string response = Command::GetResponse();
        if (response_result == Result::Ok)
        {
            response.append(", ");
            response.append(StatusToStr(status));
        }
        response.append("\n");
        return response;
    }

    std::string CommandGetStatus::StatusToStr(Status status)
    {
        for (auto it : STATUS_NAME)
        {
            if (it.status == status)
            {
                return it.name;
            }
        }
        return "?";
    }
} // namespace proto