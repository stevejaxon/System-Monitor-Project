#include <string>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include "gtest/gtest.h"
#include "../include/linux_parser.h"

using std::string;

const std::filesystem::path kTestDir("test");
const std::filesystem::path kTestDataDir("testdata");
const std::filesystem::path kTestDataDirPath = std::filesystem::current_path() / kTestDir / kTestDataDir;

TEST(OperatingSystemTest, LinuxOSTest) {
  std::filesystem::path file("fake_os_release");
  std::filesystem::path os_data_path = kTestDataDirPath / file;
  EXPECT_EQ(LinuxParser::OperatingSystem(os_data_path), "Ubuntu 22.04.4 LTS");
}

TEST(KernelTest, LinuxOSTest) {
  std::filesystem::path file("fake_proc_version");
  std::filesystem::path kernel_data_path = kTestDataDirPath / file;
  EXPECT_EQ(LinuxParser::Kernel(kernel_data_path), "5.15.146.1-microsoft-standard-WSL2");
}

TEST(NumberOfProcessesTest, LinuxOSTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  EXPECT_EQ(LinuxParser::RunningProcesses(stat_data_path), 2);
}

TEST(TotalNumberOfProcessesTest, LinuxOSTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  EXPECT_EQ(LinuxParser::TotalProcesses(stat_data_path), 90601);
}

TEST(UptimeTest, LinuxOSTest) {
  std::filesystem::path file("fake_uptime");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  EXPECT_EQ(LinuxParser::UpTime(stat_data_path), 12198);
}

TEST(MemoryUtilizationTest, LinuxOSTest) {
  std::filesystem::path file("fake_meminfo");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  EXPECT_FLOAT_EQ(LinuxParser::MemoryUtilization(stat_data_path), 0.041948750);
}

TEST(PidTest, LinuxOSTest) {
  std::filesystem::path pid_dir_path = kTestDataDirPath;
  std::vector<int> expected{1, 103, 75, 78};
  std::vector<int> actual = LinuxParser::Pids(pid_dir_path.string());
  EXPECT_EQ(actual, expected);
}

