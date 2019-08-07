#ifndef PROCESS_H
#define PROCESS_H
#pragma once
#include <processparser.h>
#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(std::string pid) {
      this->pid = pid;
      this->user = ProcessParser::getProcUser(pid);
      this->mem = ProcessParser::getVmSize(pid);
      this->cmd = ProcessParser::getCmd(pid);
      this->upTime = ProcessParser::getProcUpTime(pid);
      this->cpu = ProcessParser::getCpuPercent(pid);
  }

  void setPid(int pid);
  std::string getPid() const;
  std::string getUser() const;
  std::string getCmd() const;
  int getCpu() const;
  int getMem() const;
  std::string getUpTime() const;
  std::vector<string> getProcess();


  // DONE: Declare any necessary private members
 private:
  std::string pid;
  std::string user;
  std::string cmd;
  std::string cpu;
  std::string mem;
  std::string upTime;
};

#endif
