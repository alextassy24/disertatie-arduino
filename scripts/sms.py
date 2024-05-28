import cellular
import time

global flag
flag = 1

def sms_handles(event):
    global flag
    if event == cellular.SMS_SENT:
        print("SMS SENT!")
    
    elif event == cellular.SMS_RECEIVED:
        print("SMS Received, attempting to read...")
        ls = cellular.SMS.list()
        print(ls[-1])
        flag = 0
        
cellular.on_sms(sms_handler)
cellular.SMS("+40728552655", "Hello from A9G").send()

while flag:
    time.sleep(1)
    
print("Done!")