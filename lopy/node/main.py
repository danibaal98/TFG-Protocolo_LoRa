import socket
import time
import struct
# import random
import sys
from network import LoRa
from crypto import AES
import crypto


_LORA_PKG_FORMAT = "Bd"
_LORA_PKG_ACK_FORMAT = '!BBB'

# Open a Lora Socket, use tx_iq to avoid listening to our own messages
lora = LoRa(mode=LoRa.LORA, region=LoRa.EU868)
lora_sock = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
lora_sock.setblocking(False)

mac_addr = lora.mac()
NODE_ID = mac_addr[7]

def random_number(rfrom, rto):
    def Random():
        r = crypto.getrandbits(32)
        return ((r[0] << 24)+(r[1] << 16)+(r[2] << 8)+r[3])/4294967295.0

    return Random()*(rto-rfrom)+rfrom

# Method to send messages


def send_msg(temperature):
    msg = struct.pack(_LORA_PKG_FORMAT, NODE_ID, temperature)

    wait_ack = True
    recv_timeout = 0

    while wait_ack and recv_timeout < 20:
        lora_sock.send(msg)
        print('Message sended to gateway with temperature {}'.format(temperature))

        recv_ack = lora_sock.recv(256)
        recv_timeout += 1

        if len(recv_ack) > 0:
            device_id, pkg_len, status = struct.unpack(
                _LORA_PKG_ACK_FORMAT, recv_ack)

            if NODE_ID == device_id:
                if status == 200:
                    wait_ack = False
                    print('ACK received {}'.format(status))
                else:
                    wait_ack = False
                    print('Message failed')

        time.sleep(1)


# Main Loop

if __name__ == '__main__':
    while(True):
        time.sleep(20)
        temperature = round(random_number(15, 30), 1)
        send_msg(temperature)
