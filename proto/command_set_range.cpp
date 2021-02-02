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

    std::string CommandSetRange::GetRequest()
    {
        std::string request = Command::GetRequest();
        request.append(", ");
        request.append(PREFIX_RANGE);
        request.append(std::to_string(range));
        request.append("\n");
        return request;
    }

    Command::Result CommandSetRange::CheckResponse(const std::string &response,
                                                   int *range)
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

            std::string prefix = PREFIX_RANGE;
            pos = response.find(prefix, pos);
            if (pos == std::string::npos)
            {
                return Result::Fail;
            }
            *range = std::stoi(response.substr(pos + prefix.size()));
        }
        return ret;
    }
} // namespace proto