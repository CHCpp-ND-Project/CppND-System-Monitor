#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <fstream>
#include <string>

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
    float prevIdle_{0};     // Instead of using time, track previous values
    float prevCpuTotal_{0};
};

#endif