# ⚡ The Geyser Programming Language (.gy)

**Geyser** is a strict, explicit, performance-oriented systems programming language designed for real-time engines, robotics, and high-performance simulators.

Geyser uses an LLVM-22 backend with a C/C++-powered compiler toolchain. Its goal is to deliver high-performance native code while leaving behind decades of legacy C++ backward-compatibility constraints.

Geyser is an independent programming language project, separate from GeyserMC, the Minecraft connectivity project.

---

## 🎯 Why Geyser? (The Catch)

Modern software shouldn't have to accept unpredictable garbage-collection pauses or unnecessary language complexity.

Geyser pursues predictable execution and high hardware performance through two core architectural principles.

### ❌ The "Yeet the GC" Memory Strategy

* **No Garbage Collector:** Geyser is designed without a garbage collector, avoiding garbage-collection pauses.
* **Predictable Execution:** The goal is to reduce sources of unpredictable latency in real-time workloads.

Not having a garbage collector does not eliminate every possible source of latency. Actual timing behavior depends on the generated code, runtime environment, operating system, and hardware.

### 🔍 Absolute Explicitness

* **No Compiler Guessing Games:** Geyser emphasizes explicit data types, scope boundaries, statement terminators, and logical evaluation.
* **Compile-Time Diagnostics:** The compiler should reject syntax errors and other invalid constructs it can identify during compilation, rather than allowing those errors to proceed to execution.

These checks address the errors Geyser is designed to detect. They do not guarantee that every program is safe or free of runtime errors.

### 🖥️ Team 3 Notice: Hardware Testing Infrastructure

The plan is to establish a hardware-testing server rack containing machines with different architectures, including AArch64 and x86-64, running Geyser's `gy.exe` toolchain where supported.

Microcontrollers will use a separate connection arrangement: a dongle will connect each microcontroller to compatible host hardware, allowing compiled machine code to be transferred to the target for execution.

The precise hardware configurations and supported targets will depend on the available equipment and implemented platform support.

---

For more details about **Geyser**, read the [Geyser Explanation](explanation/GeyserExplanation.md).
