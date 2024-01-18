from django.db import models
from django.utils import timezone

# Create your models here.

class Task(models.Model):
    task_id = models.AutoField(primary_key=True, null=False)
    task_name = models.CharField(max_length=128, null=False)
    task_description = models.CharField(max_length=256)

class MotorControl(models.Model):
    id = models.AutoField(primary_key=True, null=False)
    motor_name = models.CharField(max_length=128, null=False)
    motor_speed = models.IntegerField(null=False)
    time = models.DateTimeField(auto_now_add=True, null=False)

class User(models.Model):
    id = models.AutoField(primary_key=True, null=False)
    email = models.CharField(max_length=128, null=False)
    username = models.CharField(max_length=128, null=False)
    password = models.CharField(max_length=256)
    activated = models.BooleanField(default=False)
    register_time = models.DateTimeField(auto_now_add=True, null=False)

class LoginRecord(models.Model):
    id = models.AutoField(primary_key=True, null=False)
    email = models.CharField(max_length=128, null=False)
    login_time = models.DateTimeField(auto_now_add=True, null=False)
    token = models.CharField(max_length=512, null=False)

class Motor(models.Model):
    id = models.AutoField(primary_key=True, null=False)
    name = models.CharField(max_length=64, null=False)
    avaliable = models.BooleanField(default=True)
    description = models.CharField(max_length=256)

class Spinning(models.Model):
    id = models.AutoField(primary_key=True, null=False)
    motor_name = models.CharField(max_length=128, null=False)
    scheduled_time = models.DateTimeField(null=False)
    motor_speed = models.IntegerField(null=False)
    duration_sec = models.IntegerField(null=False)


