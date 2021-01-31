#include "logger.h"

#include <cstdlib>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <ctime>

using namespace std;

void Logger::Log(Logger::Level level, const char *format, ...)
{
    logger_lock.lock();

    if (!IsLoggable(level))
    {
        logger_lock.unlock();
        return;
    }

    time_t t = time(nullptr);
    char time_buf[100];
    strftime(time_buf, sizeof time_buf, "%F %T", gmtime(&t));
    va_list args1;
    va_start(args1, format);
    va_list args2;
    va_copy(args2, args1);
    vector<char> buf(1 + vsnprintf(nullptr, 0, format, args1));
    va_end(args1);
    vsnprintf(buf.data(), buf.size(), format, args2);
    va_end(args2);

    if (buf.size() > 2)
    {
        // remove last '\n'
        if (buf.at(buf.size() - 2) == '\n')
        {
            buf.pop_back();
            buf.at(buf.size() - 1) = '\0';
        }
    }

    // print to STDOUT
    printf("%s [%s]: %s\n",
           time_buf,
           LevelToStr(level),
           buf.data());

    // print to file
    FILE *file = fopen("server.log", "a");
    if (!file)
    {
        logger_lock.unlock();
        return;
    }
    fprintf(file,
            "%s [%s]: %s\n",
            time_buf,
            LevelToStr(level),
            buf.data());
    fclose(file);

    logger_lock.unlock();
}

void Logger::SetLevel(Logger::Level level)
{
    Logger::current_level = level;
}

bool Logger::IsLoggable(Logger::Level level)
{
    switch (current_level)
    {
    case Logger::Level::NONE:
        return false;
    case Logger::Level::ERR:
        return level == Logger::Level::ERR;
    case Logger::Level::WAR:
        return (level == Logger::Level::ERR) ||
               (level == Logger::Level::WAR);
    case Logger::Level::INF:
        return (level == Logger::Level::ERR) ||
               (level == Logger::Level::WAR) ||
               (level == Logger::Level::INF);
    case Logger::Level::DBG:
        return (level == Logger::Level::ERR) ||
               (level == Logger::Level::WAR) ||
               (level == Logger::Level::INF) ||
               (level == Logger::Level::DBG);
    default:
        return false;
    }
}

const char *Logger::LevelToStr(Level level)
{
    switch (level)
    {
    case Logger::Level::NONE:
        return "NONE";
    case Logger::Level::ERR:
        return "ERR";
    case Logger::Level::WAR:
        return "WAR";
    case Logger::Level::INF:
        return "INF";
    case Logger::Level::DBG:
        return "DBG";
    default:
        return "?";
    }
}
