#include "AsyncLogger.h"

AsyncLogger::AsyncLogger(const std::string& filePath, bool enableConsole, bool enableFile) 
    :   m_Running(true),
        m_ConsoleEnabled(enableConsole),
        m_FileEnabled(enableFile)
{

#ifdef _WIN32
    // Enable ANSI colors on Windows 10+ terminals
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    if (m_FileEnabled)
        m_File.open(filePath, std::ios::out | std::ios::app);
	m_Thread = std::thread(&AsyncLogger::ThreadFunc, this);
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
    std::string formatted = Format(level, msg);
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Queue.push({ level, formatted});
    }
    m_CV.notify_one();
}

void AsyncLogger::ThreadFunc()
{
    while (m_Running) {
        std::unique_lock<std::mutex> lock(m_Mutex);

        m_CV.wait(lock, [&]() {
            return !m_Queue.empty() || !m_Running;
            });

        while (!m_Queue.empty()) {
            const LogItem item = std::move(m_Queue.front());
            m_Queue.pop();
            lock.unlock();

            // Write to console
            if (m_ConsoleEnabled)
                PrintColored(item.level, item.message);

            // Write to file
            if (m_FileEnabled)
                m_File << item.message << std::endl;

            lock.lock();
        }
    }

}

void AsyncLogger::FlushRemaining()
{
    while (!m_Queue.empty()) {
        const LogItem& item = m_Queue.front();

        if (m_ConsoleEnabled)
            PrintColored(item.level, item.message);

        if (m_FileEnabled)
            m_File << item.message << std::endl;

        m_Queue.pop();
    }

    if (m_FileEnabled)
        m_File.flush();
}

void AsyncLogger::PrintColored(Level level, const std::string& msg)
{
#ifdef _WIN32
    const char* color = "";

    switch (level) {
    case Level::Info:  color = "\033[37m"; break; // White
    case Level::Warn:  color = "\033[33m"; break; // Yellow
    case Level::Error: color = "\033[31m"; break; // Red
    }

    std::cout << color << msg << "\033[0m" << std::endl;
#else
	std::cout << msg << std::endl;
#endif
}

std::string AsyncLogger::Format(Level level, const std::string& msg)
{
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;

#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif
    std::ostringstream oss;

    oss << "[" << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << LevelToString(level) << "] "
        << msg;

    return oss.str();
}
