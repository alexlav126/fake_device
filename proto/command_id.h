#ifndef __COMMAND_ID_H_
#define __COMMAND_ID_H_

namespace proto
{
    enum class CommandId
    {
        Unknown,
        StartMeasure,
        StopMeasure,
        SetRange,
        GetStatus,
        GetResult,
    };
}

#endif // __COMMAND_ID_H_