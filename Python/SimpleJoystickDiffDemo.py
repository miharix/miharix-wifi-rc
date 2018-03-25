from websocket import create_connection
import pygame
import time
import math

def steering(x, y):
    #https://electronics.stackexchange.com/questions/19669/algorithm-for-mixing-2-axis-analog-input-to-control-a-differential-motor-drive
    # convert to polar
    r = math.hypot(x, y)
    t = math.atan2(y, x)

    # rotate by 45 degrees
    t += math.pi / 4

    # back to cartesian
    left = r * math.cos(t)
    right = r * math.sin(t)

    # rescale the new coords
    left = left * math.sqrt(2)
    right = right * math.sqrt(2)

    # clamp to -1/+1
    left = max(-1, min(left, 1))
    right = max(-1, min(right, 1))

    return left, right

def steering90(x, y):
    #https://electronics.stackexchange.com/questions/19669/algorithm-for-mixing-2-axis-analog-input-to-control-a-differential-motor-drive
    x=x/90-1
    y=y/90-1
    
    # convert to polar   
    r = math.hypot(x, y)
    t = math.atan2(y, x)

    # rotate by 45 degrees
    t += math.pi / 4

    # back to cartesian
    left = r * math.cos(t)
    right = r * math.sin(t)

    # rescale the new coords
    left = left * math.sqrt(2)
    right = right * math.sqrt(2)

    # clamp to -1/+1
    left = max(-1, min(left, 1))
    right = max(-1, min(right, 1))

    left=(left+1)*90
    right=(right+1)*90
    return left, right


debug=True
debug=False

Servo1_degree=90;
Servo2_degree=90;
Input1_degree=90;
Input2_degree=90;
J1=0;
J2=0;
Calc=[0,0];

pygame.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

if not debug:
    ws = create_connection("ws://192.168.4.1:81/websocket")

while not joystick.get_hat(0)[1]:
    pygame.event.pump()
    J1=joystick.get_axis(0);
    J2=joystick.get_axis(1)*-1;

   # print('J: {0} {1}'.format(J1, J2))

    Input1_degree=(J1+1)*90
    Input2_degree=(J2+1)*90
    
    #Calc=steering(J2,J1)
    #Servo1_degree=(Calc[1]+1)*90
    #Servo2_degree=(Calc[0]+1)*90

    Calc=steering90(Input2_degree,Input1_degree)
    Servo1_degree=Calc[1]
    Servo2_degree=Calc[0]
    #print('\nJ: {0} {1}\nC: {2} {3}\nS: {4} {5}'.format(J1, J2,Calc[0],Calc[1],Servo1_degree, Servo2_degree), end='')
    #print('\nJ: {0} {1}\nI: {2} {3}\nS: {4} {5}'.format(J1, J2,Input1_degree, Input2_degree,Servo1_degree, Servo2_degree), end='')

    S1='{:02x}'.format(int(Servo1_degree))
    S2='{:02x}'.format(int(Servo2_degree))
    S1_6 = 'S'+S1+S2+'00000000'
    #print('\n'+S1_6, end='')
    #print('\nJ: {0} {1}\nC: {2} {3}\nS: {4} {5}\n{6}'.format(J1, J2,Calc[0],Calc[1],Servo1_degree, Servo2_degree,S1_6), end='')
    S1_6=S1_6+'\n'
    
    
    time.sleep(0.05)
    if not debug:
        ws.send(S1_6)

if not debug:
    ws.close()

