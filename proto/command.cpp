#include "command.h"

namespace proto
{
    Command::Command(CommandId id, int channel)
    {
        this->id = id;
        this->channel = channel;
    }

    CommandId Command::GetId() const
    {
        return id;
    }

    std::string Command::GetResponse()
    {
        std::string response;

        if (response_result == Result::Ok)
        {
            response = "ok";
        }
        else
        {
            response = "fail";
        }
        return response;
    }

    std::string Command::IdToStr(CommandId id)
    {
        for (auto it : ID_NAME)
        {
            if (it.id == id)
            {
                return it.name;
            }
        }
        return IdToStr(CommandId::Unknown);
    }

    CommandId Command::StrToId(std::string id)
    {
        for (auto it : ID_NAME)
        {
            if (it.name == id)
            {
                return it.id;
            }
        }
        return CommandId::Unknown;
    }
} // namespace proto