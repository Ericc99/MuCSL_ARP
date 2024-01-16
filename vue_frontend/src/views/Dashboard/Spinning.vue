<template>
    <h1 class="title">Spinning Control</h1>
    <div class="columns">
        <div class="column is-5">
            <div class="field">
                <p class="title is-4">
                    Motor List
                </p>
                <div class="field">
                    <table class="table">
                        <thead>
                            <tr>
                                <th>ID</th>
                                <th>Name</th>
                                <th>Avaliability</th>
                                <th>Description</th>
                            </tr>
                        </thead>
                        <tbody v-for="motor in this.motors">
                            <tr>
                                <th>{{ motor['id'] }}</th>
                                <td>{{ motor['name'] }}</td>
                                <td>{{ motor['avaliable'] }}</td>
                                <td>{{ motor['description'] }}</td>
                            </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
        <div class="column is-5">
            <div class="field">
                <p class="title is-4">
                    Registration
                </p>
                <div class="field">
                    <label>
                        Motor Selection
                    </label>
                    <div class="control">
                        <div class="select">
                            <select v-model="this.motor_selected">
                                <option v-for="motor in this.motors">
                                    {{ motor['name'] }}
                                </option>
                            </select>
                        </div>
                    </div>
                </div>
                <div class="field">
                    <label>
                        Scheduled Time
                    </label>
                    <VueDatePicker v-model="date"/>
                </div>
                <div class="field">
                    <label>
                        Spinning Speed
                    </label>
                    <input type="number" class="input" v-model="this.speed">
                </div>
                <div class="field">
                    <label>
                        Spinning Time(s)
                    </label>
                    <input type="number" class="input" v-model="this.duration">
                </div>
                <div class="notification is-danger" v-if="this.errors.length">
                    <p v-for="error in this.errors" v-bind:key="error">
                        {{ error }}
                    </p>
                </div>
                <div class="buttons">
                    <button class="button is-success" @click="this.submit">Submit</button>
                </div>
            </div>
        </div>
    </div>
    <div class="columns">
        <div class="column is-5">
            <table class="table">
                <thead>
                    <tr>
                        <th>ID</th>
                        <th>Motor_Name</th>
                        <th>Scheduled Time</th>
                        <th>Spinning Speed</th>
                        <th>Spinning Time</th>
                    </tr>
                </thead>
                <tbody v-for="record in this.records">
                    <tr>
                        <th>{{ record['id'] }}</th>
                        <td>{{ record['motor_name'] }}</td>
                        <td>{{ record['scheduled_time'] }}</td>
                        <td>{{ record['motor_speed'] }}</td>
                        <td>{{ record['duration_sec'] }}</td>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
</template>

<script>
import axios from 'axios';
import { ref } from 'vue';
import VueDatePicker from '@vuepic/vue-datepicker';
import '@vuepic/vue-datepicker/dist/main.css';

export default{
    mounted(){
        this.getMotors()
        this.getRecords()
    },
    setup() {
        const date = ref();

        return {
        date
        }
    },
    data(){
        return{
            motors: [],
            motor_selected: '',
            speed: 0,
            duration: 0,
            records: [],
            errors: [],
        }
    },
    methods:{
        getMotors(e){
            const data = {
                'token': this.$store.state.token
            }
            axios
                .post('/api/get_motors/', data)
                .then(respone => {
                    this.motors = respone.data.motor_list
                    this.motor_selected = this.motors[0]['name']
                })
        },
        getRecords(e){
            const data = {
                'token': this.$store.state.token,
                data: null
            }
            axios
                .post('/api/spinning/', data)
                .then(response => {
                    this.records = response.data.record_list
                    console.log(this.records)
                })
        },
        submit(e){
            const data = {
                'token': this.$store.state.token,
                data: 
                {
                    'motor_name': this.motor_selected,
                    'scheduled_time': this.datetime_formatter(),
                    'motor_speed': this.speed,
                    'duration_sec': this.duration
                }
            }
            axios
                .post('/api/spinning/', data)
                .then(response => {
                    this.getRecords()
                })
                .catch(error =>{
                    if(error.response){
                        for(const property in error.response.data){
                            this.errors.push(`${property}: ${error.response.data[property]}`)
                        }
                    }
                    else if(error.message){
                        this.errors.push(`Error:${error.message}`)
                    }
                    else{
                        console.log(JSON.stringify(error))
                    }
                })
        },
        datetime_formatter(){
            const data = {
                year: this.date.getFullYear(),
                month: this.date.getMonth() + 1,
                date: this.date.getDate(),
                hours: this.date.getHours(),
                minutes: this.date.getMinutes(),
                seconds: this.date.getSeconds(),
            }
            data.month = data.month >= 10 ? data.month : "0" + data.month;
            data.date = data.date >= 10 ? data.date : "0" + data.date;
            data.hours = data.hours >= 10 ? data.hours : "0" + data.hours; 
            data.minutes = data.minutes >= 10 ? data.minutes : "0" + data.minutes;
            data.seconds = data.seconds >= 10 ? data.seconds : "0" + data.seconds;
            // Django Time Format: "%Y-%m-%dT%H:%M:%S"
            const result = 
                data.year + '-' + 
                data.month + '-' + 
                data.date + 'T' + 
                data.hours  + ':' + 
                data.minutes  + ':' + 
                data.seconds
            return result
        }
    },
    components: { VueDatePicker }
}

</script>