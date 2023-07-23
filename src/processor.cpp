#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {     // systm utilization
    std::vector<std::string> UsageCPU = LinuxParser::CpuUtilization();
    return (stof(UsageCPU[0]) + stof(UsageCPU[1]) + stof(UsageCPU[2]) + stof(UsageCPU[5]) + 
    stof(UsageCPU[6]) + stof(UsageCPU[7])) / (stof(UsageCPU[0]) + stof(UsageCPU[1]) + 
    stof(UsageCPU[2]) + stof(UsageCPU[3]) + stof(UsageCPU[4]) + stof(UsageCPU[5]) + 
    stof(UsageCPU[6]) + stof(UsageCPU[7]));
}
//{ return 0.0; }