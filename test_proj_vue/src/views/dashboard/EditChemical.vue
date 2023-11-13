<template>
    <div class="page-add-chemical">
        <h1 class="title"> Edit Chemical</h1>
        <div class="column is-6">
            <p>Chemical ID: {{ this.chemical_id }}</p>
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
    name: 'EditChemical',
    data(){
        return{
            chemical_id: '',
            chemical: {
                chemical_name: '',
                chemical_quantity: '',
                chemical_info : ''
            }
        }
    },
    mounted(){
        this.getClient()
    },
    methods: {
        getClient() {
            this.chemical_id = this.$route.params.chemical_id

            axios
                .get(`/api/chemicals/${this.chemical_id}`)
                .then(response => {
                    this.chemical.chemical_name = response.data.chemical_name
                    this.chemical.chemical_quantity = response.data.chemical_quantity
                    this.chemical.chemical_info = response.data.chemical_info
                })
                .catch(error => {
                    console.log(JSON.stringify(error))
                })
        },
        submitForm(e){
            axios
                // 注意这个不是post了用的是patch
                .patch(`/api/chemicals/${this.chemical_id}/`, this.chemical)
                .then(response => {
                    this.$router.push(`/dashboard/chemicals/${this.chemical_id}`)
                })
                .catch(error => {
                    console.log(JSON.stringify(error))
                })
        }
    }
}
</script>