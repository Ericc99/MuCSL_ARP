# 注意到如果我们直接使用通常的python manage.py runserver 0.0.0.0:8000会造成创建了两个MQTT Client
# 这是因为Django会创建两个线程，其中一个是主线程，另外一个是支线程用来在必要的时候刷新你的主线程
# 所以说我们可以在开启server的时候直接将支线程ban掉，即使用 --noreload指示
# 当然这么做我也不知道会不会对其他的东西产生影响，所以说我们可以通过以下Stack Overflow的回答来规避
# https://stackoverflow.com/questions/33814615/how-to-avoid-appconfig-ready-method-running-twice-in-django
# 依照上面给出的回答我们添加 if os.environ.get('RUN_MAIN') 判断即可规避掉在支线程当中的任何操作
# MQTT后端当中显示的连接数也变成2

import paho.mqtt.client as mqtt
from django.conf import settings
import os

from django.apps import apps

from asgiref.sync import async_to_sync
from channels.layers import get_channel_layer



def on_connect(mqtt_client, userdata, flags, rc):
    if rc == 0:
        print("MQTT Connect Success!")
        mqtt_client.subscribe('control')
    else:
        print("Bad Connection Code: ", rc)

def on_message(mqtt_client, userdata, msg):
    # print(f'Received message on topic: {msg.topic} with payload: {msg.payload}')
    topic = msg.topic
    payload = msg.payload.decode()
    channel_layer = get_channel_layer()
    async_to_sync(channel_layer.group_send)(
        'mqtt_group',
        {
            'type': 'mqtt_msg_broadcast',
            'message': payload
        }
    )
    if payload.startswith('PCNT count:'):
        count = int(payload[len('PCNT count: '):])
        motor_speed = int(count / 6)
        model = apps.get_model('main_page', 'MotorControl')
        model.objects.create(motor_speed=motor_speed, motor_name='Motor_1')
    


if os.environ.get('RUN_MAIN'):
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(settings.MQTT_USER, settings.MQTT_PASSWORD)
    client.connect(
        host=settings.MQTT_SERVER,
        port=settings.MQTT_PORT,
        keepalive=settings.MQTT_KEEPALIVE
    )