TEST(UserIdMapTest, LinuxOSTest) {
  std::filesystem::path file("fake_etc_passwd");
  std::filesystem::path etc_passwd_data_path = kTestDataDirPath / file;
  std::unordered_map<string, string> expected{{"0","root"}, {"1","daemon"}, {"2", "bin"}, {"1000", "foo"}};
  std::unordered_map<string, string> actual = LinuxParser::UserIdMap(etc_passwd_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(CpuUtilizationTest, FakeStatTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  std::vector<string> expected = {"30632", "119", "29094", "51232413", "1853", "0", "1882", "0", "0", "0"};
  std::vector<string> actual = LinuxParser::CpuUtilization(stat_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(CpuUtilizationTest, FakeCPUStatTest) {
  std::filesystem::path file("fake_cpu_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  std::vector<string> expected = {"74608", "2520", "24433", "1117073", "6176", "4054", "0", "0", "0", "0"};
  std::vector<string> actual = LinuxParser::CpuUtilization(stat_data_path);
  EXPECT_EQ(actual, expected);
}


TEST(ActiveJiffiesTest, FakeStatTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  const long expected = 61727;
  const long actual = LinuxParser::ActiveJiffies(stat_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(ActiveJiffiesTest, FakeCPUStatTest) {
  std::filesystem::path file("fake_cpu_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  const long expected = 105615;
  const long actual = LinuxParser::ActiveJiffies(stat_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(IdleJiffiesTest, FakeStatTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  const long expected = 51234266;
  const long actual = LinuxParser::IdleJiffies(stat_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(IdleJiffiesTest, FakeCPUStatTest) {
  std::filesystem::path file("fake_cpu_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  const long expected = 1123249;
  const long actual = LinuxParser::IdleJiffies(stat_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(JiffiesTest, FakeStatTest) {
  std::filesystem::path file("fake_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  const long expected = 51295993;
  const long actual = LinuxParser::Jiffies(stat_data_path);
  EXPECT_EQ(actual, expected);
}

TEST(JiffiesTest, FakeCPUStatTest) {
  std::filesystem::path file("fake_cpu_stat");
  std::filesystem::path stat_data_path = kTestDataDirPath / file;
  const long expected = 1228864;
  const long actual = LinuxParser::Jiffies(stat_data_path);
  EXPECT_EQ(actual, expected);
}


TEST(ProcCommandTest, Process1Test) {
  std::filesystem::path root_data_path = kTestDataDirPath;
  EXPECT_EQ(LinuxParser::Command(root_data_path, 1), "/sbin/init");
}

TEST(ProcCommandTest, Process75Test) {
  std::filesystem::path root_data_path = kTestDataDirPath;
  EXPECT_EQ(LinuxParser::Command(root_data_path, 75), "snapfuse /var/lib/snapd/snaps/bare_5.snap /snap/bare/5 -o ro,nodev,allow_other,suid ");
}

TEST(ProcCommandTest, Process103Test) {
  std::filesystem::path root_data_path = kTestDataDirPath;
  EXPECT_EQ(LinuxParser::Command(root_data_path, 103), "/usr/lib/chromium-browser/chromium-browser --type=zygote --ppapi-flash-path=/usr/lib/adobe-fl");
}

TEST(ProcStatusTest, Process1Test) {
  EXPECT_EQ(LinuxParser::Uid(kTestDataDirPath, 1), "0");
}

TEST(ProcStatusTest, Process103Test) {
  EXPECT_EQ(LinuxParser::Uid(kTestDataDirPath, 103), "1000");
}

TEST(ProcRamTest, Process1Test) {
  EXPECT_EQ(LinuxParser::Ram(kTestDataDirPath, 1), "165");
}

TEST(ProcRamTest, Process75Test) {
  EXPECT_EQ(LinuxParser::Ram(kTestDataDirPath, 75), "4");
}

TEST(ProcRamTest, Process103Test) {
  EXPECT_EQ(LinuxParser::Ram(kTestDataDirPath, 103), "457");
}

TEST(ProcStatsTest, Process1Test) {
  std::filesystem::path stats_data_path = kTestDataDirPath / std::filesystem::path("1") / LinuxParser::kProcStatFilePath;
  std::vector<string> actual = LinuxParser::Stats(stats_data_path);
  EXPECT_EQ(actual.size(), 52);
  EXPECT_EQ(actual[0], "1");
  EXPECT_EQ(actual[1], "(systemd)");
  EXPECT_EQ(actual[LinuxParser::kUtimeStatIndex], "285");
  EXPECT_EQ(actual[LinuxParser::kStimeStatIndex], "39");
  EXPECT_EQ(actual[LinuxParser::kCutimeStatIndex], "78");
  EXPECT_EQ(actual[LinuxParser::kCstimeStatIndex], "24");
  EXPECT_EQ(actual[LinuxParser::kStarttimeStatIndex], "76");
}

TEST(ProcStatsTest, Process75Test) {
  std::filesystem::path stats_data_path = kTestDataDirPath / std::filesystem::path("75") / LinuxParser::kProcStatFilePath;
  std::vector<string> actual = LinuxParser::Stats(stats_data_path);
  EXPECT_EQ(actual.size(), 52);
  EXPECT_EQ(actual[0], "75");
  EXPECT_EQ(actual[1], "(snapfuse)");
  EXPECT_EQ(actual[LinuxParser::kUtimeStatIndex], "0");
  EXPECT_EQ(actual[LinuxParser::kStimeStatIndex], "0");
  EXPECT_EQ(actual[LinuxParser::kCutimeStatIndex], "0");
  EXPECT_EQ(actual[LinuxParser::kCstimeStatIndex], "0");
  EXPECT_EQ(actual[LinuxParser::kStarttimeStatIndex], "217");
}

TEST(ProcStatsTest, Process103Test) {
  std::filesystem::path stats_data_path = kTestDataDirPath / std::filesystem::path("103") / LinuxParser::kProcStatFilePath;
  std::vector<string> actual = LinuxParser::Stats(stats_data_path);
  EXPECT_EQ(actual.size(), 52);
  EXPECT_EQ(actual[0], "2879");
  EXPECT_EQ(actual[1], "(chromium-browse)");
  EXPECT_EQ(actual[LinuxParser::kUtimeStatIndex], "3");
  EXPECT_EQ(actual[LinuxParser::kStimeStatIndex], "48");
  EXPECT_EQ(actual[LinuxParser::kCutimeStatIndex], "91213");
  EXPECT_EQ(actual[LinuxParser::kCstimeStatIndex], "12278");
  EXPECT_EQ(actual[LinuxParser::kStarttimeStatIndex], "3570");
}