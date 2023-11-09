import { createRouter, createWebHistory } from 'vue-router'
// 引入需要加载的View
import Home from '../views/Home.vue'
import About from '../views/About.vue'
import Login from '../views/Login.vue'
import SignUp from '../views/SignUp.vue'
import Dashboard from '../views/dashboard/Dashboard.vue'
import MyAccount from '../views/dashboard/MyAccount.vue'

import store from '../store'

const router = createRouter({
    // 访问地址的历史记录？我也不太清楚
    history: createWebHistory(import.meta.env.BASE_URL),
    routes: [
    {
        path: '/',
        name: 'Home',
        component: Home
    },
    {
        path: '/about',
        name: 'About',
        // 这个的意思大概就是只有在访问的时候才会被加载，否则不会被挂载
        // 如果在上面定义则会被全局加载，我先用全局加载，后面再尝试
        // route level code-splitting
        // this generates a separate chunk (About.[hash].js) for this route
        // which is lazy-loaded when the route is visited.
        // component: () => import('../views/AboutView.vue')
        component: About
    },
    {
        path: '/sign-up',
        name: 'SignUp',
        component: SignUp
    },
    {
        path: '/log-in',
        name: 'Login',
        component: Login
    },
    {
        path: '/dashboard',
        name: 'Dashboard',
        component: Dashboard,
        // 创建一个requireLogin的变量来要求用户登录方可访问
        meta:{
            requiredLogin: true
        }
    },
    {
        path: '/dashboard/my-account',
        name: 'MyAccount',
        component: MyAccount,
        meta:{
            requiredLogin: true
        }
    }
  ]
})

// 检测如果想要前往的地方是一个带有requireLogin meta标签的页面，但是当前store显示没有isAuthenticated状态
// 则会被redirect到/log-in
// 否则正常进行redirect
router.beforeEach((to, from, next) => {
    if(to.matched.some(record => record.meta.requiredLogin) && !store.state.isAuthenticated){
        next('/log-in')
    }
    else{
        next()
    }
})

export default router
