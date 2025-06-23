#include <pspkernel.h>
#include <pspdebug.h>
#include <pspiofilemgr.h>
#include <string.h>
#include <stdio.h>

#include "psp_scom.h"

#define SERIAL_PORT "flash0:/pspserial0"  // Update when you know the device path
#define SERIAL_BAUDRATE 9600

#define printf pspDebugScreenPrintf

int serial_fd = -1;

int serial_init() {
    serial_fd = sceIoOpen(SERIAL_PORT, PSP_O_RDWR, 0777);
    if (serial_fd < 0) {
        printf("Serial open failed!\n");
        return -1;
    }

    printf("Serial initialized.\n");
    return 0;
}

void serial_send(const char *cmd) {
    if (serial_fd >= 0) {
        sceIoWrite(serial_fd, cmd, strlen(cmd));
        printf("Sent: %s\n", cmd);
    } else {
        printf("Serial not initialized!\n");
    }
}

void serial_read_line(char *buffer, int maxLen) {
    if (serial_fd < 0) return;

    int idx = 0;
    char c = 0;

    while (idx < maxLen - 1) {
        int readBytes = sceIoRead(serial_fd, &c, 1);
        if (readBytes > 0) {
            if (c == '\n') break;
            buffer[idx++] = c;
        }
    }
    buffer[idx] = '\0';
}

// PSKey commands
void start_read() {
    serial_send("START_READ\n");
}

void stop_read() {
    serial_send("STOP_READ\n");
}

void send_signal() {
    serial_send("SEND\n");
}

void transmit_signal(const char *signalId) {
    char command[16];
    sprintf(command, "TX:%s\n", signalId);
    serial_send(command);
}
