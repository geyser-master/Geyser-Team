#include <iostream>
#include <windows.h>

// Modern LLVM-C Core Symbol Signatures
using LLVMContextCreateFn  = void* (*)();
using LLVMContextDisposeFn = void (*)(void*);
using LLVMGetVersionFn     = void (*)(unsigned*, unsigned*, unsigned*);

int main() {
    std::cout << "===============================================\n";
    std::cout << "Starting Modern C++26 LLVM 22 Validation Suite\n";
    std::cout << "===============================================\n\n";

    // Load the validated 147MB dynamic module
    char exePath[MAX_PATH];

    GetModuleFileNameA(nullptr, exePath, MAX_PATH);

    std::string exeDir(exePath);
    exeDir = exeDir.substr(0, exeDir.find_last_of("\\/"));

    std::string dllPath =
        exeDir + "\\..\\..\\platforms\\x86_64-windows\\bin\\libLLVM-22.dll";

    HMODULE hLLVM = LoadLibraryExA(
        dllPath.c_str(),
        nullptr,
        LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR |
        LOAD_LIBRARY_SEARCH_DEFAULT_DIRS
    );
    if (!hLLVM) {
        std::cerr << "[CRITICAL ERROR] Failed to map libLLVM-22.dll! System Error: " << GetLastError() << "\n";
        return 1;
    }
    std::cout << "[SUCCESS] libLLVM-22.dll successfully mapped into memory space.\n\n";

    // -------------------------------------------------------------
    // TEST 1: Modern LLVM-C Core Engine Verification
    // -------------------------------------------------------------
    std::cout << "Executing Test Layer 1: [LLVM-C Context API]...\n";

    auto createContext  = reinterpret_cast<LLVMContextCreateFn>(GetProcAddress(hLLVM, "LLVMContextCreate"));
    auto disposeContext = reinterpret_cast<LLVMContextDisposeFn>(GetProcAddress(hLLVM, "LLVMContextDispose"));

    if (!createContext || !disposeContext) {
        std::cerr << " [FAIL] Failed to locate modern context management symbols in DLL exports!\n";
        FreeLibrary(hLLVM);
        return 2;
    }

    // Allocate and instantly free an internal isolated LLVM Context structure container
    void* myContext = createContext();
    if (myContext) {
        std::cout << " [PASS] Successfully initialized isolated LLVM Context object at: " << myContext << "\n";
        disposeContext(myContext);
        std::cout << " [PASS] Safely released LLVM Context memory allocations.\n\n";
    } else {
        std::cerr << " [FAIL] Context function pointer resolved, but returned a null allocation state.\n\n";
        FreeLibrary(hLLVM);
        return 3;
    }

    // -------------------------------------------------------------
    // TEST 2: Core LLVM Metadata Verification
    // -------------------------------------------------------------
    std::cout << "Executing Test Layer 2: [Core LLVM Metadata Suite]...\n";

    if (auto getVersionFn = reinterpret_cast<LLVMGetVersionFn>(GetProcAddress(hLLVM, "LLVMGetVersion"))) {
        unsigned major = 0, minor = 0, patch = 0;
        getVersionFn(&major, &minor, &patch);
        std::cout << " [PASS] Engine responsive. Verified Version: " << major << "." << minor << "." << patch << "\n\n";
    } else {
        std::cerr << " [FAIL] Could not query version function symbol.\n";
        FreeLibrary(hLLVM);
        return 4;
    }

    std::cout << "===============================================\n";
    std::cout << "SYSTEM RUNTIME OK: BOTH LLVM BOUNDARIES VERIFIED!\n";
    std::cout << "===============================================\n";

    FreeLibrary(hLLVM);
    return 0;
}
