<template>
    <div class="page-my-account">
        <h1 class="title">My account</h1>
        <button @click="logout()" class="button is-danger">Log out</button>
    </div>
</template>

<script>
import axios from 'axios'

export default{
    name: 'MyAccount',
    mounted(){
        this.getToken()
    },
    methods:{
        logout(){
            // console.log(axios.defaults.headers.common['Authorization'])
            axios
                .post('/api/token/logout/')
                .then(response => {
                    axios.defaults.headers.common['Autorization'] = ''
                    localStorage.removeItem('token')
                    this.$store.commit('removeToken')
                    this.$router.push('/')
                })
                .catch(error => {
                    if(error.response){
                        console.log(JSON.stringify(error.response.data))
                    }
                    else if(error.message){
                        console.log(JSON.stringify(error.message))
                    }
                    else{
                        console.log(JSON.stringify(error))
                    }
                })
        },
        getToken(e){
            this.$store.token = localStorage.token
        }
    }
}
</script>