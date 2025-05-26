#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <string>

// Access rights for injection
#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

// EnumWindows callback
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD dwProcessId;
    wchar_t windowTitle[256];

    // Skip invisible windows
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }

    // Get the window title
    if (GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(wchar_t)) == 0) {
        return TRUE;
    }

    // Get the process ID
    GetWindowThreadProcessId(hwnd, &dwProcessId);

    // Print the PID and the window title
    std::wcout << L"PID: " << dwProcessId << L"\tTitle: " << windowTitle << std::endl;
    return TRUE;
}

// Injects the DLL into the target process
BOOL InjectDLL(DWORD ProcessID) {
    const char* DLL_PATH = "C:\\S1\\Schedule I Cheat.dll";

    HANDLE hProc = OpenProcess(CREATE_THREAD_ACCESS, FALSE, ProcessID);
    if (!hProc) {
        std::cerr << "[-] Failed to open process. Error: " << GetLastError() << std::endl;
        return FALSE;
    }

    // Get LoadLibraryA address
    LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibrary) {
        std::cerr << "[-] Failed to get LoadLibraryA address." << std::endl;
        CloseHandle(hProc);
        return FALSE;
    }

    // Allocate memory in target process
    LPVOID pRemoteString = VirtualAllocEx(hProc, NULL, strlen(DLL_PATH) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemoteString) {
        std::cerr << "[-] Failed to allocate memory in target process. Error: " << GetLastError() << std::endl;
        CloseHandle(hProc);
        return FALSE;
    }

    // Write DLL path to target process
    if (!WriteProcessMemory(hProc, pRemoteString, DLL_PATH, strlen(DLL_PATH) + 1, NULL)) {
        std::cerr << "[-] Failed to write DLL path to target process. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProc, pRemoteString, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return FALSE;
    }

    // Create remote thread to load the DLL
    HANDLE hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteString, 0, NULL);
    if (!hThread) {
        std::cerr << "[-] Failed to create remote thread. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProc, pRemoteString, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return FALSE;
    }

    std::cout << "[+] DLL injected successfully!" << std::endl;

    // Wait for the remote thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Clean up
    VirtualFreeEx(hProc, pRemoteString, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProc);

    return TRUE;
}

DWORD IsGameOpen() {

    DWORD processId = 0;
    std::wstring processName = L"Schedule I.exe";

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open game" << std::endl;
        return processId;
    }

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                processId = pe.th32ProcessID;
                return processId;
            }
        } while (Process32NextW(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return 0;
}

int main() {
    /*std::wcout << L"Listing visible windows:\n";
    EnumWindows(EnumWindowsProc, NULL);*/

    DWORD procId = 0;
    //while (procId == 0)
    //    Sleep(100); // wait for game to open
    //    procId = IsGameOpen();

    procId = IsGameOpen();

    if (procId == 0) {
        std::cout << "Game is not open" << std::endl;
        Sleep(100);
        return 0;
    }

    if (!InjectDLL(procId)) {
        std::cerr << "[-] DLL injection failed." << std::endl;
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
