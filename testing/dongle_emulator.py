import serial # pip install pyserial
import time

# Replace with the serial port your PSP will connect to (use loopback or USB-Serial adapter)
SERIAL_PORT = 'COM3'    # Windows example: COM3
# SERIAL_PORT = '/dev/ttyUSB0'  # Linux example

BAUDRATE = 9600

def handle_command(command):
    command = command.strip()
    print(f"Received: {command}")

    if command == "START_READ":
        return "OK\n"
    elif command == "STOP_READ":
        return "CAPTURE:500,1500,500,500\n"
    elif command == "SEND":
        return "TX_OK\n"
    elif command.startswith("TX:"):
        return "TX_OK\n"
    else:
        return "ERROR\n"

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
        print(f"Listening on {SERIAL_PORT} at {BAUDRATE} baud...")

        while True:
            if ser.in_waiting:
                line = ser.readline().decode('utf-8').strip()
                response = handle_command(line)
                time.sleep(0.1)  # Simulate processing delay
                ser.write(response.encode('utf-8'))
                print(f"Sent: {response.strip()}")

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("Exiting emulator.")

if __name__ == "__main__":
    main()
