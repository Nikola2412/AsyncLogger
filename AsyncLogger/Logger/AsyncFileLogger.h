#pragma once
#include "AsyncLogger.h"
#include <fstream>

#define LOG_DIR "LogFiles/"

class AsyncFileLogger : public AsyncLogger
{
public:
	AsyncFileLogger(const std::string& filename);
	~AsyncFileLogger();
	void setFilename(const std::string& filename) { m_Filename = filename; openFile(); }
	void setDirectory(const std::string& directory) { m_Filename = m_Directory; openFile(); }


private:
	void output(const LogItem& item) override;
	void openFile();

private:
	std::string m_Directory;
	std::string m_Filename;
	std::ofstream m_File;
};

