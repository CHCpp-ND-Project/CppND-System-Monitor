#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// Process Constructor
Process::Process(int pid){
    pid_ = pid;
    cpu_ = 1;
}
// Return this process's ID
int Process::Pid() { return pid_; }

// set private member cpu_
void Process::setProcCPUUtilization(float cpu) {
    cpu_ = cpu;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    vector<string> pT = LinuxParser::CpuUtilization(pid_);
    // procTimes vector 0:utime, 1:stime, 2:cutime, 3:cstime, 4:startTime
    float total_time = std::stof(pT[0]) + std::stof(pT[1]) +
                        std::stof(pT[2]) + std::stof(pT[3]);
    // elapsed time seconds = uptime - (starttime / Hertz)
    float freq = (float) sysconf(_SC_CLK_TCK);
    float seconds = (float) LinuxParser::UpTime() - (std::stof(pT[4])/ freq);
    float cpu = (total_time/freq)/seconds;
    Process::setProcCPUUtilization(cpu);
    return cpu;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process by crossreferencing with user id (number)
string Process::User() { 
    std::string userNum = LinuxParser::User(pid_); // returns the numeric string
    return LinuxParser::Uid(userNum);
}
// Return the age of this process (in seconds) to be formatted by ncurses_display.cpp
long int Process::UpTime() {
    return LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return cpu_ > a.cpu_; 
}