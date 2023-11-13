<template>
    <div class="page-chemicals">
        <nav class="breadcrumb" aria-label="breadcrumbs">
            <ul>
                <li><router-link to="/dashboard">Dashboard</router-link></li>
                <li class="is-active"><router-link to="/dashboard/chemicals" aria-current="true">Chemicals</router-link></li>
            </ul>
        </nav>

        <div class="columns is-multiline">
            <div class="column is-12">
                <h1 class="title">Chemicals</h1>

                <router-link :to="{ name: 'AddChemical' }" class="button is-light mt-4">Add Chemical</router-link>
            </div>

            <div
                class="column is-half"
                v-for="chemical in chemicals"
                v-bind:key="chemical.chemical_id"
            >
                <div class="box">
                    <p class="is-size-5 mb-2"><strong>{{ chemical.chemical_name }}</strong> - ID: {{ chemical.chemical_id }} Quantity: {{ chemical.chemical_quantity }}</p>
                    <router-link :to="{ name: 'Chemical', params: { chemical_id: chemical.chemical_id }}" class="button is-light">Details</router-link>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import axios from 'axios'

export default {
    name: 'Chemicals',
    data() {
        return {
            chemicals: []
        }
    },
    mounted() {
        this.getChemicals()
    },
    methods: {
        getChemicals() {
            axios
                .get('/api/chemicals/')
                .then(response => {
                    // console.log(response)
                    for (let i = 0; i < response.data.length; i++) {
                        this.chemicals.push(response.data[i])
                    }
                })
                .catch(error => {
                    console.log(JSON.stringify(error))
                })
        }
    }
}
</script>