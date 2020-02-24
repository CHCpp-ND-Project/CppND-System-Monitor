#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// if kernel is instantiated value, lookup kernel.  Then return kernel to avoid unncessary calculations
std::string System::Kernel() { 
    if (Kernel_ == "Kernel_TBD") {
        Kernel_ = LinuxParser::Kernel();    
    }
    return Kernel_; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// if OS is instantiated value, lookup OS.  Then return OS to avoid unncessary calculations
std::string System::OperatingSystem() { 
    if (OS_ == "OS_TBD") {
        OS_ = LinuxParser::OperatingSystem();    
    }
    return OS_; 
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    vector<int> SysProc = LinuxParser::RunningProcesses();
    sysProcesses_ = SysProc[1];
    return SysProc[0];
}

// DONE, combined with Running Processes to avoid open/close of file
// Return the total number of processes on the system
int System::TotalProcesses() { return sysProcesses_; }

// Return the number of seconds since the system started running from proc/uptime/
long System::UpTime() { return LinuxParser::UpTime(); }