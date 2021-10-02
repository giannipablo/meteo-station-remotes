import os
import sys
import time
import signal
import datetime
import requests
import random


API_URL = os.getenv('API_URL')
PERIOD = int(os.getenv('PERIOD', '30'))


print(PERIOD)
def signal_handler(signal, frame):
    print('\nYou pressed Ctrl+C!')
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
print('Press Ctrl+C')




while (True):

    tosend = {
        "uuid" : "640f9d98660d41d7bbe4edf9887de873",
        "dev_name" : "Ricardo 1",
        "time_stamp" : f'{datetime.datetime.utcnow().isoformat()}',
        "payload" : {
            "temperature" : random.normalvariate(25,0.1),
            "humidity" : random.normalvariate(50, 0.03),
            "wind_speed" : random.normalvariate(30,1),
            "preasure" : random.normalvariate(400, 10)
        }
    }

    requests.post(API_URL, json=tosend)

    time.sleep(PERIOD)
