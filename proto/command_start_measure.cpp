#include "command_start_measure.h"

namespace proto
{
    CommandStartMeasure::CommandStartMeasure(int channel,
                                             CommandStartMeasure::Callback callback)
        : Command(CommandId::StartMeasure, channel)
    {
        this->callback = callback;
    }

    void CommandStartMeasure::ProcessCallback()
    {
        if (callback == nullptr)
        {
            response_result = Command::Result::Fail;
            return;
        }
        bool result = callback(channel);
        response_result = result ? Command::Result::Ok : Command::Result::Fail;
    }

    std::string CommandStartMeasure::GetResponse()
    {
        std::string response = Command::GetResponse();
        response.append("\n");
        return response;
    }

    std::string CommandStartMeasure::GetRequest()
    {
        return Command::GetRequest().append("\n");
    }

    Command::Result CommandStartMeasure::CheckResponse(const std::string &response)
    {
        return Command::CheckResponseResult(response);
    }
} // namespace proto