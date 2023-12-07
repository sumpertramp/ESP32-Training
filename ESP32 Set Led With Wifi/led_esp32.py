import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout
from PyQt5.QtCore import QUrl
from PyQt5.QtNetwork import QNetworkRequest, QNetworkAccessManager

class ESP32ControlApp(QWidget):
    def __init__(self):
        super().__init__()

        self.init_ui()

    def init_ui(self):
        self.setWindowTitle('ESP32 LED Control')
        self.setGeometry(100, 100, 300, 150)

        self.esp32_ip = "192.168.1.163"  # ESP32'nin IP adresi
        self.esp32_port = 80  # ESP32 web server port numarası

        self.layout = QVBoxLayout()

        self.btn_on = QPushButton('Turn ON', self)
        self.btn_on.clicked.connect(self.turn_on_led)
        self.layout.addWidget(self.btn_on)

        self.btn_off = QPushButton('Turn OFF', self)
        self.btn_off.clicked.connect(self.turn_off_led)
        self.layout.addWidget(self.btn_off)

        self.setLayout(self.layout)

    def send_request(self, path):
        manager = QNetworkAccessManager(self)
        url = QUrl(f"http://{self.esp32_ip}:{self.esp32_port}/{path}")
        request = QNetworkRequest(url)
        manager.get(request)

    def turn_on_led(self):
        self.send_request("start")

    def turn_off_led(self):
        self.send_request("stop")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = ESP32ControlApp()
    window.show()
    sys.exit(app.exec_())