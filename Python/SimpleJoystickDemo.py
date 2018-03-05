from websocket import create_connection
import pygame
import time

Servo1_degree=90;
Servo2_degree=90;
Servo3_degree=90;
Servo4_degree=90;
Servo5_degree=90;
Servo6_degree=90;

pygame.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()
        


ws = create_connection("ws://192.168.4.1:81/websocket")

while True:
    pygame.event.pump()
    Servo1_degree=int((joystick.get_axis(0)+1)*90);
    Servo2_degree=int((joystick.get_axis(1)+1)*90);
    Servo3_degree=int((joystick.get_axis(2)+1)*90);
    Servo4_degree=int((joystick.get_axis(3)+1)*90);
    Servo5_degree=int((joystick.get_axis(4)+1)*90);
    #Servo6_degree=int(joystick.get_axis(5)*100+90);

    if(joystick.get_hat(0)[0]==1):
        Servo6_degree=Servo6_degree+1
    if(joystick.get_hat(0)[0]==-1):
        Servo6_degree=Servo6_degree-1    
    if(joystick.get_button(0)):
        Servo6_degree=90
    
 


   # print(joystick.get_hat(0)[0])
    #print(Servo6_degree)
    S1='{:02x}'.format(Servo1_degree)
    S2='{:02x}'.format(Servo2_degree)
    S3='{:02x}'.format(Servo3_degree)
    S4='{:02x}'.format(Servo4_degree)
    S5='{:02x}'.format(Servo5_degree)
    S6='{:02x}'.format(Servo6_degree)

    S1_6 = 'S'+S1+S2+S3+S4+S5+S6+'\n'
    #print(S1_6)
    time.sleep(0.01)
    ws.send(S1_6)

ws.close()
print("done")

