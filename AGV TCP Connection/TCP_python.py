import sys
from PyQt5.QtWidgets import QApplication, QWidget, QGraphicsScene, QGraphicsView, QGraphicsPolygonItem, QGraphicsTextItem, QMainWindow, QAction, QToolBar, QMessageBox, QDialog, QVBoxLayout, QLabel, QLineEdit, QPushButton
from PyQt5.QtGui import QIcon, QPolygonF, QFont, QTransform
from PyQt5.QtCore import QTimer, Qt, QTimer, QPointF
import socket

pos_x = pos_y = pos_z = pos_q = 0.0
angle_x = angle_y = angle_z = 0.0
anchor1_x = anchor2_x = anchor3_x = anchor4_x = 0.0
anchor1_y = anchor2_y = anchor3_y = anchor4_y = 0.0
anchor1_ID = anchor2_ID = anchor3_ID = anchor4_ID = ""


class AnchorObject(QGraphicsPolygonItem):
    def __init__(self):
        super().__init__()

        anchor = QPolygonF([
            QPointF( 50, 50 - 5),
            QPointF( 50 + 5, 50),
            QPointF( 50, 50 + 5),
            QPointF( 50 - 5, 50),
            QPointF(50, 50 - 5) 
        ])
        self.setPolygon(anchor)
        self.setBrush(Qt.blue)

    def drawAnchor(self,x, y):
        anchor = QPolygonF([
            QPointF(x * 120, y * 120 - 5),
            QPointF(x * 120 + 5, y * 120),
            QPointF(x * 120, y * 120 + 5),
            QPointF(x * 120 - 5, y * 120),
            QPointF(x * 120, y * 120 - 5)
        ])
        self.setPolygon(anchor)

class AnchorText(QGraphicsTextItem):
    def __init__(self):
        super().__init__()

        anchor_text = QGraphicsTextItem("----")
    
    def setAnchorText(self, ID, x, y):

        self.setPlainText(f"{ID}\n({x}, {y})")
        self.setPos(x * 120, y * 120 - 35)

class TagObject(QGraphicsPolygonItem):
    def __init__(self):
        super().__init__()

        # Tag'ı oluştur
        tag = QPolygonF([

            QPointF(50, 50 - 5),
            QPointF(50 + 5, 50),
            QPointF(50, 50 + 5),
            QPointF(50 - 5, 50),
            QPointF(50, 50 - 5) 
          
        ])
        
        #tagın büyüklüğünü değiştirme
        tag = QPolygonF([point / 2 for point in tag])

        self.setPolygon(tag)
        self.setBrush(Qt.red)
    
    def drawTag(self, x, y):

        tag_size = 10  # Beşgenin boyutu

        tag = QPolygonF([
            QPointF(x * 120, y * 120 - tag_size),
            QPointF(x * 120 + tag_size, y * 120),
            QPointF(x * 120 + tag_size, y * 120 + tag_size),
            QPointF(x * 120 - tag_size, y * 120 + tag_size),
            QPointF(x * 120 - tag_size, y * 120)
        ])

        self.setPolygon(tag)

    def rotateTag(self, angle):
        center = self.boundingRect().center()

        # Mevcut merkezi (0, 0) noktasına taşı
        translation_transform = QTransform().translate(center.x(), center.y())
        self.setTransform(translation_transform)

        # Döndürme işlemi
        rotation_transform = QTransform().rotate(angle)
        self.setTransform(rotation_transform, combine=True)

        # Mevcut merkezi eski yerine taşı
        reverse_translation_transform = QTransform().translate(-center.x(), -center.y())
        self.setTransform(reverse_translation_transform, combine=True)


class TagText(QGraphicsTextItem):
    def __init__(self):
        super().__init__()

        tag_text = QGraphicsTextItem("----")
    
    def setTagText(self, x, y):

        self.setPlainText(f"({x}, {y})")
        self.setPos(x * 120, y * 120 - 35)

