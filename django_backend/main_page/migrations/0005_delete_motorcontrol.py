# Generated by Django 5.0 on 2023-12-14 03:57

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('main_page', '0004_alter_motorcontrol_motor_id'),
    ]

    operations = [
        migrations.DeleteModel(
            name='MotorControl',
        ),
    ]
