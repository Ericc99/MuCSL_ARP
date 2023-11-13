<template>
    <div class="page-add-chemical">
        <h1 class="title"> Add Chemical</h1>
        <div class="column is-6">
            <div class="field">
                <label>Name</label>
                <div class="control">
                    <input type="text" name="name" class="input" v-model="chemical.chemical_name" required>
                </div>
            </div>

            <div class="field">
                <label>Quantity</label>
                <div class="control">
                    <input type="number" name="quantity" class="input" v-model="chemical.chemical_quantity" required>
                </div>
            </div>

            <div class="field">
                <label>Information (Intro)</label>
                <div class="control">
                    <input type="text" name="info" class="input" v-model="chemical.chemical_info" required>
                </div>
            </div>

            <div class="field">
                <div class="control">
                    <button class="button is-success" @click="submitForm">Submit</button>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import axios from 'axios'

export default{
    name: 'AddChemical',
    data(){
        return{
            chemical: {
                chemical_info : ''
            }
        }
    },
    methods: {
        submitForm(e){
            axios
                .post('/api/chemicals/', this.chemical)
                .then(response => {
                    this.$router.push('/dashboard/chemicals')
                })
                .catch(error => {
                    console.log(JSON.stringify(error))
                })
        }
    }
}
</script>