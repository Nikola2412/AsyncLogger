#include "Logger/AsyncLogger.h"

#include <memory>

int main()
{

	std::unique_ptr<AsyncLogger> consoleLogger = std::make_unique<AsyncConsoleLogger>();
	consoleLogger->Info("Console log: This is an info message.");
	consoleLogger->Warn("Console log: This is a warning message.");
	consoleLogger->Error("Console log: This is an error message.");


	std::unique_ptr<AsyncLogger> logger = std::make_unique<AsyncFileLogger>("123.txt");
	logger->Info("File log: This is an info message.");

	AsyncLogger* test = new AsyncFileLogger("test.txt");
	test->Warn("File log: This is a warning message.");


	return 0;
}

