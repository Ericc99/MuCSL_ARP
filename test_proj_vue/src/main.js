import { createApp } from 'vue'
import App from './App.vue'
import router from './router'
// 从文件系统导入store
import store from './store'

// 导入axios框架，将Vue的路由挂载到Django的端口去以获取后端数据
import axios from 'axios'
axios.defaults.baseURL = 'https://127.0.0.1:8000'

const app = createApp(App)
// 声明使用store
app.use(store)
// 声明使用axios
app.use(router, axios)
app.mount('#app')
