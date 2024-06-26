#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

const std::string kTotalProcsKey{"processes"};
const std::string kNumRunningProcsKey{"procs_running"};
const std::string kMemTotalKey{"MemTotal:"};
const std::string kMemFreeKey{"MemFree:"};
const std::string kUidKey{"Uid:"};
const std::string kMemoryUtilizationKey{"VmSize:"};

const std::filesystem::path kCmdlineFilePath("cmdline");
const std::filesystem::path kUidFilePath("status");
const std::filesystem::path kMemoryUtilizationFilePath("status");
const std::filesystem::path kProcStatFilePath("stat");

const int kUtimeStatIndex = 13;
const int kStimeStatIndex = 14;
const int kCutimeStatIndex = 15;
const int kCstimeStatIndex = 16;
const int kStarttimeStatIndex = 21;

// System
float MemoryUtilization(const std::filesystem::path &filePath);
long UpTime(const std::filesystem::path &filePath);
std::vector<int> Pids(const std::string &dirPath);
int TotalProcesses(const std::filesystem::path &filePath);
int RunningProcesses(const std::filesystem::path &filePath);
std::string OperatingSystem(const std::filesystem::path &filePath);
std::string Kernel(const std::filesystem::path &filePath);
std::unordered_map<std::string, std::string> UserIdMap(const std::filesystem::path &filePath);
std::vector<std::string> Stats(const std::filesystem::path &filePath);

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization(const std::filesystem::path &filePath);
long Jiffies(const std::filesystem::path &filePath);
long ActiveJiffies(const std::filesystem::path &filePath);
long ActiveJiffies(int pid);
long IdleJiffies(const std::filesystem::path &filePath);

// Processes
std::string Command(const std::filesystem::path &filePathRoot, int pid);
std::string Ram(const std::filesystem::path &filePathRoot, int pid);
std::string Uid(const std::filesystem::path &filePathRoot, int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif