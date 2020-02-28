#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);                          // Create input file stream
  if (filestream.is_open()) {                                 // if file stream is open...
    while (std::getline(filestream, line)) {                  // while there are lines left to get...
      // process line to be able to stringstream more readily
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);                    // create input stringstream
      while (linestream >> key >> value) {                    // extract key and value as first two elements from iss
        if (key == "PRETTY_NAME") {                           // check if key matches what we are looking for
          std::replace(value.begin(), value.end(), '_', ' '); // return to human readible format
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;                                        // string initialization
  string line;
  // create input file stream to access file
  std::ifstream stream(kProcDirectory + kVersionFilename);  // defined in .h file
  if (stream.is_open()) {                                   // confirm file is open
    std::getline(stream, line);                             // get new line from stream and assign to 'line'
    std::istringstream linestream(line);                    // convert to input string stream 'linestream'
    linestream >> os >> version >> kernel;                  // shift three times to get 'kernel'
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
// this function is to pull out all process id (integral values in /proc/)
// map to processes
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());         // from dirent.h
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization as a percentage of free remaining
float LinuxParser::MemoryUtilization() {
  string line;
  string key, value;
  float memTotal{0}, memFree{0}; // , memAvailable{0}, buffers{0}
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    int clear = 0;
    do {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = std::stoi(value);
        clear ++;
      } else if (key == "MemFree:") {
        memFree = std::stoi(value);
        clear ++;
      } /*else if (key == "MemAvailable:") {
        memAvailable = std::stoi(value);
        clear ++;
      } else if (key == "Buffers:") {
        buffers = std::stoi(value);
        clear ++;
      }*/
    } while ((std::getline(filestream, line)) || clear < 3);
  }
  return (memTotal-memFree)/memTotal; 
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string sysUpTime, sysIdleTime;
  string line;
  long upTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);                  // Create input file stream
  if (filestream.is_open()) {                                 // if file stream is open...
    std::getline(filestream, line);                    // while there are lines left to get...
    std::istringstream linestream(line);                    // create input stringstream
    linestream >> sysUpTime >> sysIdleTime;
    upTime = std::stol(sysUpTime);
  }
  return upTime;
}
/*
// unused: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// unused: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// unused: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// unused: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }
*/
// DOING: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid) {
  vector<string> procTimes{"0"};
  std::string currentPID = "/" + std::to_string(pid) + "/";
  std::string line;
  std::string nullValue;
  std::string utime{0}, stime{0}, cutime{0}, cstime{0}, starttime{0};

  std::ifstream stream(kProcDirectory + currentPID + kStatFilename);  // defined in .h file
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=0; i<22; i++) {
      if (i == 13) {
        linestream >> utime >> stime >> cutime >> cstime;
        i = 17;
      } else if (i == 21) {
        linestream >> starttime;
      } else {
        linestream >> nullValue;
      }
    }
  }
  // return vector of times as this library doens't have access to total uptime
  procTimes.emplace_back(utime);
  procTimes.emplace_back(stime);
  procTimes.emplace_back(cutime);
  procTimes.emplace_back(cstime);
  procTimes.emplace_back(starttime);
  return procTimes; 
}

// see RunningProcesses() to avoid functon call: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 500; } // function not called

// Read and return the number of running processes
// save off TotalProcesses
vector<int> LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  vector<int> procReturn{-1,-1};

  std::ifstream filestream(kProcDirectory + kStatFilename);                  // Create input file stream
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {     //extract key and value as first two elements from iss
        if (key == "processes") {
          procReturn[0] = std::stoi(value);
        } else if (key == "procs_running") {
          procReturn[1] = std::stoi(value);
        }
        if (procReturn[0] != -1 && procReturn[1] != -1) {
          return procReturn;
        }
      }
    }
  }
  return procReturn;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string currentPID = "/" + std::to_string(pid) + "/";
  std::string line;
  std::ifstream stream(kProcDirectory + currentPID + kCmdlineFilename);  // defined in .h file
  if (stream.is_open()) { 
    std::getline(stream, line);
    return line;
    //while (std::getline(stream, line)) {
      //std::istringstream linestream(line);
      //linestream >> key >> value;
      //if (key == "Uid:") {
      //return line;
      //} 
    //}  
  }

  return string(); 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string currentPID = "/" + std::to_string(pid) + "/";
  std::string line;
  std::string key, value;
  std::ifstream stream(kProcDirectory + currentPID + kStatusFilename);  // defined in .h file
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        return std::to_string(std::stoi(value)/1000);
      }
    }
  }
  return std::string{"0"};
}

// Read and return the user ID associated with a process by taking userNumber string
string LinuxParser::Uid(std::string userNum) {
  std::string line;
  std::string key, x, value;
  std::ifstream stream(kPasswordPath);  // defined in .h file
  if (stream.is_open()) { 
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      
      std::istringstream linestream(line);
      linestream >> value >> x >> key;
      if (key == userNum) {
        return value;
      }
    }  
  }
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  std::string currentPID = "/" + std::to_string(pid) + "/";
  std::string line;
  std::string key, value;
  std::ifstream stream(kProcDirectory + currentPID + kStatusFilename);  // defined in .h file
  if (stream.is_open()) { 
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      } 
    }  
  }
  return string(); 
}

// Read and return the uptime of a process
long int LinuxParser::UpTime(int pid) {
  std::string currentPID = "/" + std::to_string(pid) + "/";
  std::string line;
  std::string key, value;
  long int pidTime{0};
  std::ifstream stream(kProcDirectory + currentPID + kStatFilename);  // defined in .h file
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i=0; i<21; i++) {
        linestream >> key;
      }
      linestream >> value;
      pidTime = std::stoll(value)/sysconf(_SC_CLK_TCK);
    }
  }
  return pidTime; 
}