#include "Logger/AsyncLogger.h"

#include <memory>

int main()
{

	std::unique_ptr<AsyncConsoleLogger> consoleLogger = std::make_unique<AsyncConsoleLogger>();
	consoleLogger->Info("Console log: This is an info message.");
	consoleLogger->Warn("Console log: This is a warning message.");
	consoleLogger->Error("Console log: This is an error message.");


	std::unique_ptr<AsyncFileLogger> logger = std::make_unique<AsyncFileLogger>("async_log.txt");
	logger->Info("File log: This is an info message.");


	return 0;
}

