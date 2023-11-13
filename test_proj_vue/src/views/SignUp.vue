<template>
    <div class="page-signup">
        <div class="columns">
            <div class="column is-4 is-offset-4">
                <h1 class="title">Sign Up</h1>
                <!-- @submit.prevent 意思是说在提交的时候不刷新页面，这样就可以实现JS动态刷新页面信息 -->
                <!-- 其后等于号里面写的是在submit的时候调用的函数 -->
                <form @submit.prevent="submitForm">
                    <label>E-mail</label>
                    <div class="field has-addons">
                        <div class="control">
                            <input type="text" name="username" class="input" v-model="username">
                        </div>
                        <p class="control">
                            <a class="button is-static">
                                @connect.ust.hk
                            </a>
                        </p>
                    </div>
                    <div class="field">
                        <label>Password</label>
                        <div class="control">
                            <input type="password" name="password" class="input" v-model="password">
                        </div>
                    </div>
                    <!-- 如果存在输入error则会显示输入内容不合法 -->
                    <div class="notification is-danger" v-if="errors.length">
                        <!-- 读取所有error并且显示出来 -->
                        <p v-for="error in errors" v-bind:key="error">
                            {{ error }}
                        </p>
                    </div>
                    <div field="control">
                        <button class="button is-success">Sign up</button>
                    </div>
                </form>
            </div>
        </div>
    </div>
</template>

<script>
import axios from 'axios'

export default{
    name: 'SignUp',
    data() {
        return {
            username: '',
            password: '',
            errors: [],
        }
    },
    methods:{
        submitForm(e){
            const formData = {
                username: this.username,
                password: this.password
            }

            // 通过Axios向Django跨域发送信息
            axios
            .post('/api/users/', formData)
            // 如果有response，并且没有error则会转到Login界面
            .then(response => {
                console.log(response)
                this.$route.push('/log-in')
            })
            // 捕捉系统层面的Error或者是来自Django的Error Message
            .catch(error =>{
                // 来自Django的注册Error Response
                if(error.response){
                    // 获取来自Django的Error Response data并且将其push到前端的errors列表之中显示出来
                    for(const property in error.response.data){
                        this.errors.push(`${property}:${error.response.data[property]}`)
                    }
                    console.log(JSON.stringify(error.response.data))
                }
                // 系统层面的错误Error
                else if(error.message){
                    // this.errors.push(`Error:${error.message}`)
                    console.log(JSON.stringify(error.message))
                }
                else{
                    console.log(JSON.stringify(error))
                }
            })
        }
    }
}

</script>