#ifndef __PROTO_H_
#define __PROTO_H_

#include "command_id.h"
#include "command.h"
#include "command_start_measure.h"
#include "command_stop_measure.h"
#include "command_set_range.h"
#include "command_get_status.h"
#include "command_get_result.h"

#include <string>
#include <vector>

namespace proto
{
    class Proto
    {
    public:
        struct Callback
        {
            CommandStartMeasure::Callback start_measure;
            CommandStopMeasure::Callback stop_measure;
            CommandSetRange::Callback set_range;
            CommandGetStatus::Callback get_status;
            CommandGetResult::Callback get_result;
        };
        Proto(Callback callback);
        Command *CreateCommand(const std::string &request);
        static constexpr char PREFIX_CHANNEL[] = "channel";

    private:
        static int ExtractInt(const std::string &prefix, const std::string &s);
        static std::vector<std::string> ParseRequest(const std::string &request);
        static inline void PrepareString(std::string &s);
        static inline bool IsNotTrimmedChar(unsigned char c);
        static inline void LTrim(std::string &s);
        static inline void RTrim(std::string &s);
        static inline void ToLower(std::string &s);
        Callback callback;
    };
} // namespace proto
#endif // __PROTO_H_