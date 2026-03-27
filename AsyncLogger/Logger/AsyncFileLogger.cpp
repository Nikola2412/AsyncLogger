#include "AsyncFileLogger.h"

namespace fs = std::filesystem;

AsyncFileLogger::AsyncFileLogger(const std::string& name, const std::string& filename, const std::string& dirname) : m_Filename(filename), m_Directory(dirname)
{
	this->m_Name = name;
	openFile();
}

AsyncFileLogger::~AsyncFileLogger()
{
	if (m_File.is_open())
		m_File.close();
}


void AsyncFileLogger::output(const LogItem& item)
{
    std::lock_guard<std::mutex> lock(m_Mutex);


    if (item.message.empty()) {
        return;
    }

    /*if (!m_File.is_open()) {
        m_File.open(m_Path, std::ios::app);
        if (!m_File.is_open()) {
            std::cerr << "AsyncFileLogger: failed to open log file: " << m_Path << std::endl;
            return;
        }
    }*/

    //m_File.clear();

    m_File << item.message << '\n';

    /*if (m_File.fail()) {
        std::cerr << "AsyncFileLogger: write failed, attempting to recover"<< std::endl;

        m_File.clear();
        if (m_File.is_open()) {
            m_File.close();
        }

        m_File.open(m_Path, std::ios::app);

        if (!m_File.is_open()) {
            std::cerr << "AsyncFileLogger: reopen failed: " << m_Path << std::endl;
            return;
        }

        m_File << item.message << '\n';
        if (m_File.fail()) {
            std::cerr << "AsyncFileLogger: write failed after reopen: " << m_Path << std::endl;
            m_File.clear();
            return;
        }
    }*/

    m_File.flush();
    /*if (m_File.fail()) {
        std::cerr << "AsyncFileLogger: flush failed for file: " << m_Path << std::endl;
        m_File.clear();
    }*/
}

void AsyncFileLogger::openFile()
{
    if (m_File.is_open()) {
        m_File.close();
    }

    if(m_Directory.back() != '/')
		m_Directory += '/';

    if(!fs::exists(m_Directory))
		fs::create_directory(m_Directory);

    m_Path = m_Directory + m_Filename;

    m_File.open(m_Path, std::ios::out);

    if (!m_File.is_open()) {
		onError();
    }
}

void AsyncFileLogger::onError()
{
	m_Running = false;


    std::string error = "error.txt";
    std::string errPath = LOG_DIR + error;
	std::ofstream errFile(errPath, std::ios::out);

    if (!fs::exists(LOG_DIR))
        fs::create_directory(LOG_DIR);

    errFile << "DIR: " << m_Directory << '\n';
    errFile << "FILE: " << m_Filename << '\n';

    errFile.flush();

}
