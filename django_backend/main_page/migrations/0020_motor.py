# Generated by Django 5.0 on 2023-12-17 18:08

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('main_page', '0019_alter_loginrecord_token'),
    ]

    operations = [
        migrations.CreateModel(
            name='Motor',
            fields=[
                ('id', models.AutoField(primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=64)),
                ('avaliable', models.BooleanField(default=True)),
                ('description', models.CharField(max_length=256)),
            ],
        ),
    ]
