#include "Logger/AsyncLogger.h"

#include <memory>

int main()
{

	std::unique_ptr<AsyncLogger> consoleLogger = std::make_unique<AsyncConsoleLogger>();
	consoleLogger->Info("Console log: This is an info message.");


	std::unique_ptr<AsyncLogger> logger = std::make_unique<AsyncFileLogger>("test.txt");
	logger->Info("File log: This is an info message.");

	return 0;
}

