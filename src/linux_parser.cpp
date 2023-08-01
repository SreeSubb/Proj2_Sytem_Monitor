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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

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
          try {
            MemoryTotal = stof(memory);
          } catch (std::invalid_argument& e) {
            MemoryTotal = 0; }
        } else if (key == "MemFree") {
          try {
            MemoryFree = stof(memory);
          } catch (std::invalid_argument& e) {
            MemoryFree = 0; }
          }
      }
    }
  }
  return ((MemoryTotal - MemoryFree) / MemoryTotal);
}

long LinuxParser::UpTime() {
  string line;
  string uptime;
  string idletime;
  long SysUpTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    try {
      SysUpTime = std::stol(uptime);
    } catch (std::invalid_argument& e) {
      SysUpTime = 0;
      }
  }
  return SysUpTime;
}

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
        VecValues.emplace_back(value);
      }
    }
  }
  try {
    TotalTime = (stof(VecValues[13]) + stof(VecValues[14]) + stof(VecValues[15]) + stof(VecValues[16])); // including children's time
  } catch (std::invalid_argument& e) {
    TotalTime = 0;
  }
  return TotalTime / sysconf(_SC_CLK_TCK);
}

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

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int total;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          try {
          total = std::stoi(value);
          } catch (std::invalid_argument& e) {
          total = 0;
          }
        }
      } 
    }
  }
  return total;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          try {
            processes = std::stoi(value);
            } catch (std::invalid_argument& e) {
              processes = 0;
            }
        }
      }
    }
  }
  return processes;
}

string LinuxParser::Command(int pid) {
  string line;
  string StrPid = to_string(pid);
  std::ifstream stream(kProcDirectory + StrPid + kCmdlineFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      return line;
    }
  }
  return line;
}

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
        if (key == "VmRSS") {
          try {
            RamMB = stoi(memory) / 1024;
            } catch (std::invalid_argument& e) {
              RamMB = 0;
              }
              return to_string(RamMB);
        }
      }
    }
  }
  return to_string(RamMB);
}

string LinuxParser::Uid(int pid) {
  string StrPid = to_string(pid);
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + StrPid + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
} 

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

long LinuxParser::UpTime(int pid) {
  string line;
  string StrPid = to_string(pid);
  string key;
  long Starttime, UpTime;
  vector<std::string> stat{};
  std::ifstream stream(kProcDirectory + StrPid + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(std::getline(linestream, key, ' ')) {
        stat.emplace_back(key);
      }
    }
  }
  try {
    Starttime = stol(stat[21]);
    } catch (std::invalid_argument& e) {
      Starttime = 0;
      }
      UpTime = LinuxParser::UpTime() - (Starttime / sysconf(_SC_CLK_TCK));
      return UpTime;
}