class MainWidget(QWidget):
    def __init__(self):
        super().__init__()

        # Pozisyon bilgilerini gösteren etiketler
        self.label_pos = QLabel(self)
        self.label_pos.setAlignment(Qt.AlignCenter)
        self.label_pos.setText("Bağlantı kurulamadı! IP adresi ve port numarasını kontrol ediniz.")
        self.label_pos.setStyleSheet("color: white; background-color: gray;")
        self.label_pos.setFont(QFont("Arial", 12, QFont.Bold))
        
        #Açı bilgilerini gösteren etiketler
        self.label_angle = QLabel(self)
        self.label_angle.setAlignment(Qt.AlignCenter)
        self.label_angle.setText("Açı değeri alınamıyor.")
        self.label_angle.setStyleSheet("color: white; background-color: gray;")
        self.label_angle.setFont(QFont("Arial", 12, QFont.Bold))

        # Nesneyi eklemek için sahne ve görüntüleme oluşturuluyor
        self.scene = QGraphicsScene()
        self.view = QGraphicsView(self.scene)
        self.view.setFixedSize(800, 800)

        # Nesneyi sahneye ekleniyor
        self.tag_rotating = TagObject()
        self.scene.addItem(self.tag_rotating)

        #Anchor1 oluştur
        self.anchor1 = AnchorObject()
        self.scene.addItem(self.anchor1)

        #Anchor2 oluştur
        self.anchor2 = AnchorObject()
        self.scene.addItem(self.anchor2)

        #Anchor3 oluştur
        self.anchor3 = AnchorObject()
        self.scene.addItem(self.anchor3)

        #Anchor4 oluştur
        self.anchor4 = AnchorObject()
        self.scene.addItem(self.anchor4)
        
        #Anchor1 text ekle
        self.anchor1_text = AnchorText()
        self.scene.addItem(self.anchor1_text)

        #Anchor2 text ekle
        self.anchor2_text = AnchorText()
        self.scene.addItem(self.anchor2_text)

        #Anchor3 text ekle
        self.anchor3_text = AnchorText()
        self.scene.addItem(self.anchor3_text)

        #Anchor4 text ekle
        self.anchor4_text = AnchorText()
        self.scene.addItem(self.anchor4_text)

        #Tag text ekle
        self.tag_text = TagText()
        self.scene.addItem(self.tag_text)

        # Ana pencere düzeni
        vbox = QVBoxLayout(self)
        vbox.addWidget(self.label_pos)
        vbox.addWidget(self.label_angle)
        vbox.addWidget(self.view)
        self.setLayout(vbox)


