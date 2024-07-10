<template>
    <h1 class="title">
        WebSocket Test
    </h1>
    <div class="buttons">
        <button class="button is-light" @click="this.wsTest">WS Test</button>
    </div>
    <div class="buttons">
        <button class="button is-light" @click="this.wsTerminate">Manual Shutdown</button>
    </div>
</template>
<script>

export default{
    data(){
        return{
            client: null,
        }
    },
    methods:{
        wsTest(e){
            this.client = new WebSocket('ws:127.0.0.1:8000/websocket/')
            this.client.onopen = function() {
                console.log('Connection Success..')
            }
            this.client.onmessage = this.wsOnMsg
        },
        wsOnMsg(e){
            console.log(e.data)
        },
        wsTerminate(e){
            // Manual close of WS
            // Usually not necessary
            this.client.close()
        }
    }
}
</script>