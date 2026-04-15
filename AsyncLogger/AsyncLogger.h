#pragma once
#ifndef ASYNCLOGGER_H
#define ASYNCLOGGER_H

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
#include <vector>
#include <memory>

#define ASYNC_NAME "AsyncLogger"

class AsyncLogger
{
protected:
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
	AsyncLogger(const std::string& name) : m_Name(name), m_Running(true), m_Thread(&AsyncLogger::ThreadFunc, this) {}
    AsyncLogger(std::vector<std::shared_ptr<AsyncLogger>> loggers,const std::string& name = ASYNC_NAME);
    virtual ~AsyncLogger();

    void Info(const std::string& msg) { Log(Level::Info, msg); }
    void Warn(const std::string& msg) { Log(Level::Warn, msg); }
    void Error(const std::string& msg) { Log(Level::Error, msg); }

    void AddLogger(std::shared_ptr<AsyncLogger> logger) {
        logger->setName(this->getName());
        m_Loggers.push_back(logger);
    }

    void setName(const std::string& name) { this->m_Name = name; }
    std::string& setName() { return this->m_Name; }

	std::string getName() const { return this->m_Name; }

protected:
    AsyncLogger() : AsyncLogger(ASYNC_NAME) {}

    void Log(Level level, const std::string& msg);
    void ThreadFunc();
    void FlushRemaining();

    std::string Format(Level level, const std::string& msg);
    const char* LevelToString(Level level);

    virtual void output(const LogItem& item) {};


protected:
	std::string m_Name;

    std::queue<LogItem> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_CV;

    std::atomic<bool> m_Running;
    std::thread m_Thread;

	std::vector<std::shared_ptr<AsyncLogger>> m_Loggers;
};

#include "Logger/AsyncConsoleLogger.h"
#include "Logger/AsyncFileLogger.h"

#endif // !ASYNCLOGGER_H
