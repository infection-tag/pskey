#include <pspkernel.h>
#include <pspdebug.h>
#include <pspiofilemgr.h>
#include <pspthreadman.h>
#include <string.h>
#include <stdio.h>

#include "psp_scom.h"

#define SERIAL_PORT "flash0:/pspserial0" /* TODO: Update when you know the device path */
#define SERIAL_BAUDRATE 9600

#define printf pspDebugScreenPrintf

int serial_fd = -1;
int serial_thread_id = -1;

/* Serial initialization */
int serial_init() {
    serial_fd = sceIoOpen(SERIAL_PORT, PSP_O_RDWR, 0777);
    if (serial_fd < 0) {
        printf("Serial open failed!\n");
        return -1;
    }

    printf("Serial initialized.\n");
    return 0;
}

/* Send command to dongle */
void serial_send(const char *cmd) {
    if (serial_fd >= 0) {
        sceIoWrite(serial_fd, cmd, strlen(cmd));
        printf("Sent: %s\n", cmd);
    } else {
        printf("Serial not initialized!\n");
    }
}

/* Read one line from dongle */
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

/* Serial listening thread */
int serial_listener_thread(SceSize args, void *argp) {
    char buffer[128];

    while (1) {
        serial_read_line(buffer, sizeof(buffer));
        if (strlen(buffer) > 0) {
            printf("Dongle: %s\n", buffer);
        }

        sceKernelDelayThread(1000); /* 1 ms delay to prevent CPU lock */
    }

    return 0;
}

/* Start the serial listener thread */
int start_serial_listener() {
    serial_thread_id = sceKernelCreateThread("SerialListener", serial_listener_thread, 0x11, 0x1000, 0, NULL);
    if (serial_thread_id >= 0) {
        sceKernelStartThread(serial_thread_id, 0, NULL);
        printf("Serial listener started.\n");
        return 0;
    } else {
        printf("Failed to start serial listener.\n");
        return -1;
    }
}

/* PSKey serial commands */
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
