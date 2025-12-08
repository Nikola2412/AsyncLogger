#include "AsyncFileLogger.h"

AsyncFileLogger::AsyncFileLogger(const std::string& filename) : m_Filename(filename) ,m_Directory(LOG_DIR)
{
	openFile();
}

AsyncFileLogger::~AsyncFileLogger()
{
	if (m_File.is_open())
		m_File.close();
}


void AsyncFileLogger::output(const LogItem& item)
{
	if(m_File.is_open())
	{
		m_File << item.message << '\n';
	}
}

void AsyncFileLogger::openFile()
{

    // Close the file if it's already open
    if (m_File.is_open()) {
        m_File.close();
    }

    m_File.open(m_Filename, std::ios::app);

    if (!m_File.is_open()) {
        std::cerr << "Failed to open log file" << std::endl;
        std::abort();  // Abort the program if the file can't be opened
    }
}
