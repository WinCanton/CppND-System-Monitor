#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"

void Process::setPid(int pid) { 
    this->pid = pid; 
}

std::string Process::getPid() const {
    return this->pid;
}

std::string Process::getUser() const { 
    return this->user; 
}


std::string Process::getCmd() const { 
    return this->cmd; 
}


int Process::getCpu() const {
    return stoi(this->cmd); 
}


int Process::getMem() const {
    return stoi(this->mem); 
}


std::string Process::getUpTime() const {
    return this->upTime;
}


std::vector<string> Process::getProcess()
{
    std::vector<string> processData;
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);
    processData.push_back(this->pid);
    processData.push_back(this->user);
    processData.push_back(this->cpu.substr(0,5));
    processData.push_back(this->mem.substr(0,5));
    processData.push_back(this->upTime.substr(0,5));
    processData.push_back(this->cmd.substr(0,30));

    return processData; 
}
