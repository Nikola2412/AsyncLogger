#include "Logger/AsyncLogger.h"

#include <memory>

int main()
{
	AsyncLogger logger("async_log.txt");
	logger.Info("This is an info message.");
	logger.Warn("This is a warning message.");
	logger.Error("This is an error message.");

	std::unique_ptr<AsyncLogger> ptrLogger = std::make_unique<AsyncLogger>("unique_ptr_log.txt");
	ptrLogger->Info("Unique pointer info message.");
	ptrLogger->Warn("Unique pointer warning message.");
	ptrLogger->Error("Unique pointer error message.");

	AsyncLogger *file = new AsyncLogger("file_only_log.txt", false, true);
	file->Info("File only info message.");
	file->Warn("File only warning message.");
	file->Error("File only error message.");
	delete file;
}

