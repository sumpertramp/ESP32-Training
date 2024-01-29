"""
===============================================================================
File: TCP_Server_Python.py
Date: [29.01.2024]

Description:
   The code creates a TCP server operating on a specified IP address and port 
number. The server listens for incoming connections and accepts each connection. 
Once a connection is established, the server continuously receives data and prints 
it to the screen. If the client closes the connection, the server prints this 
information to the screen and continues to wait for the next connection.

NOT! Make sure you enter the correct server IP.

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
  
===============================================================================
"""


import socket

#Server IP adress and port number
HOST = "192.168.1.118"
PORT = 80

# Create socket object using IP
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    # Bind the socket to the specified IP adress and port
    s.bind((HOST, PORT))

    # Listen for incoming connections
    s.listen()
    
    # Print a message indicating that the server is listening
    print(f"Server listening on {HOST}:{PORT}")
    
    # infinite loop to handle incoming connections
    while True:

        #Accept a connection from a client
        conn, addr = s.accept()

        with conn:
            print(f"Connected by {addr}")

            while True:
                data = conn.recv(1024)
                if not data:
                    break
                print(f"Received: {data.decode('utf-8')}")
            print("Connection closed by client")