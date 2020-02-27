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
    std::vector<Process> tempProc;

    for (int pid : newProcesses) {
        Process pidProc = Process(pid);
        pidProc.Process::CpuUtilization();
        tempProc.emplace_back(pidProc);
    }

    std::sort(tempProc.begin(), tempProc.end());
    processes_ = tempProc;
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
    sysProcesses_ = SysProc[0];
    return SysProc[1];
}

// Return the total number of processes on the system
// Combined with Running Processes to avoid open/close of file
int System::TotalProcesses() { return sysProcesses_; }

// Returns the number of seconds since the system started running from proc/uptime/
long System::UpTime() {
    sysUpTime_ = LinuxParser::UpTime(); 
    return sysUpTime_;
}