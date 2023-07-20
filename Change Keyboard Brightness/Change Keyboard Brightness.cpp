// This entire program was translated by ChatGPT from the .ps1 program!
#include <iostream>
#include <Windows.h>
#include <string>
#include <regex>

// Function to execute a command and capture its output
std::string executeCommand(const std::wstring& command) {
    std::string output;
    HANDLE hPipeRead, hPipeWrite;
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0)) {
        return output;
    }

    STARTUPINFOW si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    si.hStdError = hPipeWrite;
    si.hStdOutput = hPipeWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if (CreateProcessW(NULL, (LPWSTR)command.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(hPipeWrite);

        const int bufferSize = 4096;
        char buffer[bufferSize];
        DWORD bytesRead;

        while (ReadFile(hPipeRead, buffer, bufferSize - 1, &bytesRead, NULL) != 0 && bytesRead != 0) {
            buffer[bytesRead] = '\0';
            output += buffer;
        }

        CloseHandle(hPipeRead);

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return output;
}

// Function to extract the number from the string output
int extractNumber(const std::string& input) {
    std::regex regex("\\d+");
    std::smatch match;
    if (std::regex_search(input, match, regex)) {
        return std::stoi(match.str());
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [changePercent]" << std::endl;
        return 1;
    }

    std::wstring ectoolPath = L"C:\\Program Files\\crosec\\ectool";

    // Get current keyboard brightness
    std::wstring getBrightnessCommand = ectoolPath + L" pwmgetkblight";
    std::string output = executeCommand(getBrightnessCommand);
    std::cout << output;

    // Get the number from the string output
    int currentBacklightPercent = extractNumber(output);
    if (currentBacklightPercent == -1) {
        std::cerr << "Failed to extract the backlight percent from the output." << std::endl;
        return 1;
    }

    // Parse the change percentage from the input
    int changePercent = std::stoi(argv[1]);
    int newBacklightPercent = currentBacklightPercent + changePercent;
    if (newBacklightPercent >= 100) {
        newBacklightPercent = 100;
    }
    if (newBacklightPercent <= 0) {
        newBacklightPercent = 0;
    }

    std::cout << "Current keyboard backlight percent: " << currentBacklightPercent << std::endl;
    std::cout << "New keyboard backlight percent: " << newBacklightPercent << std::endl;

    // Set the new keyboard brightness
    std::wstring setBrightnessCommand = ectoolPath + L" pwmsetkblight " + std::to_wstring(newBacklightPercent);
    std::string setOutput = executeCommand(setBrightnessCommand);
    std::cout << setOutput;

    return 0;
}
