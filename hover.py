import threading
from time import sleep
import serial
import sys

if len(sys.argv) == 1:
    print(" ")
    print("Hoverboard Mainboard Controller Python Edition") 
    print("CT Asia Robotics CO., Ltd. 2018") 
    print("Based on Segway_wantai2_with_serial_command_v3.ino - Program for control BLDC Motor created by Muchammad Sobrun Ibnu Atfal.")
    print(" ")
    print("Usage: python hover.py serialdev1 serialdev2")
    print("Type ? or --help for help")
    exit()
else:
    if sys.argv[1] == ("--help" or "?"):
        print(" ")
        print("To use a wheel, first enable the wheel by sending thw wheel ID + EN, e.g. 1EN") 
	print("Set the speed limit by sending the wheel ID + SP + desired speed limit, e.g. 1SP300")
        print("Next, set the desired acceleration by sending the wheel ID + AC + the desired acceleration coefficient e.g. 1AC10")
        print("Note that the acceleration is number of speed increments per second * 20 e.g. if acceleration is 5, then speed will change by 100/s")
        print("Finally to set the wheel speed send the wheel ID + V + desired speed, e.g. 1V250")
        print(" " )
        print("Type 'help??' for more help")
        print(" ")
    	exit()
    elif sys.argv[1] == "help??":
	print("To disable a wheel use DI, e.g. 1DI")
        print("To get set speed limit use GSP, e.g. 1GSP")
        print("To get last requested speed use LST, e.g. 1LST")
        print("To get acceleration coefficient use GAC, e.g. 1GAC")
        print("For an emergency stop type STOP.")
        print(" ")
        print("To control the mainboard directly, use 19200 baud and send two 8 bit unsigned integers 20 times per second")
        print("from range 1000, -1000 to control each wheel.")
        print("Board firmware a modified version of the firmware found at: https://github.com/NiklasFauth/hoverboard-firmware-hack")
	print(" ")
	exit()
    else:
	dev1 = sys.argv[1]
#    dev2 = sys.argv[2]
    	board1 = serial.Serial(dev1, 19200)
#    with serial.Serial(dev2, 19200, timeout=10) as board2:
global reply
global sp
global ac
global spa
global speedMotor
halt = 0

sp = [0, 0, 0, 0] # Array containing sent speeds for each motor.
ac = [0, 0, 0, 0] # Array containing acceleration coefficients.
max = [0, 0, 0, 0] # Array containing speed limits.
spa = [0, 0, 0, 0] # Array containing last sent speeds.
speedMotor = [0, 0, 0, 0] # Array containing target speeds
reply = [0, 0, 0, 0] # Ensures that replies are not repeated, 0 = 0, 1 = 1
speedM = 0 # Integer with received target modifier.
stringComplete = 0

def UserInput():
  global stringComplete
  global halt
  while halt == 0:
#-------------------------------------------------GET USER INPUT
    print(" ")
    inputString = raw_input("Type exit() to exit. Command: ")
#----------------------------------------------QUIT
    if inputString == "exit()" or inputString == "stop":
        halt = 1
        exit()
#-----------------------------------------------INPUT PROCESSING
    if inputString != "" and int(inputString[:1]) >= 1 and int(inputString[:1]) <= 4:
        print("OK")
        print("ID: " + inputString[:1])
        ID_int = int(inputString[:1])
        stringComplete = 1
    else:
	print(" ")
#-----------------------------------------------SPEED MANAGEMENT
    if stringComplete == 1 and inputString[1:2] == "V":
        speedM = int(inputString[2:])
	print("Speed: " + inputString[2:])
    	if sp[ID_int - 1] == speedM:
	    print("The requested speed equals the current speed")
	    reply[ID_int - 1] = 1
	elif abs(speedM) <= max[ID_int - 1]:
	    reply[ID_int - 1] = 0
	    speedMotor[ID_int - 1] = speedM
	else:
	    print("The requested speed exceeds the speed limit!")
#----------------------------------------------SPEED LIMIT PROCESSING
    elif stringComplete == 1 and inputString[1:3] == "SP":
	speedM = int(inputString[3:])
	max[ID_int - 1] = speedM
	print("Set Max Speed: " + inputString[3:])
	print(" ")
#----------------------------------------------ENABLE
    elif stringComplete == 1 and inputString[1:3] == "EN":
	print("ENABLE")
	print(" ")
