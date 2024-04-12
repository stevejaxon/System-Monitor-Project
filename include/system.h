#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <utility>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System(Processor cpu) : cpu_(std::move(cpu)) {}
  virtual Processor& Cpu() = 0;
  virtual std::vector<Process>& Processes() = 0;
  virtual float MemoryUtilization() = 0;
  virtual long UpTime() = 0;
  virtual int TotalProcesses() = 0;
  virtual int RunningProcesses() = 0;
  virtual std::string Kernel() = 0;
  virtual std::string OperatingSystem() = 0;

 protected:
  Processor cpu_;
  std::vector<Process> processes_ = {};
};

#endif