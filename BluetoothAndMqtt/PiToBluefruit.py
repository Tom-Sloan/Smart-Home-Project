from bluepy.btle import UUID, Peripheral
import argparse


# Command Line Interface
def main():
    '''
    Used Like:
    
    python PiToBluefruit CA:34:A7:E4:DF:50 0x0001 1
    
    This would turn off the light of the Adafruit Feather
    
    Default is set to the parameters of the example
    
    '''
    
    
    parser = argparse.ArgumentParser(description= "Connect to nrf52 via Bluetooth")
    #Address Parameter
    parser.add_argument('address', metavar='MAC_ADDR', type=str, nargs="?",  
        help='mac address of BLE device usually in 00:00:00:00:00:00', default="CA:34:A7:E4:DF:50")
    # Characteristic UUID Parameter
    parser.add_argument('chara_uuid', metavar='UUID', type=lambda x: hex(int(x,0)), nargs="?", 
        help='Charactistic UUID on BLE device', default='0x0001')
    # Message Parameter
    parser.add_argument('message', metavar='M', type=str, nargs="?",
        help='message to be sent', default='2') 
    args = parser.parse_args() # Gets all values passed by cl

    addr = args.address # Gets MAC Address of BLE device, first argument
    uuid = UUID(int(args.chara_uuid, 16)) # Convert hexstring to int-hex of passed argument and create UUID
    message = args.message # Gets message passed which was third argument
    
    send_ble(addr, uuid, message)
    
    
    
def send_ble(addr="CA:34:A7:E4:DF:50", uuid=0x0001, message='2'):
    try:
        print("ble sending ", message)
        p = Peripheral(addr, "random") # Connect to Peripheral with addr
        ch = p.getCharacteristics(uuid=uuid)[0] # Get first Characteristic of Peripheral
        print("Sending {}".format(message)) 
        ch.write(str.encode(message), withResponse=True) # Send message as bytes, withResponse checks if message sent
    finally:
        p.disconnect() # Disconnect from Peripheral

if __name__ == '__main__':
    main()
