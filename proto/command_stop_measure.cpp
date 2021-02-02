#include "command_stop_measure.h"

namespace proto
{
    CommandStopMeasure::CommandStopMeasure(int channel,
                                           CommandStopMeasure::Callback callback)
        : Command(CommandId::StopMeasure, channel)
    {
        this->callback = callback;
    }

    void CommandStopMeasure::ProcessCallback()
    {
        if (callback == nullptr)
        {
            response_result = Command::Result::Fail;
            return;
        }
        bool result = callback(channel);
        response_result = result ? Command::Result::Ok : Command::Result::Fail;
    }

    std::string CommandStopMeasure::GetResponse()
    {
        std::string response = Command::GetResponse();
        response.append("\n");
        return response;
    }

    std::string CommandStopMeasure::GetRequest()
    {
        return Command::GetRequest().append("\n");
    }

    Command::Result CommandStopMeasure::CheckResponse(const std::string &response)
    {
        return Command::CheckResponseResult(response);
    }
} // namespace proto