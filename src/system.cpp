#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> CurrentPids = LinuxParser::Pids();
    //Process Current;
    for(auto &p : CurrentPids) {
        Process Current = Process(p);
        processes_.push_back(Current);     
    }
    std::sort(processes_.begin(), processes_.end(), [] (Process p, Process q) { return p < q; }); // call operator overloading
    return processes_;
}
//{ return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    return LinuxParser::Kernel();
}
//{ return string(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}
//{ return 0.0; }

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}
//{ return string(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}
//{ return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime(); 
}
//{ return 0; }
