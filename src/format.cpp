#include <string>

#include "format.h"

using std::string;

// DONE: this helper function converts seconds in long int format to HH:MM:SS string format 
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// DONE: REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string HH{std::to_string(seconds/60/60)};               // revisit, may need to use floor cmath function 
    string MM{std::to_string((seconds/60)%60)};
    string SS{std::to_string(seconds%60)};
    return HH + ":" + MM + ":" + SS;
}