<template>
    <div class="page-dashboard">
        <nav class="breadcrumb" aria-label="breadcrumbs">
            <ul>
                <li class="is-active"><router-link to="/dashboard">Dashboard</router-link></li>            </ul>
        </nav>
        <div class="column is-multiline">
            <div class="column is-12">
                <h1 class="title">Dashboard</h1>
                <router-link to="/dashboard/chemicals" class="button is-light"><strong>View Avaliable Chemicals</strong></router-link>
            </div>
        </div>
    </div>
</template>

<script>
export default{
    name: 'Dashboard'
}
</script>