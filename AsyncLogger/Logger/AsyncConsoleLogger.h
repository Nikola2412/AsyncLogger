#pragma once

#include "../AsyncLogger.h"

#ifdef _WIN32
	#include <windows.h>
#endif

class AsyncConsoleLogger : public AsyncLogger
{
public:
	AsyncConsoleLogger();

private:
	void output(const LogItem& item) override;
};

