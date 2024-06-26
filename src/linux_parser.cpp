#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "system_memory.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

/**
 *  @brief  Searches in an external file for a line starting with the specified
 * value, and returns the associated value.
 *  @param  filePath  A reference to the path to the file to read data from.
 *  @param  key  The word to search the file for, at the start of the line in
 * the file.
 *
 *  @returns The first matching token of type `Value` on the row that starts
 * with the specified word.
 */
template <typename Key, typename Value>
Value FindValue(const std::filesystem::path &filePath, const Key &key) {
  Key currentKey;
  Value currentValue;
  string line;
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> currentKey >> currentValue;
      if (currentKey == key) {
        stream.close();
        return currentValue;
      }
    }
    stream.close();
  }
  return currentValue;
}

vector<string> LinuxParser::Stats(const std::filesystem::path &filePath) {
  string line;
  std::ifstream stream(filePath);
  vector<string> tokens;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    tokens.assign(std::istream_iterator<string>(linestream), std::istream_iterator<string>());
    stream.close();
  }
  return tokens;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem(const std::filesystem::path &filePath) {
  string line;
  string key;
  string value;
  std::ifstream filestream(filePath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          filestream.close();
          return value;
        }
      }
    }
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel(const std::filesystem::path &filePath) {
  string os, version, kernel;
  string line;
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
  }
  return kernel;
}

vector<int> LinuxParser::Pids(const std::string &dirPath) {
  vector<int> pids;
  const std::filesystem::path directory{dirPath};
  for (auto const& dir_entry : std::filesystem::directory_iterator{directory}) {
    if (dir_entry.is_directory()) {
      // Is every character of the name a digit?
      string filename(dir_entry.path().filename());
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization(const std::filesystem::path &filePath) {
  float value, memTotal, memFree;
  string line, keyName;
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> keyName >> value;
      if (keyName == kMemTotalKey) {
        memTotal = value;
      } else if (keyName == kMemFreeKey) {
        memFree = value;
      }
    }
    stream.close();
  }
  // The output, display expects the utilization percentage to not be multiplied
  // by 100 e.g. 0.041 instead of 4.1.
  return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime(const std::filesystem::path &filePath) {
  long upTime;
  string line;
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
    stream.close();
  }
  return upTime;
}

long LinuxParser::Jiffies(const std::filesystem::path &filePath) {
  return LinuxParser::ActiveJiffies(filePath) +
         LinuxParser::IdleJiffies(filePath);
}

// NOTE: Provided function not required in this implementation
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies(const std::filesystem::path &filePath) {
  vector<string> values = LinuxParser::CpuUtilization(filePath);
  return stol(values[LinuxParser::kUser_]) + stol(values[LinuxParser::kNice_]) +
         stol(values[LinuxParser::kSystem_]) +
         stol(values[LinuxParser::kIRQ_]) +
         stol(values[LinuxParser::kSoftIRQ_]) +
         stol(values[LinuxParser::kSteal_]) +
         stol(values[LinuxParser::kGuest_]) +
         stol(values[LinuxParser::kGuestNice_]);
}

long LinuxParser::IdleJiffies(const std::filesystem::path &filePath) {
  vector<string> values = LinuxParser::CpuUtilization(filePath);
  return stol(values[LinuxParser::kIdle_]) +
         stol(values[LinuxParser::kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization(
    const std::filesystem::path &filePath) {
  vector<string> stats = Stats(filePath);
  // Remove the "cpu" element from the start of the vector.
  vector<string>(stats.begin() + 1, stats.end()).swap(stats);
  return stats;
}

int LinuxParser::TotalProcesses(const std::filesystem::path &filePath) {
  return FindValue<std::string, int>(filePath, kTotalProcsKey);
}

int LinuxParser::RunningProcesses(const std::filesystem::path &filePath) {
  return FindValue<std::string, int>(filePath, kNumRunningProcsKey);
}

string LinuxParser::Command(const std::filesystem::path &filePathRoot,
                            int pid) {
  std::filesystem::path filePath = filePathRoot /
                                   std::filesystem::path(std::to_string(pid)) /
                                   kCmdlineFilePath;
  string line{""};
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    std::getline(stream, line);
    stream.close();
    // Handles the situation where some commandlines contain null characters.
    std::replace(line.begin(), line.end(), '\0', ' ');
  }
  return line;
}

string LinuxParser::Ram(const std::filesystem::path &filePathRoot, int pid) {
  std::filesystem::path filePath = filePathRoot /
                                   std::filesystem::path(std::to_string(pid)) /
                                   kMemoryUtilizationFilePath;
  int amount;
  string line, key, unitValue;
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> amount >> unitValue;
      if (key == kMemoryUtilizationKey) {
        stream.close();
        SystemMemory::Unit unit = SystemMemory::UnitFromString(unitValue);
        return SystemMemory::Utilization(unit, amount).ToMbString();
      }
    }
    stream.close();
  }
  return "0";
}

string LinuxParser::Uid(const std::filesystem::path &filePathRoot, int pid) {
  std::filesystem::path filePath =
      filePathRoot / std::filesystem::path(std::to_string(pid)) / kUidFilePath;
  return FindValue<string, string>(filePath, kUidKey);
}

// NOTE: Provided function not required in this implementation
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// NOTE: Provided function not required in this implementation
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

std::unordered_map<string, string> LinuxParser::UserIdMap(
    const std::filesystem::path &filePath) {
  std::unordered_map<string, string> uid_map;

  string line, datum, userName;
  std::ifstream stream(filePath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      // Split the line stream on the ':' delimiter, storing the value in the
      // `datum` variable.
      std::getline(linestream, datum, ':');
      userName = datum;
      // The second portion of the line is not what we need, so we ignore it.
      std::getline(linestream, datum, ':');
      std::getline(linestream, datum, ':');
      // The third portion of the line is the user's ID.
      uid_map[datum] = userName;
    }
    stream.close();
  }
  return uid_map;
}