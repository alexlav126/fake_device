#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <mutex>

class Logger
{
public:
    enum class Level
    {
        NONE,
        DBG,
        INF,
        WAR,
        ERR
    };

    static void Log(Level level, const char *format, ...);
    static void SetLevel(Level level);

private:
    Logger();
    static bool IsLoggable(Level level);
    static const char *LevelToStr(Level level);

    inline static std::mutex logger_lock;
    inline static Level current_level;
};

#endif // __LOGGER_H_