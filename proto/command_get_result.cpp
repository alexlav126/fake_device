#include "command_get_result.h"

namespace proto
{
    CommandGetResult::CommandGetResult(int channel,
                                       CommandGetResult::Callback callback)
        : Command(CommandId::GetResult, channel)
    {
        this->callback = callback;
    }

    void CommandGetResult::ProcessCallback()
    {
        if (callback == nullptr)
        {
            response_result = Command::Result::Fail;
            return;
        }
        bool result = callback(channel, &this->result);
        response_result = result ? Command::Result::Ok : Command::Result::Fail;
    }

    std::string CommandGetResult::GetResponse()
    {
        std::string response = Command::GetResponse();
        if (response_result == Result::Ok)
        {
            response.append(", ");
            response.append(std::to_string(result));
        }
        response.append("\n");
        return response;
    }

    std::string CommandGetResult::GetRequest()
    {
        return Command::GetRequest().append("\n");
    }

    Command::Result CommandGetResult::CheckResponse(const std::string &response,
                                                    float *result)
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
            *result = strtof(response.c_str() + pos, nullptr);
        }
        return ret;
    }
} // namespace proto