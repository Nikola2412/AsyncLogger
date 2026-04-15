#pragma once
#ifndef ASYNCCONSOLELOGGER_H
#define ASYNCCONSOLELOGGER_H

#include "../AsyncLogger.h"

#ifdef _WIN32
	#include <windows.h>
#endif

class AsyncConsoleLogger : public AsyncLogger
{
public:
	AsyncConsoleLogger(const std::string& name = "AsyncConsoleLogger");
	//AsyncConsoleLogger();

private:
	void output(const LogItem& item) override;
};

#endif // !ASYNCCONSOLELOGGER_H
