#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"
#include <iostream>
using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes sorted by cpu utilization
vector<Process>& System::Processes() { 
    std::vector<int> newProcesses = LinuxParser::Pids();
    
    std::vector<int> lastProcesses;

    for (Process proc : processes_) {
        lastProcesses.emplace_back(proc.Pid());
    }

    for (int pid : newProcesses) {

        // create an iterator borrowed from https://thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/
        if(std::find(lastProcesses.begin(), lastProcesses.end(), pid) == lastProcesses.end())  //has to create an iterator every time

	        //Process pid_new = Process(pid);
            processes_.emplace_back(Process(pid));
    }
    // debug int size  = processes_.size();
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// if kernel is instantiated value, lookup kernel.  Then return kernel to avoid unncessary calculations
std::string System::Kernel() { 
    if (Kernel_ == "Kernel_TBD") {
        Kernel_ = LinuxParser::Kernel();    
    }
    return Kernel_; 
}

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// if OS is instantiated value, lookup OS.  Then return OS to avoid unncessary calculations
std::string System::OperatingSystem() { 
    if (OS_ == "OS_TBD") {
        OS_ = LinuxParser::OperatingSystem();    
    }
    return OS_; 
}

// Returns the number of processes actively running on the system
int System::RunningProcesses() { 
    vector<int> SysProc = LinuxParser::RunningProcesses();
    sysProcesses_ = SysProc[1];
    return SysProc[0];
}

// Return the total number of processes on the system
// Combined with Running Processes to avoid open/close of file
int System::TotalProcesses() { return sysProcesses_; }

// Returns the number of seconds since the system started running from proc/uptime/
long System::UpTime() { return LinuxParser::UpTime(); }