class WifiDialog(QDialog):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):

        self.setWindowTitle("AGV'i İnternete Bağla")
        self.setWindowIcon(QIcon("Icon_AGV.png"))
        self.setFixedSize(250, 250)
        self.setWindowFlags(self.windowFlags() & ~Qt.WindowContextHelpButtonHint)
        self.setStyleSheet("background-color: #333333;")

        layout = QVBoxLayout()
    
        self.label_username = QLabel("Kullanıcı Adı:")
        self.label_username.setStyleSheet("color: #FFA500;")
        self.label_username.setFont(QFont("Arial", 10, QFont.Bold))
        self.edit_username = QLineEdit(self)
        self.edit_username.setStyleSheet("color: white; font-weight: bold;")

        self.label_password = QLabel("Parola:")
        self.label_password.setStyleSheet("color: #FFA500;")
        self.label_password.setFont(QFont("Arial", 10, QFont.Bold))
        self.edit_password = QLineEdit(self)
        self.edit_password.setStyleSheet("color: white; font-weight: bold;")
        self.edit_password.setEchoMode(QLineEdit.Password)

        self.label_IP = QLabel("IP Numarası:")
        self.label_IP.setStyleSheet("color: #FFA500;")
        self.label_IP.setFont(QFont("Arial", 10, QFont.Bold))
        self.edit_IP = QLineEdit(self)
        self.edit_IP.setStyleSheet("color: white; font-weight: bold;")
        
        self.label_port = QLabel("Port Numarası:")
        self.label_port.setStyleSheet("color: #FFA500;")
        self.label_port.setFont(QFont("Arial", 10, QFont.Bold))
        self.edit_port = QLineEdit(self)
        self.edit_port.setStyleSheet("color: white; font-weight: bold;")

        self.btn_connect = QPushButton("Bağlan", self)
        self.btn_connect.setStyleSheet("background-color: #FFA500; color: #333333;")
        self.btn_connect.clicked.connect(self.accept)

        self.error_label = QLabel()
        self.error_label.setStyleSheet("color: red")

        layout.addWidget(self.label_username)
        layout.addWidget(self.edit_username)
        layout.addWidget(self.label_password)
        layout.addWidget(self.edit_password)
        layout.addWidget(self.label_IP)
        layout.addWidget(self.edit_IP)
        layout.addWidget(self.label_port)
        layout.addWidget(self.edit_port)
        layout.addWidget(self.btn_connect)
        layout.addWidget(self.error_label)

        self.setLayout(layout)

    def getUsernameAndPassword(self):
        username = self.edit_username.text()
        password = self.edit_password.text()
        ip = self.edit_IP.text()
        port = self.edit_port.text()
        return username, password, ip, port
    
    def tryConnection(self):
        ip_address = self.edit_IP.text()
        port_number = int(self.edit_port.text())

        try:
            #Create a socket
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.settimeout(5) #Adjust the timeout

            #Connect to client
            client_socket.connect((ip_address, port_number))
            self.accept()
        except socket.error as e:
            self.error_label.setText("Hata: Bağlantı kurulamadı. Lütfen IP adresi ve port numarasını kontrol ediniz. ")
        except ValueError as e:
            self.error_label.setText()

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        # Set up the main user interface elements
        self.setWindowTitle("AGV Kontrol Paneli")
        self.setWindowIcon(QIcon("Icon_AGV.png"))
        self.setStyleSheet("background-color: lightgray")
        self.setGeometry(100, 100, 800, 600)

        # Add main widget
        self.central_widget = MainWidget()
        self.setCentralWidget(self.central_widget)

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

        # Set up a timer for connection timeout
        self.timeout_timer = QTimer(self)
        self.timeout_timer.timeout.connect(self.handleConnectionTimeout)

    def toggleAPConnection(self):
        if self.s:
            self.disconnectFromAccessPoint()
        else:
            self.connectToAccessPoint()
        
    def toggleWiFiConnection(self):
        wifi_dialog = WifiDialog()
        result = wifi_dialog.exec_()
 
        if result == QDialog.Accepted:
            username, password, ip, port = wifi_dialog.getUsernameAndPassword()
            self.sendWiFiCredentials(username, password, ip, port)
            self.refreshData(ip,port)

    def sendWiFiCredentials(self, username, password, ip, port):
        url = f"http://{self.host}/submit"
        data = {"username": username, "password": password, "ip": ip, "port": port}

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

        print(f"Connecting to ESP32 via WiFi with Username: {username}, Password: {password}, Ip: {ip}, Port: {port} ")    
    
    def refreshData(self, ip, port):
        while True:
            try:
                # Create socket object using IP
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    #Bind the socket to the specified IP address and port
                    s.bind((ip, int(port)))
                    # Listen for incoming connections
                    s.listen()
                    print(f"Server listening on {ip}:{port}")

                    while True:
                        # Accept a willingness connection from a client
                        conn, addr = s.accept()
                        with conn:
                            print(f"Connected by {addr}")
                            while True:
                                data = conn.recv(1024)
                                if not data:
                                    break
                                data_client = f"Received: {data.decode("utf-8")}".strip()
                                print(data_client)

                                # Processing data
                                lines = data_client.split('\n')

                                for line in lines:
                                    elements = line.split(',')

                                    if "Received: DIST" in elements:
                                        DIST_index = elements.index("Received: DIST")
                                        anchor_count = int(elements[DIST_index + 1])
                                        print("Anchor Sayısı: " + str(anchor_count))
                                
                                    if "AN0" in elements:
                                        AN0_index = elements.index("AN0")
                                        anchor1_ID = str(elements[AN0_index + 1])
                                        anchor1_x = float(elements[AN0_index + 2])
                                        anchor1_y = float(elements[AN0_index + 3])
                                        anchor1_z = float(elements[AN0_index + 4])
                                    
                                        print("Anchor1 IC: " + str(anchor1_ID))
                                        print("Anchor1 X: " + str(anchor1_x))
                                        print("Anchor1 Y: " + str(anchor1_y))
                                        print("Anchor1 Z: " + str(anchor1_z))
                                
                                    if "AN1" in elements:
                                        AN1_index = elements.index("AN1")
                                        anchor2_ID = str(elements[AN1_index + 1])
                                        anchor2_x = float(elements[AN1_index + 2])
                                        anchor2_y = float(elements[AN1_index + 3])
                                        anchor2_z = float(elements[AN1_index + 4])
                                
                                        print("Anchor2 ID: " + str(anchor2_ID))
                                        print("Anchor2 X: " + str(anchor2_x))
                                        print("Anchor2 Y: " + str(anchor2_y))
                                        print("Anchor2 Z: " + str(anchor2_z))
                                
                                    if "AN2" in elements:
                                        AN2_index = elements.index("AN2")
                                        anchor3_ID = str(elements[AN2_index + 1])
                                        anchor3_x = float(elements[AN2_index + 2])
                                        anchor3_y = float(elements[AN2_index + 3])
                                        anchor3_z = float(elements[AN2_index + 4])

                                        print("Anchor3 ID: " + str(anchor3_ID))
                                        print("Anchor3_X: " + str(anchor3_x))
                                        print("Anchor3_Y: " + str(anchor3_y))
                                        print("Anchor3_Z: " + str(anchor3_z))

                                    if "AN3" in elements:
                                        AN3_index = elements.index("AN3")
                                        anchor4_ID = str(elements[AN3_index + 1])
                                        anchor4_x = float(elements[AN3_index + 2])
                                        anchor4_y = float(elements[AN3_index + 3])
                                        anchor4_z = float(elements[AN2_index + 4])

                                        print("Anchor4 ID: " + str(anchor4_ID))
                                        print("Anchor4 X: " + str(anchor4_x))
                                        print("Anchor4 Y: " + str(anchor4_y))
                                        print("Anchor4 Z: " + str(anchor4_z))
                                
                                    if "POS" in elements:
                                        pos_index = elements.index("POS")
                                        pos_x = float(elements[pos_index + 1])
                                        pos_y = float(elements[pos_index + 2])
                                        pos_z = float(elements[pos_index + 3])
                                        pos_q = float(elements[pos_index + 4])
                                
                                    if "Received: E:" in elements:
                                        angle_index = elements.index("Received: E:")
                                        angle_x = float(elements[angle_index + 1])
                                        angle_y = float(elements[angle_index + 2])
                                        angle_z = float(elements[angle_index + 3])

                                        print(f"Angle x: {angle_x}")
                                        print(f"Angle y: {angle_y}")
                                        print(f"Angle z: {angle_z}")
        
            except Exception as e:
                print(f"Hata {e}")
                #conn.sendall(b"Data received successfully")

    def connectToAccessPoint(self):
        try:
            # Close the previous connection if it exists
            if self.s:
                self.s.close()
                pass
             
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
    
    def disconnectFromAccessPoint(self):
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