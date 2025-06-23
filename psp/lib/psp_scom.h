#ifndef PSP_SCOM_H
#define PSP_SCOM_H

/* Initializes the serial port */
int serial_init();

/* Sends a raw command to the dongle */
void serial_send(const char *cmd);

/* Reads a line from the dongle (blocking read) */
void serial_read_line(char *buffer, int maxLen);

/* PSKey serial command functions */
void start_read();
void stop_read();
void send_signal();
void transmit_signal(const char *signalId);

#endif
