import { createRouter, createWebHistory } from 'vue-router'
// 引入需要加载的View
import Home from '../views/Home.vue'
import About from '../views/About.vue'

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
    }
  ]
})

export default router
