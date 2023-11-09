# tutorial_proj

## General Information

> tutorial_proj repository is designed for learning purpose targeting frontend-backend seperated web application

> Great thanks to Stein for the video tutorial and code instructions

## Learning Notes

### [Ver 0] Initial Commit

To init a vue project

``` bash
# 创建新项目
npm create vue@latest
# 安装相关依赖
npm install
# 跑Dev branch的代码
npm run dev
```

Installation of necessary dependencies

``` bash
npm install vuex --save
npm install axios --save
npm install bulma --save
npm install -D sass
```

Init the Django project

```bash
# Start a project
django-admin startproject test_proj_django
# Start an app
python3 manage.py startapp smart_lab
```

Installation of pip packages

``` bash
pip install djoser
pip install django-rest-framework
pip install django-cors-headers
```

### [Ver 1] Project Config and Basic Views

#### Django Modifications

- Updated test_proj_django/settings.py to introduce the utilities and middlerware we need
- Installed Mysql dependency as intructed [here](https://pypi.org/project/mysqlclient/)
- Updated database relative settings in test_proj_django/settings.py to modify the database from sqlite to mysql
- Performed migration with following commands

```bash
python3 manage.py makemigrations
python3 manage.py migrate
```


#### Vue Modifications

- Updated store/index.js file to establish user cookie system
- Updated App.vue, established the basic page layout and imported necessary dependencies
- Updated router/index.js file to update navigation and page info
- Updated Home.vue and About.vue page

## Major References

1. [Django and Vue3 Tutorial in Designing a Invoice App](https://www.youtube.com/watch?v=ktplOX4bz0M&t=1139s)

2. [Django Official Guide](https://docs.djangoproject.com/en/4.2/)

## Commit History

> 2023.11.09 [Ver 0] Initial Commit
> 2023.11.09 [Ver 1] Project Config and Basic Views