#ifndef PROCESSCONTAINER_H
#define PROCESSCONTAINER_H
#pragma once
#include "process.h"
#include <string>
#include <vector>

class ProcessContainer {
    public:
        ProcessContainer() {
            this->refreshList();
        }
        void refreshList();
        //std::string printList();
        std::vector<std::vector<std::string>> getList();

    private:
        std::vector<Process>_list;
};

#endif 
