#pragma once
#ifndef ASYNCFILELOGGER_H
#define ASYNCFILELOGGER_H

#include "../AsyncLogger.h"
#include <fstream>
#include <filesystem>

#define LOG_FILENAME "async_log.txt"
#define LOG_DIR "LogFiles/"

class AsyncFileLogger : public AsyncLogger
{
public:
	AsyncFileLogger(const std::string& name = "AsyncFileLogger", const std::string& filename = LOG_FILENAME, const std::string& dirnamne = LOG_DIR);
	//AsyncFileLogger(const std::string& filename = LOG_FILENAME, const std::string& dirnamne = LOG_DIR);
	~AsyncFileLogger();
	//void setFilename(const std::string& filename) { m_Filename = filename; openFile(); }
	//void setDirectory(const std::string& directory) { m_Filename = m_Directory; openFile(); }


private:
	void output(const LogItem& item) override;
	void openFile();

	void onError();

private:
	std::string m_Path;
	std::string m_Directory;
	std::string m_Filename;
	std::ofstream m_File;
};

#endif // !ASYNCFILELOGGER_H