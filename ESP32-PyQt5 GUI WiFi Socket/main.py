
'''
===============================================================================
File: ESP32_Client_GUI.py
Date: [13.12.2023]

Description:
   This Python script creates a GUI application to interact with an ESP32 device
   over a TCP socket. It allows the user to enter data, send it to the ESP32, and
   display the response.

Usage:
   - Ensure the PyQt5 library is installed (install with 'pip install PyQt5').
   - Run the script, and a GUI window will appear.
   - Input the ESP32's IP address and port in the script's 'esp32_ip' and
     'esp32_port' variables.

Note:
   - This script assumes the existence of an ESP32 device running a server that
     can handle requests for simulated data. Adjust the 'esp32_ip' and
     'esp32_port' variables accordingly.

Dependencies:
   - PyQt5

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
===============================================================================
'''

import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QLineEdit, QPushButton
from PyQt5.QtNetwork import QTcpSocket

class ESP32Client(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("ESP32 Client")
        self.setGeometry(100, 100, 400, 200)

        # Create interface elements
        self.label = QLabel("Enter data to send to ESP32:")
        self.text_edit = QLineEdit()
        self.send_button = QPushButton("Send Data")
        self.response_label = QLabel("Response from ESP32:")

        # Create layout
        layout = QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(self.text_edit)
        layout.addWidget(self.send_button)
        layout.addWidget(self.response_label)

        # Set layout for the widget
        self.setLayout(layout)

        # Establish connections
        self.send_button.clicked.connect(self.send_data)

        # Create TCP socket
        self.tcp_socket = QTcpSocket()

        # IP address and port of ESP32
        self.server_ip = "192.168.1.163"
        self.server_port = 80

    def send_data(self):
        # Get the data
        data_to_send = self.text_edit.text()

        # Connect with TCP socket
        self.tcp_socket.connectToHost(self.server_ip, self.server_port)

        if self.tcp_socket.waitForConnected(1000):
            # Send the data
            self.tcp_socket.write(data_to_send.encode())

            # Receive the response
            if self.tcp_socket.waitForReadyRead(3000):
                response_data = self.tcp_socket.readAll().data().decode()
                self.response_label.setText("Response from ESP32: " + response_data)
            else:
                self.response_label.setText("Timeout! No response received.")

            # Close the connection
            self.tcp_socket.disconnectFromHost()
            self.tcp_socket.waitForDisconnected(1000)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    client_app = ESP32Client()
    client_app.show()
    sys.exit(app.exec_())