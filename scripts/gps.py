import time
import cellular
import gps
import machine

# Initialize GPS
gps.on()

# Initialize LED pin (adjust the pin number as necessary)
led = machine.Pin(27, machine.Pin.OUT)

# Factory default coordinates (last known location before shipping)
factory_default_location = ("22.51420833333333", "113.5009966666667")

# Function to blink LED
def blink_led(interval):
    led.value(1)
    time.sleep(interval / 2)
    led.value(0)
    time.sleep(interval / 2)

# Function to get a valid GPS location
def get_valid_location(timeout=300):
    start_time = time.time()
    waiting_message_printed = False
    while True:
        # Blink LED fast (every 1 second) while looking for signal
        blink_led(1)

        # Check elapsed time
        elapsed_time = time.time() - start_time
        if elapsed_time > timeout:
            print("Timeout: Unable to get a valid GPS fix.")
            return None
        
        location = gps.get_location()
        satellites = gps.get_satellites()
        
        # Check if the GPS has valid satellite data and a new location
        if satellites[0] > 0 and location != factory_default_location:
            print("Valid Location:", location)
            print("Satellites (tracked, visible):", satellites)
            print("-----")
            return location
        else:
            if not waiting_message_printed:
                print("Waiting for GPS fix...")
                waiting_message_printed = True

# Get a valid GPS location
location = get_valid_location()
if location:
    print("Valid location acquired:", location)
else:
    print("Failed to acquire a valid location within the timeout period.")

# Keep printing the valid location and blinking the LED every 3 seconds to indicate the GPS is connected
while True:
    location = gps.get_location()
    satellites = gps.get_satellites()
    print("Current Location:", location)
    print("Satellites (tracked, visible):", satellites)
    blink_led(3)
