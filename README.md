# ⚡ The Geyser Programming Language (.gy)

**Geyser** is a strict, explicit, and blistering fast systems programming language engineered from the ground up for real-time engines, robotics, and high-performance simulators. Linked natively with an LLVM-22 backend, Geyser drops 40 years of legacy C++ backward-compatibility baggage to achieve raw hardware execution speed with zero runtime boilerplate.

---

## 🎯 Why Geyser? (The Catch)

Modern software shouldn't suffer from legacy overhead or unpredictable background collector lag. Geyser captures optimal hardware performance using three uncompromising core architectural pillars:

### ❌ The "Yeet the GC" Memory Strategy
*   **Zero Background Lag:** The Garbage Collector is permanently banished to eliminate unpredictable stuttering frames during real-time loops.
*   **Automatic Scope Cleanups:** Local data sits flat inside memory stack slots. The exact millisecond your execution block exits and hits a closing curly brace `}`, the compiler automatically inserts instructions to destroy those memory slots.

### 🧠 Hardware Cache Separation
*   **Cached Instruction Loops:** Loop execution blocks and branching mechanisms are pinned directly inside the native CPU Instruction Cache (I-Cache) to repeat at peak clock cycles.
*   **Uncached Synchronized Data:** To permanently resolve multi-threaded synchronization bugs, standard variable reads and writes completely bypass the local CPU Data Cache (D-Cache). Operations interact directly with physical memory slots for 100% real-time data transparency across all cores.

### 🔍 Absolute Explicitness
*   **No Compiler Guessing Games:** Every data type, scope boundary, statement terminator, and logical evaluation must be completely explicit. If the compiler encounters structural ambiguity, it halts instantly at build time before unsafe code can ever reach physical hardware.
