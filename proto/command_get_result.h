#ifndef __COMMAND_GET_RESULT_H_
#define __COMMAND_GET_RESULT_H_

#include <string>
#include "command.h"

namespace proto
{
    class CommandGetResult : public Command
    {
    public:
        using Callback = bool (*)(int channel,
                                  float *output_result);
        CommandGetResult(int channel, Callback callback);
        void ProcessCallback() override;
        std::string GetResponse() override;
        std::string GetRequest() override;
        Result CheckResponse(const std::string &response, float *result);

    private:
        Callback callback;
        float result;
    };
} // namespace proto
#endif // __COMMAND_GET_RESULT_H_