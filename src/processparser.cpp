#include "processparser.h"

std::string ProcessParser::getVmSize(std::string pid) {
    std::string line;
    //Declaring search attribute for file
    std::string name = "VmData";
    std::string value;
    float result;
    //Opening stream for specific line
    std::ifstream stream = Util::getStream(std::string (Path::basePath() + pid + Path::statusPath()));
    while(std::getline(stream, line)) {
        //Searching line by line
        if (line.compare(0, name.size(), name) == 0) {
            // slicing string line on ws for values using sstream
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            //conversion kB to GB
            result = (stof(values[1])/float(1024));
            break;
        }
    }
    return std::to_string(result);
}


std::vector<std::string> ProcessParser::getPidList() {
    DIR* dir;
    
    std::vector<std::string> container;
    if (!(dir = opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));

    while (dirent* dirp = readdir(dir)) {
        // is this a directory?
        if(dirp->d_type != DT_DIR)
            continue;

        // Is every character of the name a digit?
        if (all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c){ return std::isdigit(c); })) {
            container.push_back(dirp->d_name);
        }
    }
    
    // Validating process of directory closing
    if (closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
}


int ProcessParser::getTotalThreads() {
    //get every process and read their number of threads
    std::string name = "Threads:";
    std::vector<std::string> pids = ProcessParser::getPidList();
    int TotalThreads = 0;
    for (std::size_t it=0; it < pids.size(); it++)  {
        std::ifstream stream = Util::getStream((Path::basePath() + pids[it] + Path::statusPath()));
        std::string line;

        while (std::getline(stream, line)) {
            if (line.compare(0, name.size(), name) == 0) {
                std::istringstream buf(line);
                std::istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> values(beg, end);
                TotalThreads += stoi(values[1]);
            }
        }
    }
    return TotalThreads;
}


int ProcessParser::getTotalNumberOfProcesses()
{
    string line;
    int result = 0;
    string name = "processes";
    ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}


int ProcessParser::getNumberOfRunningProcesses()
{
    string line;
    int result = 0;
    string name = "procs_running";
    ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}


std::string ProcessParser::getOsName() {
    std::string line;
    std::string name = "PRETTY_NAME=";
    std::string result = "";

    std::ifstream stream = Util::getStream(("/etc/os-release"));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::size_t found = line.find("=");
            found++;
            result = line.substr(found);
            result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
            return result;
        }
    }
    return "";
}


std::string ProcessParser::getSysKernelVersion() {
    std::string line;
    std::string name = "Linux version ";

    std::ifstream stream = Util::getStream((Path::basePath() + Path::versionPath()));
    std::getline(stream, line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> values(beg, end);
    return values[2];
}

float ProcessParser::getSysRamPercent() {
    std::string line;
    std::string name1 = "MemTotal:";
    std::string name2 = "MemFree:";
    std::string name3 = "Buffers:";
    float totalMem = 0;
    float freeMem = 0;
    float buffers = 0;

    std::ifstream stream = Util::getStream((Path::basePath() + Path::memInfoPath()));
    while (std::getline(stream, line)) {
        std::istringstream buf(line);
        std::istream_iterator<std::string> beg(buf), end;
        std::vector<std::string> values(beg, end);

        if (values[0] == name1) 
            totalMem = stof(values[1]);
        if (values[0] == name2)
            freeMem = stof(values[1]);
        if (values[0] == name3)
            buffers = stof(values[1]);
        if (totalMem != 0 && freeMem != 0 && buffers != 0)
            break;
    }
    return float(100.0 * (1 - freeMem / (totalMem-buffers)));
}


float ProcessParser::getSysActiveCpuTime(vector<string> values) {
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) + 
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}


float ProcessParser::getSysIdleCpuTime(vector<string> values) {
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}


std::string ProcessParser::printCpuStats(std::vector<std::string> values1, std::vector<std::string> values2) {
    float activeTime = (ProcessParser::getSysActiveCpuTime(values2) - ProcessParser::getSysActiveCpuTime(values1));
    float idleTime = ProcessParser::getSysIdleCpuTime(values2) - ProcessParser::getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0 * (activeTime/totalTime);
    std::string s = (std::to_string(result));
    return s;
}


std::vector<std::string> ProcessParser::getSysCpuPercent(string coreNumber) {
    string line;
    string name = "cpu" + coreNumber;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
        std::istringstream buf(line);
        std::istream_iterator<std::string> beg(buf), end;
        std::vector<std::string> values(beg, end);
        if (values[0] == name) {
            return values;
        }
    }
    return (std::vector<std::string>());
}


int ProcessParser::getNumberOfCores() {
    //Get the number of host cpu cores
    string line;
    string name = "cpu cores";
    std::ifstream stream = Util::getStream((Path::basePath() + "cpuinfo"));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            return stoi(values[3]);
        }
    }
    return 0;
}


std::string ProcessParser::getCmd(std::string pid) {
    std::string line;
    std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::cmdPath()));
    std::getline(stream, line);
    return line;
}


std::string ProcessParser::getProcUser(std::string pid) {
    std::string line;
    std::string name = "Uid";
    std::string result = "";
    std::string etcp = "/etc/passwd";

    std::ifstream stream = Util::getStream(std::string (Path::basePath() + pid + Path::statusPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            result = values[1];
            break;
        }
    }

    stream = Util::getStream(etcp);
    name = ("x:" + result);
    while (std::getline(stream, line)) {
        if (line.find(name) != std::string::npos)  {
           result = line.substr(0, line.find(":")); 
           return result;
        }
    }
    return "";
}


long int ProcessParser::getSysUpTime() {
    std::string line;
    std::ifstream stream = Util::getStream(std::string (Path::basePath() + Path::upTimePath()));
    std::getline(stream, line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> values(beg, end);
    // Using sysconf to get clock ticks of the host machine
    return stoi(values[0]);
}


std::string ProcessParser::getProcUpTime(string pid) {
    std::string line;
    std::ifstream stream = Util::getStream((Path::basePath() + pid + "/" + Path::statPath()));
    std::getline(stream, line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> values(beg, end);
    // Using sysconf to get clock ticks of the host machine
    return std::to_string(stof(values[13])/sysconf(_SC_CLK_TCK));
}


std::string ProcessParser::getCpuPercent(std::string pid) {
    std::string line;
    //opening stream for the file
    std::ifstream stream = Util::getStream((Path::basePath() + pid + "/" + Path::statPath()));
    std::getline(stream, line);
    std::istringstream buf(line);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> values(beg, end);
    //acquiring relevant times for calculation of active occupation of CPU for selected process
    float utime = stof(ProcessParser::getProcUpTime(pid));
    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);
    float uptime = ProcessParser::getSysUpTime();
    float freq = sysconf(_SC_CLK_TCK);
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    float result = 100.0 * ((total_time/freq)/seconds);
    return std::to_string(result);
}
