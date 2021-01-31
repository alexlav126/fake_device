#ifndef __COMMAND_STOP_MEASURE_H_
#define __COMMAND_STOP_MEASURE_H_

#include <string>
#include "command.h"

namespace proto
{
    class CommandStopMeasure : public Command
    {
    public:
        using Callback = bool (*)(int channel);
        CommandStopMeasure(int channel, Callback callback);
        void ProcessCallback() override;
        std::string GetResponse() override;

    private:
        Callback callback;
    };
} // namespace proto
#endif // __COMMAND_STOP_MEASURE_H_