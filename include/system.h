#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      

  // Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  // Define variables that don't change as private members
  // kernel & OS to prevent multiple function calls
  std::string Kernel_ = {"Kernel_TBD"};
  std::string OS_ = {"OS_TBD"};
  int sysProcesses_ = {0};
  long sysUpTime_ = {0};
};

#endif