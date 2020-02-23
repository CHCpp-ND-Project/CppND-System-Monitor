#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// DONE: Read and return the system uptime
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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 500; }

// TODO: Read and return the number of running processes
// save off TotalProcesses
int LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  long upTime;
  std::ifstream filestream(kProcDirectory + kStatFilename);                  // Create input file stream
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {     //extract key and value as first two elements from iss
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }