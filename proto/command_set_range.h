#ifndef __COMMAND_SET_RANGE_H_
#define __COMMAND_SET_RANGE_H_

#include <string>
#include "command.h"

namespace proto
{
    class CommandSetRange : public Command
    {
    public:
        using Callback = bool (*)(int channel,
                                  int input_range,
                                  int *output_range);
        CommandSetRange(int channel, int range, Callback callback = nullptr);
        static constexpr char PREFIX_RANGE[] = "range";
        void ProcessCallback() override;
        std::string GetResponse() override;
        std::string GetRequest() override;
        Result CheckResponse(const std::string &response, int *range);

    private:
        Callback callback;
        int range;
    };
} // namespace proto
#endif // __COMMAND_SET_RANGE_H_