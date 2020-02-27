#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    std::string key, cpuUser, cpuNice, cpuSystem, cpuIdle, cpuIOWait, cpuIrq, cpuSoftIrq, cpuSteal, cpuGuest, cpuGuestNice;
    float cpuUserF, cpuNiceF, cpuSystemF, cpuIdleF, cpuIOWaitF, cpuIrqF, cpuSoftIrqF, cpuStealF, cpuGuestF, cpuGuestNiceF;
    float CPU_Percentage{0};
    std::string line;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);                  // Create input file stream
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        // extract key and values from row
        linestream  >> key >> cpuUser >> cpuNice >> cpuSystem
                    >> cpuIdle >> cpuIOWait >> cpuIrq >> cpuSoftIrq >> cpuSteal 
                    >> cpuGuest >> cpuGuestNice;
        cpuIdleF = std::stof(cpuIdle);
        cpuIOWaitF = std::stof(cpuIOWait);
        cpuUserF = std::stof(cpuUser);
        cpuNiceF = std::stof(cpuNice);
        cpuSystemF = std::stof(cpuSystem);
        cpuIrqF = std::stof(cpuIrq);
        cpuSoftIrqF = std::stof(cpuSoftIrq);
        cpuStealF  = std::stof(cpuSteal);

        float idle = cpuIdleF + cpuIOWaitF;
        float nonIdle = cpuUserF + cpuNiceF + cpuSystemF + cpuIrqF + cpuSoftIrqF + cpuStealF;
        float cpuTotal = idle + nonIdle;
        float cpuTotalDelta = cpuTotal - prevCpuTotal_;
        float cpuTotalIdled = idle - prevIdle_;

        CPU_Percentage = (cpuTotalDelta - cpuTotalIdled)/cpuTotalDelta;
        prevIdle_ = idle;
        prevCpuTotal_ = cpuTotal;
    }
    
    return CPU_Percentage;
}