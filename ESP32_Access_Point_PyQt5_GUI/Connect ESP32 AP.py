"""
===============================================================================
File: Connect ESP32 AP.py
Date: [10.01.2024]

Description:
  This Python script defines a simple PyQt5 GUI application for connecting to 
  and disconnecting from an ESP32 Access Point using sockets. The application 
  consists of a main window with "Connect" and "Disconnect" buttons, as well 
  as a label indicating the connection status. The user can click the "Connect" 
  button to establish a connection with the ESP32 Access Point, and the 
  "Disconnect" button to terminate the connection. Connection status updates 
  are displayed in the UI. The application handles socket-related errors 
  during connection and disconnection attempts.

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com

===============================================================================
"""


import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QLabel, QVBoxLayout
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QIcon
import socket

class WiFiClientApp(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        # Set up the main user interface elements
        self.setWindowTitle('Connect ESP32 Access Point')
        self.setGeometry(100, 100, 200, 100)
        self.setWindowIcon(QIcon('icon.png'))

        self.label_connect = QLabel('Not Connected', self)
        self.btnConnect = QPushButton('Connect', self)
        self.btnConnect.clicked.connect(self.connectToESP32)

        self.btnDisconnect = QPushButton('Disconnect', self)
        self.btnDisconnect.clicked.connect(self.disconnectFromESP32)

        # Create a vertical layout to organize the elements
        layout = QVBoxLayout()
        layout.addWidget(self.label_connect)
        layout.addWidget(self.btnConnect)
        layout.addWidget(self.btnDisconnect)

        self.setLayout(layout)

        # Initialize the connection parameters and socket
        self.host = '192.168.4.1'  # ESP32 Access Point name
        self.port = 80  # Port number the ESP32 is listening on
        self.s = None

    def connectToESP32(self):
        try:
            # Close the previous connection if it exists
            if self.s:
                self.s.close()
            
            # Create a new socket and connect to the ESP32
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.connect((self.host, self.port))

            # Read the message from ESP32
            message = self.s.recv(1024).decode('utf-8')
            print(message)
          
            # Update the UI to indicate a successful connection
            self.label_connect.setText('Connected')
        except socket.error as e:
            # Handle connection errors and update the UI accordingly
            self.label_connect.setText('Connection failed - ' + str(e))

    def disconnectFromESP32(self):
        try:
            # Close the existing connection and set the socket to None
            if self.s:
                self.s.close()
                self.s = None
                self.label_connect.setText('Disconnected')
            else:
                # Update the UI if there is no active connection
                self.label_connect.setText('Not Connected')
        except socket.error as e:
            # Handle disconnection errors and update the UI accordingly
            self.label_connect.setText('Disconnection failed - ' + str(e))

if __name__ == '__main__':
    # Create and run the application
    app = QApplication(sys.argv)
    window = WiFiClientApp()
    window.show()
    sys.exit(app.exec_())
