#include "Logger/AsyncLogger.h"

#include <memory>
#include <thread>
#include <chrono>

int main()
{
	std::unique_ptr<AsyncConsoleLogger> consoleLogger = std::make_unique<AsyncConsoleLogger>();
	consoleLogger->Info("Console log: This is an info message.");
	consoleLogger->Warn("Console log: This is a warning message.");
	consoleLogger->Error("Console log: This is an error message.");

	std::unique_ptr<AsyncFileLogger> logger = std::make_unique<AsyncFileLogger>();
	logger->Info("File log: This is an info message.");
	logger->Warn("File log: This is a warning message.");
	logger->Error("File log: This is an error message.");


	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	return 0;
}

