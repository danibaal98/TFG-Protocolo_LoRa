from network import WLAN
import machine
 
def connect():
    ssid = "" # el nombre de tu red Wi-Fi
    password = "" # la contraseña de tu red Wi-Fi

    wlan = WLAN(mode=WLAN.STA)


    if wlan.isconnected() == True:
        print("Already connected")
        return

    #wlan.active(True)
    wlan.connect(ssid, auth=(WLAN.WPA2, password), timeout=5000)

    while not wlan.isconnected():
        machine.idle()

    print("Connection successful")
    ip = wlan.ifconfig()
    return ip[0]
