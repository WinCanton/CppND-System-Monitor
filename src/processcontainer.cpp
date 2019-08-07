#include "processcontainer.h"

void ProcessContainer::refreshList() {
    std::vector<std::string> pids = ProcessParser::getPidList();
    this->_list.clear();
    for (auto pid : pids) {
        Process proc(pid);
        this->_list.push_back(proc);
    }
}


std::vector<std::vector<std::string>> ProcessContainer::getList() {
    std::vector<std::vector<std::string>> values;
    for (std::size_t i = (this->_list.size()-10); i < this->_list.size(); i++) {
        values.push_back(this->_list[i].getProcess());
    }
    return values;
}
