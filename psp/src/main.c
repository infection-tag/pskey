#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>

PSP_MODULE_INFO("PSKey", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#define printf pspDebugScreenPrintf

void sendSerialCommand(const char *command) {
    printf("Sent: %s\n", command);
}

int main() {
    pspDebugScreenInit();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    printf("PSKey Ready\n");

    /* Initialize serial and start listener */
    if (serial_init() == 0) {
        start_serial_listener();
    }

    while (1) {
        SceCtrlData pad;
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_CROSS) {
            start_read();
        } else if (pad.Buttons & PSP_CTRL_CIRCLE) {
            stop_read();
        } else if (pad.Buttons & PSP_CTRL_TRIANGLE) {
            send_signal();
        } else if (pad.Buttons & PSP_CTRL_SQUARE) {
            transmit_signal("01");
        } else if (pad.Buttons & PSP_CTRL_START) {
            printf("Exiting PSKey...\n");
            sceKernelDelayThread(1000000);
            sceKernelExitGame();
        }

        sceKernelDelayThread(10000); /* 10 ms delay to debounce buttons */
    }

    return 0;
}