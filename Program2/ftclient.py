# // Name: Hudson Dean
# // Program Name: Client Server simple file transfer
# // Description: Program uses socket programming to connect a client to a server to enter commands. The client will be able to use the following commands: -l, -g
# //                -l command will allow client to view directory of server while -g will check if a filename associated with command exists, if it does, it will send contents of file to client.
# //
# // Course Name: CS372 Intro to Computer Networks
# // Last Modified: 5/29/19
import socket
import select
import sys
import os

#error if not enough args given in command
if len(sys.argv) < 5:
    print "Correct usage: script, IP address, port number, command, dataPort"
    print "If command is -g, include filename after command"
    exit()

#Set up data TCP connection
dataSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#get name of client
hostname = socket.gethostname()
#get IP address of client
hostIP = socket.gethostbyname(hostname)
#last argument of command is data port
dataPort = int(sys.argv[len(sys.argv)-1])
#create data socket
dataSocket.bind((hostname, dataPort))

#create client socket to send command
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#get name of server from command line
IP_address = socket.gethostbyname(sys.argv[1])
#get port of server from command line
Port = int(sys.argv[2])
#connect to server
server.connect((IP_address, Port))
#send command to server
strArg = ""
for args in sys.argv:
    strArg = strArg + args + " "
server.send(strArg)

#listen for server connection to data port
dataSocket.listen(10)
#accept connection from server
conn, addr = dataSocket.accept()
#recieve first message from server
data = conn.recv(1024)
#if server sends data that indicates an error, print the error message
if(len(data) == 20):
    print(data)
#if server sends data that indicates an error, print the error message
elif(len(data) == 50):
    print(data)
#if not error data, continue recieving data
else:
    #check command for what command flag used
    if(str(sys.argv[3]) == "-g"):
        #check if file name already exists in current directory
        exists = os.path.isfile(str(sys.argv[4]))
        #if filename exists, attach processID to front of file namec
        if(exists):
            fileName = str(os.getpid()) + str(sys.argv[4])
            file = open(fileName, "w+")
        #if file name does not exist, use that filename
        else:
            file = open(str(sys.argv[4]), "w+")
        #write server data to file until no more data is sent
        while 1:
            file.write(data)
            data = conn.recv(1024)
            if not data:
                break
        file.close()
        print("File transfer complete!")

    elif(str(sys.argv[3]) == "-l"):
        #recieve directory data from server until no more data
        while 1:
            print(data)
            data = conn.recv(1024)
            if not data:
                break
#close open sockets
dataSocket.close()
server.close()
