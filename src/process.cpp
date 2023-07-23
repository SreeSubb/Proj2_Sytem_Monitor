#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization  // cpu utilization for the specific PID
float Process::CpuUtilization() const {     //activejiffies(Pid)/UpTime(Pid)
    long active, elapsed;
    active = LinuxParser::ActiveJiffies(pid);
    elapsed = LinuxParser::UpTime(pid);
    return float(active) / float(elapsed);  
}
//{ return 0; }

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid);
}
//{ return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(pid);
}
//{ return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() {
    string UidStr = LinuxParser::Uid(pid);
    return LinuxParser::User(stoi(UidStr));
}
//{ return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid);
}
//{ return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return CpuUtilization() < a.CpuUtilization();   // calls operator overloading
}  
//{ return true; }