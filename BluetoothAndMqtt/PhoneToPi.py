import paho.mqtt.client as mqtt
from time import sleep
from bluetooth import *
from PiToBluefruit import send_ble
try:
	global flag
	flag = True

	BLE_SERVER = None
	UUID = None

	def start_client(broker='192.168.2.41', sub_list=['other', 'info'], client_name='client-002'):
		return broker, sub_list, mqtt.Client(client_name)

	broker, sub_list, client = start_client()

	pub_topic = sub_list[0]
	sub_topic = sub_list[1]

	# TODO: Change it so sleep(3) is no longer needed. It has been started with state
	def bluetooth_call(message):
		print("Called Bluetooth")
		sleep(3)
		if None in [BLE_SERVER, UUID]:
			state = send_ble(message=message)
		else:
			state = send_ble(addr=BLE_SERVER ,uuid=UUID , message=message)
		return message, state


	def on_message(client, userdata, message):
		sleep(1)
		message = str(message.payload.decode("utf-8").strip())

		try:
			if message == "quit":
				print("setting flag to false")
				global flag
				flag = False
			elif message == "1":
				client.publish(pub_topic, bluetooth_call(message) + " was sent to ble\n")
			else:
				client.publish(pub_topic, message + " was recived on pi\n")
		except Exception as err:
			print(err)
		finally:
			print("Messsage recieved: ", message, "\n")


	client.on_message = on_message


	client.connect(broker)
	client.loop_start()

	print("listening on ", sub_topic)
	print("returning info on ", pub_topic)

	client.subscribe(sub_topic)

	while flag:
		sleep(1)
finally:
	print("Closing Temp/Light Client")
	sleep(1)
	client.disconnect()
	client.loop_stop()
