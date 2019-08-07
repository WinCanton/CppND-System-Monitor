#ifndef SYSINFO_H
#define SYSINFO_H
#pragma once
#include <string>
#include <vector>

#include "process.h"

class SysInfo {
    private:
        std::vector<std::string> lastCpuStats;
        std::vector<std::string> currentCpuStats;
        std::vector<std::string> coresStats;
        std::vector<std::vector<std::string>> lastCpuCoresStats;
        std::vector<std::vector<std::string>> currentCpuCoresStats;
        std::string cpuPercent;
        float memPercent;
        std::string osName;
        std::string kernelVer;
        long upTime;
        int totalProc;
        int runningProc;
        int threads;

    public:
        SysInfo() {
            this->getOtherCores(ProcessParser::getNumberOfCores());
            this->setLastCpuMeasures();
            this->setAttributes();
            this->osName = ProcessParser::getOsName();
            this->kernelVer = ProcessParser::getSysKernelVersion();
        }
        void setAttributes();
        void setLastCpuMeasures();
        std::string getMemPercent() const;
        long getUpTime() const;
        std::string getThreads() const;
        std::string getTotalProc() const;
        std::string getRunningProc() const;
        std::string getKernelVersion() const;
        std::string getOsName() const;
        std::string getCpuPercent() const;
        void getOtherCores(int _size);
        void setCpuCoresStats();
        std::vector<std::string> getCoresStats() const;
};

#endif
