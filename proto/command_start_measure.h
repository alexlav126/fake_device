#ifndef __COMMAND_START_MEASURE_H_
#define __COMMAND_START_MEASURE_H_

#include <string>
#include "command.h"

namespace proto
{
    class CommandStartMeasure : public Command
    {
    public:
        using Callback = bool (*)(int channel);
        CommandStartMeasure(int channel, Callback callback);
        void ProcessCallback() override;
        std::string GetResponse() override;
        std::string GetRequest() override;
        Result CheckResponse(const std::string &response);

    private:
        Callback callback;
    };
} // namespace proto
#endif // __COMMAND_START_MEASURE_H_