#----------------------------------------------DISABLE
    elif stringComplete == 1 and inputString[1:3] == "DI":
        print("DISABLE")
	print(" ")
 	sp[ID_int - 1] = 0
	speedMotor[ID_int - 1] = 0
	ac[ID_int - 1] = 0
	max[ID_int - 1] = 0
#---------------------------------------------SET AC COEFFICIENT
    elif stringComplete == 1 and inputString[1:3] == "AC":
	speedM = int(inputString[3:])
	print("Acceleration set to: " + inputString[3:])
	print(" ")
	ac[ID_int - 1] = speedM
#---------------------------------------------GET SPEED LIMIT
    elif stringComplete == 1 and inputString[1:5] == "GLMT":
	print("Max speed on wheel ID " + ID_int + ": " + max[ID_int - 1])
	print(" ")
#---------------------------------------------GET LAST REQUESTED SPEED
    elif stringComplete == 1 and inputString[1:3] == "GSP":
	print("Last speed on wheel ID " + ID_int + ": " + speedMotor[ID_int - 1])
	print(" ")
#---------------------------------------------GET ACCELERATION
    elif stringComplete == 1 and inputString[1:3] == "GAC":
	print("Acceleration on wheel ID " + ID_int + ": " + ac[ID_int])
	print(" ")
#---------------------------------------------GET ACTUAL SPEED
    elif stringComplete == 1 and inputString[1:2] == "GS":
	print("Speed on wheel ID " + ID_int + ": " + sp[ID_int - 1])
	print(" ")
#----------------------------------------------HELP
    elif stringComplete == 1 and (inputString == "?" or inputString == "--help"):
	print(" ")
  	print("To use a wheel, first set the speed limit by sending the wheel ID + SP + desired speed limit, e.g. 1SP300")
  	print("Next, set the desired acceleration by sending the wheel ID + AC + the desired acceleration coefficient e.g. 1AC10")
  	print("Note that the acceleration is number of speed increments per second * 20 e.g. if acceleration is 5, then speed will change by 100/s")
  	print("Finally to set the wheel speed send the wheel ID + V + desired speed, e.g. 1V250")
  	print(" " )
  	print("Type 'more help' for more help")
  	print(" ")
    elif stringComplete == 1 and inputString == "more help":
  	print("To get set speed limit use GSP, e.g. 1GSP")
  	print("To get last requested speed use LST, e.g. 1LST")
	print("To get current speed use GS, e.g. 1GS")
	print("To get set acceleration coefficient use GAC, e.g. 1GAC")
  	print("To disable a wheel use DI, e.g. 1DI")
	print("For an emergency stop type STOP.")
	print(" ")
  	print("To control the mainboard directly, use 19200 baud and send two 8 bit unsigned integers 20 times per second")
  	print("from range 1000, -1000 to control each wheel.")
	print("Board firmware a modified version of the firmware found at: https://github.com/NiklasFauth/hoverboard-firmware-hack")
#-----------------------------------------------CLEAR INPUT
    if stringComplete == 1:
	stringComplete = 0
	inputString = ""

#----------------------------------------------SEND SPEEDS AND ACCELERATION
def sendSpeed():
  while halt == 0:
#----------------------------------------------AC & DC
    for i in range(4):
	if speedMotor[i] > sp[i]:
	    if speedMotor[i] >= 0 and reply[i] == 0:
		print("Positive Acceleration")
		reply[i] = 1
	    if speedMotor[i] <= 0 and reply[i] ==0:
		print("Negative Deacceleration")
		reply[i] = 1
	    sp[i] = sp[i] + ac[i]
	if speedMotor[i] < sp[i]:
	    if speedMotor[i] > 0 and reply[i] == 0:
		print("Positive Deacceleration")
		reply[i] = 1
 	    if speedMotor[i] < 0 and reply[i] == 0:
		print("Negative Acceleration")
		reply[i] = 1
	    sp[i] = sp[i] - ac[i]
#--------------------------------------------ENSURE FULL STOP
    for i in range(4):
	if speedMotor[i] == 0 and sp[i] == spa[i]:
	    sp[i] = 0
#--------------------------------------------SET LAST SPEED
    for i in range(4):
	spa[i] = sp[i] 
#-------------------------------------------SEND SERIAL
    board1.write('\x5C')
    board1.write('\xDA')
    sleep(.1)
#-------------------------------------------DECLARE AND START THREADS
getinput = threading.Thread(target=UserInput)
send = threading.Thread(target=sendSpeed)
getinput.start()
send.start()
getinput.join()
send.join()
