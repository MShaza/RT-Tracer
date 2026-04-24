# RT-Scope: High-Precision Linux Latency Profiler

**RT-Scope** is a C++17 utility designed to measure and analyze thread scheduling jitter across multiple CPU cores. It provides microsecond-accurate telemetry to identify OS interference and hardware interrupts in real-time environments.

---

## 🚀 Key Features
* **Hardware-Level Precision:** Uses busy-waiting and high-resolution steady clocks to measure sub-microsecond latency.
* **CPU Affinity:** Pins worker threads to specific cores using `sched_setaffinity` to observe core-specific noise.
* **Real-Time Priority:** Implements `SCHED_FIFO` (Priority 99) to bypass standard OS scheduling overhead.
* **O(1) Statistical Engine:** Custom Histogram implementation for memory-efficient tracking of **P50**, **P99**, and **Max** outliers.

---

## 📊 Understanding the Metrics
In real-time systems, the **Average** is a lie. RT-Scope focuses on the "Long Tail":

| Metric | Description | What it Reveals |
| :--- | :--- | :--- |
| **Min** | The hardware floor. | Base execution time of the measurement loop. |
| **Avg** | Arithmetic mean. | General system load and energy state. |
| **P99** | 99th Percentile. | Determinism. Shows the "worst-case" for 99% of tasks. |
| **Max** | Absolute outlier. | Hard Interrupts (IRQs) or kernel "Stop-the-world" events. |

---

## 🛠️ Build & Run

### Prerequisites
* Linux Environment (Tested on MacBook Air / Ubuntu)
* CMake 3.10+
* G++ (C++17 support)

### Installation
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make