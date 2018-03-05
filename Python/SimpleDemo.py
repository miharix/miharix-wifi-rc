from websocket import create_connection
import pygame

Servo1_degree=90;
Servo2_degree=90;
Servo3_degree=90;
Servo4_degree=90;
Servo5_degree=90;
Servo6_degree=90;

ws = create_connection("ws://192.168.4.1:81/websocket")

while Servo1_degree!=180:
    Servo1_degree=int(input("Servo1:"))
    Servo2_degree=int(input("Servo2:"))
    Servo3_degree=int(input("Servo3:"))
    Servo4_degree=int(input("Servo4:"))
    Servo5_degree=int(input("Servo5:"))
    Servo6_degree=int(input("Servo6:"))
    S1='{:02x}'.format(Servo1_degree)
    S2='{:02x}'.format(Servo2_degree)
    S3='{:02x}'.format(Servo3_degree)
    S4='{:02x}'.format(Servo4_degree)
    S5='{:02x}'.format(Servo5_degree)
    S6='{:02x}'.format(Servo6_degree)

    S1_6 = 'S'+S1+S2+S3+S4+S5+S6+'\n'
    print(S1_6)
    
    ws.send(S1_6)

ws.close()
print("done")
