from rest_framework import serializers
from .models import Task, MotorControl, User, LoginRecord, Motor, Spinning

class TaskSerializer(serializers.ModelSerializer):
    class Meta:
        model = Task
        fields = '__all__'
        read_only_fields = ('task_id',)

class MotorControlSerializer(serializers.ModelSerializer):
    class Meta:
        model = MotorControl
        fields = '__all__'
        read_only_fields = ('id',)

class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = '__all__'

class LoginRecordSerializer(serializers.ModelSerializer):
    class Meta:
        model = LoginRecord
        fields = '__all__'

class MotorSerializer(serializers.ModelSerializer):
    class Meta:
        model = Motor
        fields = '__all__'

class SpinningSerializer(serializers.ModelSerializer):
    class Meta:
        model = Spinning
        fields = '__all__'