#include "AsyncConsoleLogger.h"

AsyncConsoleLogger::AsyncConsoleLogger() 
{
#ifdef _WIN32
    // Enable ANSI colors on Windows 10+ terminals
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}



void AsyncConsoleLogger::output(const LogItem& item)
{
#ifdef _WIN32
    const char* color = "";

    switch (item.level) {
    case Level::Info:  color = "\033[37m"; break; // White
    case Level::Warn:  color = "\033[33m"; break; // Yellow
    case Level::Error: color = "\033[31m"; break; // Red
    }

    std::cout << color << item.message << "\033[0m" << '\n';
#else
    std::cout << item.message << '\n';
#endif
}
