#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
string format;
int hours, minutes, secs;
hours = seconds / 3600;
minutes = (seconds % 3600) / 60;
secs = (seconds % 3600) % 60;
format = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(secs);
return format;
}
//{ return string(); }