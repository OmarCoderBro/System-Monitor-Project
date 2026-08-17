#include <filesystem>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <algorithm>
#include <cctype>

double getCpuUsage() {
    std::ifstream file("/proc/stat");

    if (!file) {
        return -1.0;
    }

    std::string line;
    std::getline(file, line);

    std::istringstream stream(line);

    std::string cpu;
    long long user, nice, system, idle, iowait;

    stream >> cpu >> user >> nice >> system >> idle >> iowait;

    long long idleTime = idle + iowait;
    long long totalTime = user + nice + system + idle + iowait;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    file.clear();
    file.seekg(0);

    std::getline(file, line);

    std::istringstream stream2(line);

    stream2 >> cpu >> user >> nice >> system >> idle >> iowait;

    long long idleTime2 = idle + iowait;
    long long totalTime2 = user + nice + system + idle + iowait;

    long long totalDelta = totalTime2 - totalTime;
    long long idleDelta = idleTime2 - idleTime;

    return 100.0 * (1.0 - static_cast<double>(idleDelta) / totalDelta);
}

void printMemoryUsage() {
    std::ifstream file("/proc/meminfo");

    if (!file) {
        std::cerr << "Failed to open /proc/meminfo\n";
        return;
    }

    std::string label;
    long totalMemory = 0;
    long availableMemory = 0;
    long value;
    std::string unit;

    while (file >> label >> value >> unit) {
        if (label == "MemTotal:") {
            totalMemory = value;
        } else if (label == "MemAvailable:") {
            availableMemory = value;
        }
    }

    long usedMemory = totalMemory - availableMemory;

    std::cout << "Memory Usage\n";
    std::cout << "Total: " << totalMemory / 1024 << " MB\n";
    std::cout << "Used:  " << usedMemory / 1024 << " MB\n";
}

int getProcessCount() {
    int count = 0;

    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        std::string name = entry.path().filename().string();

        if (!name.empty() &&
            std::all_of(name.begin(), name.end(), ::isdigit)) {
            count++;
        }
    }

    return count;
}

double getUptime() {
    std::ifstream file("/proc/uptime");

    if (!file) {
        return -1.0;
    }

    double uptimeSeconds;
    file >> uptimeSeconds;

    return uptimeSeconds;
}

int main() {
    std::cout << "Starting Linux System Monitor..." << std::endl;

    while (true) {
        std::cout << "\n====================" << std::endl;
        std::cout << "Linux System Monitor" << std::endl;
        std::cout << "====================" << std::endl;

        double cpuUsage = getCpuUsage();

        std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;

        printMemoryUsage();
        std::cout << "Processes: " << getProcessCount() << std::endl;

        double uptime = getUptime();

        std::cout << "Uptime: " << uptime / 3600 << " hours" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}