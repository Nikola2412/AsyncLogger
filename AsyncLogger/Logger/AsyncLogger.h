#pragma once

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif

#define LOG_DIR "LogFiles/"

class AsyncLogger
{
public:
    enum class Level {
        Info,
        Warn,
        Error
    };
public:

    AsyncLogger(const std::string& filePath, bool enableConsole = true, bool enableFile = true);
    ~AsyncLogger();


    void Info(const std::string& msg) { Log(Level::Info, msg); }
    void Warn(const std::string& msg) { Log(Level::Warn, msg); }
    void Error(const std::string& msg) { Log(Level::Error, msg); }

private:
    void Log(Level level, const std::string& msg);
    void ThreadFunc();
    void FlushRemaining();

    void PrintColored(Level level, const std::string& msg);

private:

    std::string Format(Level level, const std::string& msg);

    const char* LevelToString(Level lvl) {
        switch (lvl) {
            case Level::Info:  return "INFO";
            case Level::Warn:  return "WARN";
            case Level::Error: return "ERROR";
        }
        return "UNKNOWN";
    }

    struct LogItem {
        Level level;
        std::string message;
    };

private:
    std::queue<LogItem> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_CV;
    std::atomic<bool> m_Running;
    std::thread m_Thread;
    std::ofstream m_File;

    bool m_ConsoleEnabled;
    bool m_FileEnabled;
};

