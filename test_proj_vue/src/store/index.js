import { createStore } from 'vuex'

// 创建一个随着用户浏览器加载的Token，我感觉就是cookie
// Ok 来更正一下，这个不是cookie而是一个本地的属性
// 可以有一个local store和一个session store用来存储各种属性
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
                // console.log('Gotten Item from local store!')
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
            // console.log(state.token)
        },
        removeToken(state){
            state.token = ''
            state.isAuthenticated = false
        }
    },
    // 目前还留空，不知道是干啥的
    actions:{

    },
    modules:{

    }
})
