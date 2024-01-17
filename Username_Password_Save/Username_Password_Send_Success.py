import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QAction, QToolBar, QMessageBox, QDialog, QVBoxLayout, QLabel, QLineEdit, QPushButton
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QTimer
from PyQt5.QtNetwork import QTcpSocket
import socket

class WifiDialog(QDialog):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.setWindowTitle("WiFi Bağlantı Bilgileri")
        self.setGeometry(200, 200, 300, 150)

        layout = QVBoxLayout()

        self.label_username = QLabel("Kullanıcı Adı:")
        self.edit_username = QLineEdit(self)

        self.label_password = QLabel("Parola:")
        self.edit_password = QLineEdit(self)
        self.edit_password.setEchoMode(QLineEdit.Password)

        self.btn_connect = QPushButton("Bağlan", self)
        self.btn_connect.clicked.connect(self.accept)

        layout.addWidget(self.label_username)
        layout.addWidget(self.edit_username)
        layout.addWidget(self.label_password)
        layout.addWidget(self.edit_password)
        layout.addWidget(self.btn_connect)

        self.setLayout(layout)

    def getUsernameAndPassword(self):
        username = self.edit_username.text()
        password = self.edit_password.text()
        return username, password

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        # Set up the main user interface elements
        self.setWindowTitle("ESP32 Kontrol Paneli")
        self.setGeometry(100, 100, 800, 600)

        self.toolbar = QToolBar(self)
        self.addToolBar(self.toolbar)

        self.access_point_action = QAction(QIcon("APdisconnect.png"), "Erişim Noktasına Bağlan", self)
        self.toolbar.addAction(self.access_point_action)
        self.access_point_action.triggered.connect(self.toggleAPConnection)

        self.wifi_action = QAction(QIcon("WiFidisconnect.png"), "WiFi'a Bağlan", self)
        self.toolbar.addAction(self.wifi_action)
        self.wifi_action.triggered.connect(self.toggleWiFiConnection)
          
        # Initialize the connection parameters and socket
        self.host = '192.168.4.1'  # ESP32 Access Point name
        self.port = 80  # Port number the ESP32 is listening on
        self.s = None

        # Create a TCP socket
        self.tcp_socket = QTcpSocket()

        # Set up a timer for connection timeout
        self.timeout_timer = QTimer(self)
        self.timeout_timer.timeout.connect(self.handleConnectionTimeout)

    def toggleAPConnection(self):
        if self.s:
            self.disconnectFromESP32()
        else:
            self.connectToESP32()
    
    def toggleWiFiConnection(self):
        wifi_dialog = WifiDialog()
        result = wifi_dialog.exec_()

        if result == QDialog.Accepted:
            username, password = wifi_dialog.getUsernameAndPassword()
            self.connectToESP32ViaWiFi(username, password)

    def connectToESP32ViaWiFi(self, username, password):
        # Add the code to connect to ESP32 via WiFi using the provided username and password
        # This is a placeholder, and you need to implement the actual WiFi connection logic
        # based on the ESP32 requirements

        #Connect with TCP socket
        url = f"http://{self.host}/submit"
        data = {"username": username, "password": password}

        # Use a library like requests to send POST request
        # You might need to install the requests library using: pip install requests
        try:
            import requests
            response = requests.post(url, data=data, timeout=5)

            if response.status_code == 200:
                # Connection successful, handle the response as needed
                print("Received response:", response.text)
            else:
                self.showMessageBox("Connection Error", f"Failed to connect to {self.host}: {response.status_code}")
        except Exception as e:
            self.showMessageBox("Connection Error", f"Failed to connect to {self.host}: {str(e)}")

        print(f"Connecting to ESP32 via WiFi with Username: {username} and Password: {password}")    


    def connectToESP32(self):
        try:
            # Close the previous connection if it exists
            if self.s:
                self.s.close()

            # Create a new socket and connect to the ESP32
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.timeout_timer.start(10000)  # Set a 10-second timeout
            self.s.connect((self.host, self.port))
            self.access_point_action.setIcon(QIcon("APconnect.png"))
            self.timeout_timer.stop()  # Connection successful, stop the timeout timer

            # Check if connected to the ESP32 access point
            if not self.isConnectedToAccessPoint():
                self.access_point_action.setIcon(QIcon("APdisconnect.png"))
                self.showMessageBox("Bağlantı Hatası", "ESP32 access pointine bağlantı sağlanamadı.")

        except socket.error as e:
            # Handle connection errors and show a message box
            self.access_point_action.setIcon(QIcon("APdisconnect.png"))
            self.timeout_timer.stop()  # Connection failed, stop the timeout timer
            self.showMessageBox("Bağlantı Hatası", "ESP32'ye bağlanırken bir hata oluştu:\n{}".format(str(e)))

    def isConnectedToAccessPoint(self):
        # Check if the connection to the ESP32 access point is successful
        # You can implement additional checks based on your requirements
        return True

    def disconnectFromESP32(self):
        try:
            if self.s:
                self.s.close()
                self.s = None
            self.access_point_action.setIcon(QIcon("APdisconnect.png"))

        except socket.error as e:
            # Handle disconnection errors and show a message box
            self.access_point_action.setIcon(QIcon("APdisconnect.png"))
            self.showMessageBox("Bağlantı Kesme Hatası", "ESP32 bağlantısı kesilirken bir hata oluştu:\n{}".format(str(e)))

    def handleConnectionTimeout(self):
        # Handle connection timeout and show a message box
        self.access_point_action.setIcon(QIcon("APdisconnect.png"))
        self.showMessageBox("Bağlantı Hatası", "ESP32'ye bağlantı sağlanamadı: Zaman aşımı")

    def showMessageBox(self, title, message):
        msg_box = QMessageBox(self)
        msg_box.setIcon(QMessageBox.Warning)
        msg_box.setWindowTitle(title)
        msg_box.setText(message)
        msg_box.exec_()

if __name__ == '__main__':
    # Create and run the application
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())