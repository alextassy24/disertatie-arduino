import machine
import time
import cellular
import gps

# Initialize GPS
gps.on()
    
# Initialize GPRS
def initialize_gprs(apn, user, pwd):
    try:
        print("Initializing GPRS...")
        cellular.gprs(apn, user, pwd)
        print("GPRS initialized.")
        return True
    except Exception as e:
        print("Failed to initialize GPRS:", e)
        return False

# Define your APN settings
apn = "live.vodafone.ro"
user = "live"
pwd = ""

# Attempt to initialize GPRS
if not initialize_gprs(apn, user, pwd):
    print("Retrying GPRS initialization...")
    time.sleep(5)
    if not initialize_gprs(apn, user, pwd):
        raise RuntimeError("Failed to initialize GPRS after multiple attempts.")

# Proceed with your MQTT code after successfully initializing GPRS
try:
    from umqtt import simple
    print("umqtt Imported  successfully")
except ImportError:
    print("umqtt Failed to import")
    print("Installing umqtt...")
    try:
        import upip
        upip.install("micropython-umqtt.simple")
        try:
            print("Trying to import umqtt again...")
            from umqtt import simple
        except ImportError as e:
            print("Failed to import umqtt: ",e)
    except Exception as e:
        print("Failed to install umqtt: ",e)

# Define MQTT topics
led_topic = "alextassy24/feeds/led"
sensor_topic = "alextassy24/feeds/gps"

# Set up LED pin
led = machine.Pin(27, machine.Pin.OUT)

# Time period for sending data
time_period = 10

# Callback function for received messages
def cb(topic, msg):
    print('Received Data: Topic = {}, Msg = {}'.format(topic, msg))
    received_data = str(msg, 'utf-8')
    if received_data == "0":
        led.value(0)
    if received_data == "1":
        led.value(1)

# MQTT server details
name = "a9g-micropython"
server = "io.adafruit.com"
port = 1883
username = "alextassy24"
password = "Pudelpipi2708!"

# Connect to MQTT server
try:
    print("Connecting to MQTT server...")
    client = simple.MQTTClient(name, server, port, username, password)
    client.connect()
    print("Connected to MQTT server.")
except Exception as e:
    print("Failed to connect to MQTT server:", e)
    raise

# Set callback and subscribe to topic
client.set_callback(cb)
client.subscribe(led_topic)
print("Subscribed to topic:", led_topic)

# Function to send sensor data
def send_sensor():
    msg = "100"
    print('Sending Data: Topic = {}, Msg = {}'.format(sensor_topic, msg))
    client.publish(sensor_topic, msg)

# Main loop
while True:
    led.value(1)
    time.sleep(1)
    led = not value
    
    try:
        for _ in range(time_period):
            for _ in range(10):
                client.check_msg()
                time.sleep(0.1)
        send_sensor()
    except Exception as e:
        print("Error during MQTT operation:", e)
        # Optionally, you can add reconnect logic here


