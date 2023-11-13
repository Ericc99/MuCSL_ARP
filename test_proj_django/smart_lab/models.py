from django.db import models
# 引入Django的User系统，这个是Django系统自带的用户管理系统
from django.contrib.auth.models import User

# Create your models here.

# 化学试剂库
class Chemical(models.Model):
    # 化学试剂ID
    chemical_id = models.AutoField(primary_key=True, null=False)
    # 化学试剂的名字
    chemical_name = models.CharField(max_length=64, null=False)
    # 化学试剂的简单描述
    chemical_info = models.TextField(max_length=128, null=True)
    # 化学试剂库存量
    chemical_quantity = models.FloatField(null=False)
    # 化学试剂库存量单位，暂时简化
    # chemical_quantity_unit = models.CharField(max_length=32, null=False)
    def __str__(self):
        return self.chemical_name


# 操作员库
class Operator(models.Model):
    # 操作员ID
    operator_id = models.AutoField(primary_key=True, null=False)
    # 操作员名字
    operator_name = models.CharField(max_length=32, null=False)
    def __str__(self):
        return self.operator_name

# 实验库
class Experiment(models.Model):
    # 实验ID
    experiment_id = models.AutoField(primary_key=True, null=False)
    # 实验创建时间
    experiment_create_datetime = models.DateTimeField(null=False)
    # 实验预约时间
    experiment_scheduled_datetime = models.DateField(null=False)
    # 实验操作员，对应的操作员库当中的操作员，是一个foreign key
    experiment_operator = models.ForeignKey(Operator, on_delete=models.CASCADE, null=False)
    def __str__(self):
        return self.experiment_id

# 任务库
class Task(models.Model):
    # 任务ID
    task_id = models.AutoField(primary_key=True, null=False)
    # 任务时长
    task_duration = models.FloatField(null=True)
    # 隶属的实验ID
    task_parent_experiment_id = models.ForeignKey(Experiment, on_delete=models.CASCADE, null=False)
    def __str__(self):
        return self.task_id

# 管理事件库
class Management(models.Model):
    # 管理事件ID
    management_id = models.AutoField(primary_key=True, null=False)
    # 管理事件创建时间
    management_created_datetime = models.DateField(null=False)
    # 管理事件管理员
    management_operator = models.ForeignKey(Operator, on_delete=models.CASCADE, null=False)
    def __str__(self):
        return self.management_id


# 化学试剂操作事件库
class Chemical_Operations(models.Model):
    # 操作事件ID
    chemical_operation_id = models.AutoField(primary_key=True, null=False)
    # 操作事件发生时间
    chemical_operation_datetime = models.DateTimeField(null=False)
    # 操作剂量
    chemical_operation_quantitiy = models.FloatField(null=False)
    # 操作属性是否是增加
    chemical_operation_is_add = models.BooleanField(null=False)
    # 操作化学品ID
    chemical_operation_chemical_id = models.ForeignKey(Chemical,on_delete=models.CASCADE, null=False)
    # 操作化学品操作员ID
    chemical_operation_operator_id = models.ForeignKey(Operator,on_delete=models.CASCADE, null=False)
    # 操作隶属任务
    chemical_operation_parent_task = models.ForeignKey(Task, on_delete=models.CASCADE, default='')
    # 操作隶属管理事件
    chemical_operation_parent_management = models.ForeignKey(Management, on_delete=models.CASCADE, default='')
    def __str__(self):
        return self.chemical_operation_id

