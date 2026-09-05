<template>
  <NuxtLayout name="docs">
    <ContentRenderer v-if="page" :value="page" />
    <div v-else class="not-found">
      <h1>Page Not Found</h1>
      <p>This documentation page doesn't exist yet.</p>
      <NuxtLink to="/docs">Go to Documentation</NuxtLink>
    </div>
  </NuxtLayout>
</template>

<script setup lang="ts">
const route = useRoute()
const { data: page } = await useAsyncData(
  `docs-${route.path}`,
  () => queryCollection('content').path(route.path).first()
)
</script>

<style scoped>
.not-found {
  text-align: center;
  padding: 80px 0;
}
.not-found h1 {
  font-family: var(--face-display);
  font-weight: 400;
  font-size: clamp(26px, 4vw, 38px);
  text-transform: uppercase;
  /* The display face is drawn tight; +4px tracking fought it. */
  letter-spacing: var(--track-tight);
  margin-bottom: var(--sp-3);
}
.not-found p {
  color: var(--ash);
  margin-bottom: var(--sp-5);
}
</style>
