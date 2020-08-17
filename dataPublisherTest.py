import time
from uuid import uuid4
from datetime import datetime
import zmq

def send_message(topic, id, timestamp, payload):
    message = [topic, payload.encode("ascii")]
    sock.send_multipart(message)




ctx = zmq.Context()
sock = ctx.socket(zmq.PUB)
sock.bind('tcp://*:9990')
topic = b"test"
offset = 0
x=0
y=0
while True:
    message_id, now = str(uuid4()), datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    x = x+1
    y = x*2

    payload = "${data1} {data2};".format(data1=x,data2=y);
    send_message(topic,message_id,now,payload)
    time.sleep(0.01)

