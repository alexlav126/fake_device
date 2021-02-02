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

    std::string CommandGetStatus::GetRequest()
    {
        return Command::GetRequest().append("\n");
    }

    Command::Result CommandGetStatus::CheckResponse(const std::string &response,
                                                    Status *status)
    {
        Result ret = Command::CheckResponseResult(response);
        if (ret == Result::Ok)
        {
            std::string delim = ", ";
            size_t pos = response.find(delim);
            if (pos == std::string::npos)
            {
                return Result::Fail;
            }
            pos += delim.length();
            for (int i = 0; i < NUM_STATUS; i++)
            {
                size_t p = response.find(STATUS_NAME[i].name, pos);
                if (p != std::string::npos)
                {
                    *status = STATUS_NAME[i].status;
                    return Result::Ok;
                }
            }
            return Result::Fail;
        }
        return ret;
    }
} // namespace proto