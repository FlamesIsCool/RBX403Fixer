#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <wininet.h>
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Wininet.lib")

bool DeleteDirectory(const std::wstring& dir) {
    std::wstring temp = dir + L"\\*";
    WIN32_FIND_DATA ffd;
    HANDLE hFind = FindFirstFile(temp.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) return false;

    do {
        std::wstring file = dir + L"\\" + ffd.cFileName;
        if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0) continue;

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            DeleteDirectory(file);
        }
        else {
            DeleteFile(file.c_str());
        }
    } while (FindNextFile(hFind, &ffd) != 0);
    FindClose(hFind);

    return RemoveDirectory(dir.c_str()) != 0;
}

void DeleteRobloxFolder() {
    TCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        std::wstring robloxPath = std::wstring(path) + L"\\Roblox";
        std::wcout << L"Trying to delete folder: " << robloxPath << std::endl;

        if (PathFileExists(robloxPath.c_str())) {
            if (DeleteDirectory(robloxPath)) {
                std::wcout << L"Deleted Roblox folder successfully.\n";
            }
            else {
                std::wcerr << L"Failed to delete Roblox folder.\n";
            }
        }
        else {
            std::wcout << L"Roblox folder does not exist.\n";
        }
    }
}

bool DownloadInstaller(const std::wstring& dest) {
    std::wstring url = L"https://www.roblox.com/download/client?os=win";
    std::wcout << L"Downloading Roblox installer...\n";

    HRESULT hr = URLDownloadToFile(NULL, url.c_str(), dest.c_str(), 0, NULL);
    if (SUCCEEDED(hr)) {
        std::wcout << L"Downloaded installer to: " << dest << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to download installer. HRESULT: " << hr << std::endl;
        return false;
    }
}

void RunInstaller(const std::wstring& filepath) {
    std::wcout << L"Running installer...\n";
    ShellExecute(NULL, L"open", filepath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

int main() {
    std::wcout << L"=== Roblox 403 Fixer ===\n";
    DeleteRobloxFolder();

    std::wstring installerPath = L"RobloxInstaller.exe";
    if (DownloadInstaller(installerPath)) {
        RunInstaller(installerPath);
    }

    std::wcout << L"Done. Press Enter to exit.\n";
    std::cin.get();
    return 0;
}
