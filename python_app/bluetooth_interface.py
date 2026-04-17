from jnius import autoclass
from android.permissions import request_permissions, Permission

BluetoothAdapter = autoclass('android.bluetooth.BluetoothAdapter')
BluetoothDevice = autoclass('android.bluetooth.BluetoothDevice')
BluetoothSocket = autoclass('android.bluetooth.BluetoothSocket')
UUID = autoclass('java.util.UUID')

request_permissions([
    Permission.BLUETOOTH_CONNECT, 
    Permission.BLUETOOTH_SCAN,
    Permission.ACCESS_FINE_LOCATION
])

def get_socket_stream(name):
    paired_devices = BluetoothAdapter.getDefaultAdapter().getBondedDevices().toArray()
    socket = None

    for device in paired_devices:
        if device.getName() == name:
            socket = device.createRfcommSocketToServiceRecord(
                UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"))
            recv_stream = socket.getInputStream()
            send_stream = socket.getOutputStream()
            break
    
    if socket is None:
        print('Not Connected')
        return None, None
    
    socket.connect()
    print('Connected')
    return recv_stream, send_stream