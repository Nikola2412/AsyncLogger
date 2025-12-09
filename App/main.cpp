#include <AsyncLogger.h>

#include <memory>
#include <thread>
#include <chrono>

int main()
{
	std::unique_ptr<AsyncConsoleLogger> consoleLogger = std::make_unique<AsyncConsoleLogger>();
	std::unique_ptr<AsyncFileLogger> fileLogger = std::make_unique<AsyncFileLogger>();

	consoleLogger->Info("Console log: This is an info message.");
	consoleLogger->Warn("Console log: This is a warning message.");
	consoleLogger->Error("Console log: This is an error message.");

	fileLogger->Info("File log: This is an info message.");
	fileLogger->Warn("File log: This is a warning message.");
	fileLogger->Error("File log: This is an error message.");



	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	return 0;
}