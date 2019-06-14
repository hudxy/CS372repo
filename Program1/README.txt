# // Name: Hudson Dean
# // Program Name: Simple Chat Server
# // Description: Program uses socket programming to connect a client to a server to initiate chat. Server is written in Python while client is written using C
# //               Program initiates TCP connection between client and server until either host writes "/quit". Hosts alternate turns sending messages with Client going first.
# // Course Name: CS372 Intro to Computer Networks
# // Last Modified: 5/1/19

1. Place both files (chatcli.c & chatser.py) in same directory
2. On one flip server, navigate to the directory where the files were placed
3. Start the server with the following command: python chatser.py <DESIRED PORT NUMBER>
**Note: Replace <DESIRED PORT NUMBER> with a port number you want the server to run on
If successful, the following message will appear: Awaiting connection... SERVER IP: <SERVER IP> Host Name: <HOSTNAME> ON PORT: <PORT NUMBER>
4. Open a separate flip terminal and navigate to directory where the files were placed (preferably a different flip number (i.e. server on flip1 and client on flip2))
5. Compile chat client with the command: make
**Note: using makefile, the executable "chatcli" should be available... If not compile using the command: gcc chatcli.c -o chatcli
6. Run chat client with the command: ./chatcli <HOSTNAME> <PORT NUMBER>
**Note: <HOSTNAME> must match the host name of the flip instance running from the server!
**Note: <PORT NUMBER> must match the port number that was used to initiate the server!
If successful connection, the following message will appear on server console: <socket._socketobject object at <HEXADECIMAL>>
7. Start Chat by typing on client terminal FIRST!
**Note Client must initiate chat by sending message FIRST, otherwise inputs from server will be buffered
8. Response must be made from the server before client can send another message, otherwise client inputs will be buffered
9. Messages MUST be alternating starting with client
10. Continue alternating messages until one host enters: /quit
11. The other host must respond with another message to fully Disconnect
12. Once "Disconnected..." prompt appears on server, client can reconnect
13. To close chat, disconnect client from server then ctrl-C on server to end process of server


~~~Thank you for trying it out!~~~
