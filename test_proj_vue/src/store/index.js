import { createStore } from 'vuex'

// 创建一个随着用户浏览器加载的Token，我感觉就是cookie
export default createStore({
    // 定义参数
    state:{
        user:{
            username: ''
        },
        isAuthenticated: false,
        token: ''
    },
    // 定义方法
    mutations:{
        initializeStore(state){
            // localStorage为内置函数，获取当前store obj
            // 相当于是在检测是否有登录状态
            if(localStorage.getItem('token')){
                state.token = localStorage.getItem('token')
                state.isAuthenticated = true
            }
            // 否则清空store中参数
            else{
                state.token = ''
                state.isAuthenticated = false
            }
        },
        // token的接口方法
        setToken(state, token){
            state.token = token
            state.isAuthenticated = true
        },
        removeToken(state){
            state.token = ''
            isAuthenticated = false
        }
    },
    // 目前还留空，不知道是干啥的
    actions:{

    },
    modules:{

    }
})
