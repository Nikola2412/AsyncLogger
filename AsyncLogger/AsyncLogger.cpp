#include "AsyncLogger.h"

AsyncLogger::AsyncLogger()
{
    m_Running = true;
    m_Thread = std::thread(&AsyncLogger::ThreadFunc, this);
}

AsyncLogger::AsyncLogger(std::vector<std::shared_ptr<AsyncLogger>> loggers) {
    m_Loggers = std::move(loggers);
    m_Running = false;
}

AsyncLogger::~AsyncLogger()
{
    m_Running = false;
    m_CV.notify_one();
    if (m_Thread.joinable())
        m_Thread.join();

    FlushRemaining();
}

void AsyncLogger::Log(Level level, const std::string& msg)
{

    if (!m_Loggers.empty()) {
        for (auto &logger : m_Loggers) {
            if (logger) {
                logger->Log(level, msg);
            }
        }
        return;
    }

    std::string formatted = Format(level, msg);

    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Queue.push(LogItem{ level, formatted });
    }

    m_CV.notify_one();
}

void AsyncLogger::ThreadFunc()
{
    while (m_Running)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);

        m_CV.wait(lock, [&] {
            return !m_Queue.empty() || !m_Running;
            });

        if (!m_Running && m_Queue.empty())
            break;

        const LogItem item = std::move(m_Queue.front());
        m_Queue.pop();

        lock.unlock();

        output(item);
    }
}

void AsyncLogger::FlushRemaining()
{
    while (true) {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (m_Queue.empty()) break;
        const LogItem item = std::move(m_Queue.front());
        m_Queue.pop();
        lock.unlock();
        output(item);
    }
}

const char* AsyncLogger::LevelToString(Level level)
{
    switch (level) {
    case Level::Info:  return "INFO";
    case Level::Warn:  return "WARN";
    case Level::Error: return "ERROR";
    }
    return "UNKNOWN";
}


std::string AsyncLogger::Format(Level level, const std::string& msg)
{
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);

    std::tm tm_now;
#ifdef _WIN32
    localtime_s(&tm_now, &t);
#else
    localtime_r(&t, &tm_now);
#endif

    std::ostringstream oss;
    oss << "[" << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << LevelToString(level) << "] "
        << msg;
    return oss.str();
}
