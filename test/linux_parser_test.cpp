#include <string>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include "gtest/gtest.h"
#include "../include/linux_parser.h"

using std::string;

const std::filesystem::path kTestDir("test");
const std::filesystem::path kTestDataDir("testdata");

TEST(OperatingSystemTest, LinuxOSTest) {
  std::filesystem::path file("fake_os_release");
  std::filesystem::path os_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir / file;
  EXPECT_EQ(LinuxParser::OperatingSystem(os_data_path), "Ubuntu 22.04.4 LTS");
}

TEST(KernelTest, LinuxOSTest) {
  std::filesystem::path file("fake_proc_version");
  std::filesystem::path kernel_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir / file;
  EXPECT_EQ(LinuxParser::Kernel(kernel_data_path), "5.15.146.1-microsoft-standard-WSL2");
}

TEST(NumberOfProcessesTest, LinuxOSTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir / file;
  EXPECT_EQ(LinuxParser::RunningProcesses(stat_data_path), 2);
}

TEST(TotalNumberOfProcessesTest, LinuxOSTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir / file;
  EXPECT_EQ(LinuxParser::TotalProcesses(stat_data_path), 29904);
}

TEST(UptimeTest, LinuxOSTest) {
  std::filesystem::path file("fake_uptime");
  std::filesystem::path stat_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir / file;
  EXPECT_EQ(LinuxParser::UpTime(stat_data_path), 12198);
}

TEST(MemoryUtilizationTest, LinuxOSTest) {
  std::filesystem::path file("fake_meminfo");
  std::filesystem::path stat_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir / file;
  EXPECT_FLOAT_EQ(LinuxParser::MemoryUtilization(stat_data_path), 0.041948750);
}

TEST(PidTest, LinuxOSTest) {
  std::filesystem::path pid_dir_path = std::filesystem::current_path() / kTestDir / kTestDataDir;
  std::vector<int> expected{1, 103};
  std::vector<int> actual = LinuxParser::Pids(pid_dir_path.string());
  EXPECT_EQ(actual, expected);
}

TEST(UserIdMapTest, LinuxOSTest) {
  std::filesystem::path file("fake_etc_passwd");
  std::filesystem::path etc_passwd_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir / file;
  std::unordered_map<int, string> expected{{0,"root"}, {1,"daemon"}, {2, "bin"}, {1000, "foo"}};
  std::unordered_map<int, string> actual = LinuxParser::UserIdMap(etc_passwd_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(CommandTest, Process1Test) {
  std::filesystem::path root_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir;
  EXPECT_EQ(LinuxParser::Command(root_data_path, 1), "/sbin/init");
}

TEST(CommandTest, Process103Test) {
  std::filesystem::path root_data_path = std::filesystem::current_path() / kTestDir / kTestDataDir;
  EXPECT_EQ(LinuxParser::Command(root_data_path, 103), "/usr/lib/chromium-browser/chromium-browser --type=zygote --ppapi-flash-path=/usr/lib/adobe-fl");
}