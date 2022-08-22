import serial
import time

ser = serial.Serial(
        # Serial Port to read the data from
        port='COM6',
 
        #Rate at which the information is shared to the communication channel
        baudrate = 9600,
   
        #Applying Parity Checking (none in this case)
        parity=serial.PARITY_NONE,
 
       # Pattern of Bits to be read
        stopbits=serial.STOPBITS_ONE,
     
        # Total number of bits to be read
        bytesize=serial.EIGHTBITS,
 
        # Number of serial commands to accept before timing out
        timeout=0.25
)

def send_data(data):
        data=data.encode()
        print(data)
        ser.write(data)


# for i in range(1,9,1):
#         send_data("s"+str(i)+"!")
#         time.sleep(0.1)
#         send_data("r1!")
#         time.sleep(3)

while True:
        #inp=input("Girdi: ")
        #inp=inp+"!"
        send_data("k")
        time.sleep(0.01)


