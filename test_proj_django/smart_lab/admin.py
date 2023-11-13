from django.contrib import admin

# Register your models here.

from .models import Chemical, Operator, Experiment, Task, Management

model_list = [Chemical, Operator, Experiment, Task, Management]

for model in model_list:
    admin.site.register(model)
