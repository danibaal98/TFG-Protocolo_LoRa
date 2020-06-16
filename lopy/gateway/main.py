import socket
import struct
from network import LoRa
import wifiConnect
import time
import urequests as requests

_LORA_PKG_FORMAT = "Bd"
_LORA_PKG_ACK_FORMAT = '!BBB'

FIWARE_ADDR = '195.220.224.174:7896'

# Open a Lora Socket, use rx_iq to avoid listening to our own messages
lora = LoRa(mode=LoRa.LORA, region=LoRa.EU868)
lora_sock = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
lora_sock.setblocking(False)

KEY = b'notsuchsecretkey'

def sendDataToFIWARE(msg):
    r = requests.post('http://{}/iot/d?k=1068318794&i=sensor001'.format(FIWARE_ADDR), headers={'Content-Type': 'text/plain'}, data='temp|{}'.format(msg))
    if r.status_code == 200:
        print('Enviado con exito a FIWARE')
    else:
        print('Error')

def getData():
    data = lora_sock.recv(512)

    if data:
        device_id, msg = struct.unpack(_LORA_PKG_FORMAT, data)
        print('Message received {}'.format(msg))
        sendDataToFIWARE(msg)

        ack_pkg = struct.pack(_LORA_PKG_ACK_FORMAT, device_id, 1, 200)

        lora_sock.send(ack_pkg)
        
    time.sleep_ms(50)


if __name__ == '__main__':
    #print('eeee')
    ip = wifiConnect.connect()
    print('Connected to WiFi with IP: {}'.format(ip))
    while True:
        getData()
