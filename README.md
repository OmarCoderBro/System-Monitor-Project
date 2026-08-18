# Linux System Monitor

A lightweight Linux system monitoring tool written in C++. The application reads system information directly from the Linux `/proc` virtual filesystem and displays live CPU usage, memory usage, process count, and system uptime.

## Features

* Real-time CPU utilization monitoring
* Memory usage tracking
* Running process count
* System uptime tracking
* Live terminal updates
* Dockerized Ubuntu Linux environment
* CMake-based build system

## Architecture

```text
                    Linux Kernel
                         │
        ┌────────────────┼────────────────┐
        │                │                │
        ▼                ▼                ▼
    /proc/stat      /proc/meminfo     /proc/uptime
        │                │                │
        ▼                ▼                ▼
    CPU Parser      Memory Parser    Uptime Parser
        │                │                │
        └────────────────┼────────────────┘
                         │
                         ▼
                 C++ System Monitor
                         │
                         ▼
                  Terminal Output
```

Process information is collected by scanning numeric process directories within `/proc`.

## Tech Stack

* **C++17**
* **Linux**
* **CMake**
* **Docker**
* **Ubuntu 24.04**

## How It Works

Linux exposes kernel and system information through the `/proc` virtual filesystem. This application reads several of these interfaces directly:

| Source          | Information                                         |
| --------------- | --------------------------------------------------- |
| `/proc/stat`    | CPU time counters used to calculate CPU utilization |
| `/proc/meminfo` | Total and available system memory                   |
| `/proc/uptime`  | System uptime                                       |
| `/proc/[PID]`   | Running processes                                   |

CPU utilization is calculated by sampling CPU time counters over a one-second interval and comparing total CPU time against idle time.

```text
Sample CPU counters
        ↓
Wait 1 second
        ↓
Sample counters again
        ↓
Calculate time differences
        ↓
CPU Utilization
```

## Project Structure

```text
linux-system-monitor/
├── CMakeLists.txt
├── Dockerfile
├── .dockerignore
└── src/
    └── main.cpp
```

## Running with Docker

### Build the image

```bash
docker build -t linux-system-monitor .
```

### Run the monitor

```bash
docker run --rm -it linux-system-monitor
```

The application runs inside an Ubuntu Linux Docker container, allowing it to access Linux's `/proc` filesystem even when developed from a non-Linux host.

## Example Output

```text
==============================
      Linux System Monitor
==============================
CPU Usage:  7.42%
Memory Usage
Total: 15943 MB
Used: 7342 MB
Processes: 87
Uptime: 12.4 hours
==============================
```

