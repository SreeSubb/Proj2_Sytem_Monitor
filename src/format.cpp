#include <iostream>
#include <iomanip>
#include <string>
#include "format.h"
using std::string;
using std::stringstream;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
int hours, minutes, secs;
hours = seconds / 3600;
minutes = (seconds % 3600) / 60;
secs = (seconds % 3600) % 60;
stringstream format{};
format << std::setw(2) << std::setfill('0') << hours 
       << std::setw(1) << ":"
       << std::setw(2) << std::setfill('0') << minutes 
       << std::setw(1) << ":"
       << std::setw(2) << std::setfill('0') << secs;
return format.str();
}