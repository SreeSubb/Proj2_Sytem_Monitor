#include <dirent.h>
#include <unistd.h>
#include <sstream>
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
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
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
float LinuxParser::MemoryUtilization() {
  string line;
  string key, memory, kb;
  float MemoryTotal, MemoryFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()) {
    while(getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> memory >> kb) {
        if (key == "MemTotal") {
          MemoryTotal = stof(memory);
          // string MemoryTotal = memory;
        } else if (key == "MemFree") {
          MemoryFree = stof(memory);
          //string MemoryFree = memory;
        }
      }
    }
    return ((MemoryTotal - MemoryFree) / MemoryTotal);
  }
}
//{ return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  string idletime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return std::stol(uptime);
  }
  return std::stol(uptime);
}
//{ return 0; }
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  string StrPid = to_string(pid);
  vector<string> VecValues{};
  long TotalTime;
  std::ifstream stream(kProcDirectory + StrPid + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(std::getline(linestream, value, ' ')) {
        VecValues.push_back(value);
      }
    }
  }
  TotalTime = stol(VecValues[13]) + stol(VecValues[14]) + stol(VecValues[15]) + stol(VecValues[16]); // including children's time
  return TotalTime / sysconf(_SC_CLK_TCK);
}
//{ return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> CpuMode{};
  string line;
  string cpu, user, nice, system, idle, iowait;
  string irq, softirq, steal, guest, guestNice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice;
    CpuMode.insert(CpuMode.end(), {user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice});  
    }
  return CpuMode;
}
//{ return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
       while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      } 
    }
  }
  return std::stoi(value);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
}
//{ return 0; }
/* same as TotalProcesses*/
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string StrPid = to_string(pid);
  std::ifstream stream(kProcDirectory + StrPid + kCmdlineFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      return line;
    }
  }
}
//{ return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string StrPid = to_string(pid);
  string line;
  string key, memory;
  int RamMB;
  std::ifstream stream(kProcDirectory + StrPid + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');  
    std::istringstream linestream(line);
      while (linestream >> key >> memory) {
        if (key == "VmSize") {
          RamMB = stoi(memory) / 1024;
          return to_string(RamMB);
        }
      }
    }
  }
  return to_string(RamMB);
}
//{ return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string StrPid = to_string(pid);
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + StrPid + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value;
} 
//{ return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int uid) {
  string line;
  string username, value1, value2;
  string StrUid = to_string(uid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
      while (linestream >> username >> value1 >> value2) {
        if (value2 == StrUid ) {
          return username;
        }
      }
    }
  }
  return username;
}
//{ return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string StrPid = to_string(pid);
  string key;
  long time;
  vector<std::string> stat{};
  std::ifstream stream(kProcDirectory + StrPid + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(std::getline(linestream, key, ' ')) {
      //while(linestream, key, " ") {
        stat.push_back(key);
      }
    }
  }
  time = stol(stat[21]);
  return time / sysconf(_SC_CLK_TCK);
}
//{ return 0; }
