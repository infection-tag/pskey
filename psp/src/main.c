#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include "../lib/psp_scom.h"

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

    while (1) {
        SceCtrlData pad;
        sceCtrlReadBufferPositive(&pad, 1);

		/* Start reading inputs from the PSP */

        if (pad.Buttons & PSP_CTRL_CROSS) {
            sendSerialCommand("START_READ\n");
        } else if (pad.Buttons & PSP_CTRL_CIRCLE) {
            sendSerialCommand("STOP_READ\n");
        } else if (pad.Buttons & PSP_CTRL_TRIANGLE) {
            sendSerialCommand("SAVE\n");
        } else if (pad.Buttons & PSP_CTRL_SQUARE) {
            sendSerialCommand("TX:01\n");
        } else if (pad.Buttons & PSP_CTRL_START) {
            printf("Exiting PSKey...\n");
            sceKernelDelayThread(1000000);
            sceKernelExitGame();
        }

        sceKernelDelayThread(10000); /* Debounce */
    }

    return 0;
}
