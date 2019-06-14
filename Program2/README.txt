// Name: Hudson Dean
// Program Name: Client Server simple file transfer
// Description: Program uses socket programming to connect a client to a server to enter commands. The client will be able to use the following commands: -l, -g
//                -l command will allow client to view directory of server while -g will check if a filename associated with command exists, if it does, it will send contents of file to client.
//
// Course Name: CS372 Intro to Computer Networks
// Last Modified: 5/29/19

1. Place both files (ftserver.c & ftclient.py) in same directory
2. On one flip server, navigate to the directory where the files were placed
3. Compile the server with the following command: gcc ftserver.c -o ftserver
4. Run the server with the following command: ftserver <DESIRED PORT NUMBER>
**Note: Replace <DESIRED PORT NUMBER> with a port number you want the server to run on
If successful, the following message will appear: ftserver listening on port number: <PORT NUMBER>
5. Open a separate flip terminal and navigate to directory where the files were placed (preferably a different flip number (i.e. server on flip1 and client on flip2))
6. Run the client with the following command: python ftclient.py <HOSTNAME> <SERVER PORT NUMBER> <COMMAND> (File Name) <DATA PORT NUMBER>
**Note: <HOSTNAME> must match the host name of the flip instance running from the server!
**Note: <SERVER PORT NUMBER> must match port number specified when running server executable
**Note: <COMMAND> can only be -g or -l.
**Note: IF -g is used, include a (File Name) to retrieve. -l command does not require / use (File Name) option
7. On successful command -l will display contents of current server directory to client
  On successful command -g will copy file from server to client, if file name already exists in client directory, processID will be attached to beginning of file name
8. On unsuccessful command, server should display message to client and halt execution on client
  -**Note: this sometimes result in a bug where ctrl-C SIGINT may need to be called on client
9. After successful command server will continue to accept client commands until server is closed
10. To close server, use ctrl-C on server to end process of server


~~~Thank you for trying it out!~~~
