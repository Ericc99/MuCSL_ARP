<template>
    <div class="page-chemical">
        <nav class="breadcrumb" aria-label="breadcrumbs">
            <ul>
                <li><router-link to="/dashboard">Dashboard</router-link></li>
                <li><router-link to="/dashboard/chemicals">Chemicals</router-link></li>
                <!-- 这里的chemical不是下面的本地变量chemical而是一个router生成的东西 -->
                <li class="is-active"><router-link :to="{ name: 'Chemical', params: { id: chemical.chemical_id }}" aria-current="true">{{ this.chemical.name }}</router-link></li>
            </ul>
        </nav>
        <div class="column is-12">
            <h2 class="subtitle">Chemical Details</h2>

            <p><strong>{{ chemical.name }}</strong></p>

            <p>Chemical ID: {{ chemical.id }}</p>
            <p>Stocking Quantity: {{ chemical.quantity }}</p>
            <p v-if="chemical.info">Chemical Info: {{ chemical.info }}</p>
        </div>

        <div class="buttons">
            <router-link :to="{ name: 'EditChemical', params: { chemical_id: this.$route.params.chemical_id }}" class="button is-light">Edit</router-link>
            <router-link :to="{ name: 'DropChemical', params: { chemical_id: this.$route.params.chemical_id }}" class="button is-danger">Delete</router-link>
        </div>
    </div>
</template>

<script>
import axios from 'axios'

export default {
    name: 'Chemical',
    data () {
        return {
            chemical: {
                id: '',
                name: '',
                quantity: 0,
                info: ''
            }
        }
    },
    mounted() {
        this.getClient()
    },
    methods: {
        getClient() {
            this.chemical.id = this.$route.params.chemical_id

            axios
                .get(`/api/chemicals/${this.chemical.id}`)
                .then(response => {
                    this.chemical.name = response.data.chemical_name
                    this.chemical.quantity = response.data.chemical_quantity
                    this.chemical.info = response.data.chemical_info
                    // console.log(this.chemical)
                })
                .catch(error => {
                    console.log(JSON.stringify(error))
                })
        }
    },
    computed: {
        // unpaidInvoices() {
        //     return this.client.invoices.filter(invoice => invoice.is_paid === false)
        // },
        // paidInvoices() {
        //     return this.client.invoices.filter(invoice => invoice.is_paid === true)
        // }
    }
}
</script>
