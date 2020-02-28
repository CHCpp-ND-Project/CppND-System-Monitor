#include <string>

#include "format.h"

using std::string;

// this helper function converts seconds in long int format to HH:MM:SS string format 
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    string HH{std::to_string(seconds/60/60)};               // revisit, may need to use floor cmath function 
    string MM{std::to_string((seconds/60)%60)};
    string SS{std::to_string(seconds%60)};
    return HH + ":" + MM + ":" + SS;
}