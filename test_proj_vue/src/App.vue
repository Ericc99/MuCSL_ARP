<template>
    <div id="wrapper">
        <!-- 导航栏 -->
        <nav class="navbar is-dark">
            <!-- 导航栏左侧标题 -->
            <div class="navbar-brand">
                <router-link to="/" class="navbar-item">
                    <strong>Smart Lab @ HKUST</strong>
                </router-link>
            </div>
            <!-- 导航栏正文内容 -->
            <div class="navbar-menu">
                <!-- 导航栏右侧内容 -->
                <div class="navbar-end">
                    <!-- 调用Cookie内容检测是否已登陆 -->
                    <template v-if="$store.state.isAuthenticated">
                        <router-link to="/dashboard" class="navbar-item">Dashboard</router-link>
                    </template>
                    <template v-else>
                        <router-link to="/" class="navbar-item">Home</router-link>
                    </template>
                    <div class="navbar-item">
                        <div class="buttons">
                            <router-link to="/sign-up" class="button is-success">
                                <strong>Sign up</strong>
                            </router-link>
                            <router-link to="/log-in" class="button is-light">Log in</router-link>
                        </div>
                    </div>
                </div>
            </div>
        </nav>

        <!-- 正文部分 -->
        <section class="section">
            <!-- router文件当中定义加载的其他页面内容 -->
            <router-view/>
        </section>

        <!-- 页脚部分 -->
        <footer class="footer">
            <p class="has-text-centered">Copyright (c) 2023 @ HKUST Smart Lab</p>
        </footer>

    </div>
</template>

<script>
// 导入axios引导包
import axios from 'axios'

export default{
    name: 'App',
    beforeCreate(){
        // 只要加上$都是在引用外部变量
        this.$store.commit('initializeStore')
        const token = this.$store.state.token
        if(token){
            axios.defaults.headers.common['Authorization'] = 'Token' + token
            console.log('Token Detected: ' + token)
        }
        else{
            axios.defaults.headers.common['Authorization'] = ''
            console.log('Token Not Detected.')
        }
    }
}

</script>

<!-- 引入bulma标准css文档 -->
<style lang="scss">
@import '../node_modules/bulma/css/bulma.css'
</style>