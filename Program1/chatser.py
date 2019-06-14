# // Name: Hudson Dean
# // Program Name: Simple Chat Server
# // Description: Program uses socket programming to connect a client to a server to intiate chat. Server is written in Python while client is written using C
# //               Program intiates TCP connection between client and server until either host writes "/quit". Hosts alternate turns sending messages with Client going first.
# // Course Name: CS372 Intro to Computer Networks
# // Last Modified: 5/1/19

#Citations: Python Standard Library https://docs.python.org/release/2.6.5/library/internet.html

#python imports for socketing
import socket
import select
import sys
import threading

#instantiate TCP
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#get name of server
hostname = socket.gethostname()
#get IP address of server
hostIP = socket.gethostbyname(hostname)

#If port number was not given, display error message and quit
if len(sys.argv) < 2:
    print "Please enter port number..."
    exit()

################################################################
##                       Start Up                             ##
################################################################
#get port number and bind host name and port
port = int(sys.argv[1])
server.bind((hostname, port))

#Listen on specified port
print "Awaiting connection... SERVER IP: " + hostIP + " Host Name: " + hostname + " ON PORT: " + str(port)
server.listen(10)

#array for holding connections
connections = []

#Function Description: handles connection between client and server. Server awaits for message then is allowed
#                       to send a response.
#Pre-conditions: must be called within cThread target, must have valid TCP connection, CLient must send message first
#Post-conditions: Client message printed to console, Server message sent to client until "/quit"
def handler(conn, addr):
    global connections
    while 1:
        ################################################################
        ##                       Receive Message                      ##
        ################################################################
        #wait for client to send message
        data = conn.recv(1024)

        #if client quits, end connection
        if not data:
            print "Disconnected..."
            connections.remove(conn)
            conn.close()
            break
        elif data == '/quit\r\n':
            print "Disconnected..."
            connections.remove(conn)
            conn.close()
            break
        elif data == '/quit\n':
            print "Disconnected..."
            connections.remove(conn)
            conn.close()
            break

        #print client message without new line
        print(data.strip("\n"))

        ################################################################
        ##                       Send Message                         ##
        ################################################################
        #gather server message
        msg = raw_input()
        #send server message to connections
        for connection in connections:
            connection.send(bytes(msg))

        #if server quits, end connection
        if msg == '/quit\n':
            print "Disconnected..."
            connections.remove(conn)
            conn.close()
            break
        elif msg == '/quit':
            print "Disconnected..."
            connections.remove(conn)
            conn.close()
            break

#continue to accept clients on port
while 1:
    conn, addr = server.accept()
    cThread = threading.Thread(target=handler, args=(conn, addr))
    #cThread.daemon = True
    cThread.start()
    connections.append(conn)
    print(conn)
