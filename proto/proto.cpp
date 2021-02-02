#include "proto.h"
#include <algorithm>

namespace proto
{
    Proto::Proto(Callback callback)
    {
        this->callback = callback;
    }

    Command *Proto::CreateCommand(const std::string &request)
    {
        auto items = ParseRequest(request);
        const int POS_COMMAND_NAME = 0;
        const int POS_CHANNEL = 1;

        if (items.size() <= POS_CHANNEL)
        {
            return nullptr;
        }
        auto cmd_id = Command::StrToId(items.at(POS_COMMAND_NAME));
        int channel = ExtractInt(Command::PREFIX_CHANNEL,
                                 items.at(POS_CHANNEL));

        switch (cmd_id)
        {
        case CommandId::StartMeasure:
            return new CommandStartMeasure(channel, callback.start_measure);
        case CommandId::StopMeasure:
            return new CommandStopMeasure(channel, callback.stop_measure);
        case CommandId::SetRange:
        {
            const int POS_RANGE = 2;
            if (items.size() <= POS_RANGE)
            {
                return nullptr;
            }
            int range = ExtractInt(CommandSetRange::PREFIX_RANGE,
                                   items.at(POS_RANGE));
            return new CommandSetRange(channel, range, callback.set_range);
        }
        case CommandId::GetStatus:
            return new CommandGetStatus(channel, callback.get_status);
        case CommandId::GetResult:
            return new CommandGetResult(channel, callback.get_result);
        default:
            return nullptr;
        }
    }

    int Proto::ExtractInt(const std::string &prefix, const std::string &s)
    {
        size_t pos = s.find(prefix);
        if (pos == std::string::npos)
        {
            return -1;
        }
        int result = std::stoi(s.substr(pos + prefix.size()));
        return result;
    }

    std::vector<std::string> Proto::ParseRequest(const std::string &request)
    {
        std::vector<std::string> items;
        std::string delim = " ";

        size_t prev = 0, pos = 0;
        size_t req_len = request.length();
        do
        {
            pos = request.find(delim, prev);
            if (pos == std::string::npos)
            {
                pos = req_len;
            }
            std::string item = request.substr(prev, pos - prev);
            if (!item.empty())
            {
                items.push_back(item);
            }
            prev = pos + delim.length();
        } while (pos < req_len && prev < req_len);

        for (auto &it : items)
        {
            PrepareString(it);
        }

        return items;
    }

    inline void Proto::PrepareString(std::string &s)
    {
        ToLower(s);
        LTrim(s);
        RTrim(s);
    }

    inline bool Proto::IsNotTrimmedChar(unsigned char c)
    {
        return !(std::isspace(c) || c == ',' || c == '\n');
    }

    inline void Proto::LTrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), IsNotTrimmedChar));
    }

    inline void Proto::RTrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), IsNotTrimmedChar).base(),
                s.end());
    }

    inline void Proto::ToLower(std::string &s)
    {
        std::transform(s.begin(),
                       s.end(),
                       s.begin(),
                       [](unsigned char c) { return std::tolower(c); });
    }

} // namespace proto