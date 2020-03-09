import serial
import time
num = 141301
if __name__=="__main__":
    filename = input("Input filename: ")

    for i in range(1, 6):
        print(f"{i}")
        time.sleep(1)
    print("Play song")
    time.sleep(0.5) # gives a little window to play music
    serial_bytes = []
    
    with serial.Serial(f"/dev/tty.usbmodem{num}") as ser:
        ser.flushInput()
        while True:
            try:
                ser_bytes = ser.readline()
                serial_bytes.append(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
            except KeyboardInterrupt:
                print("Stopped listening")
                break

    with open(f"{filename}.c", 'w') as f:
         file_content = "int delay_arr[] = {" + ', \n'.join(serial_bytes) + "};"
         f.write(file_content)
    with open(f"{filename}.h", 'w') as h:
        h.write("int delay_arr[];") 
              
         
