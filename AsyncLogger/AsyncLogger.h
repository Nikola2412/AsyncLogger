#pragma once
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

class AsyncLogger
{
public:
    enum class Level {
        Info,
        Warn,
        Error
    };

    struct LogItem {
        Level level;
        std::string message;
    };

public:
    virtual ~AsyncLogger();

    void Info(const std::string& msg) { Log(Level::Info, msg); }
    void Warn(const std::string& msg) { Log(Level::Warn, msg); }
    void Error(const std::string& msg) { Log(Level::Error, msg); }

protected:
    AsyncLogger();
    void Log(Level level, const std::string& msg);
    void ThreadFunc();
    void FlushRemaining();

    std::string Format(Level level, const std::string& msg);
    const char* LevelToString(Level level);

    virtual void output(const LogItem& item) = 0;  // FIX: pass-by-const-ref !!

protected:
    std::queue<LogItem> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_CV;

    std::atomic<bool> m_Running;
    std::thread m_Thread;
};

#include "Logger/AsyncConsoleLogger.h"
#include "Logger/AsyncFileLogger.h"