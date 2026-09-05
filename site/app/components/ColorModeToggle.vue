<template>
  <ClientOnly>
    <button
      class="color-mode-toggle"
      :aria-label="`Switch to ${nextMode} stock`"
      :title="`Switch to ${nextMode} stock`"
      @click="toggle"
    >
      <AppIcon :name="colorMode.value === 'dark' ? 'sun' : 'moon'" :size="15" />
      <span class="stock">{{ nextMode }}</span>
    </button>
    <template #fallback>
      <span class="color-mode-toggle" aria-hidden="true">
        <AppIcon name="moon" :size="15" />
        <span class="stock">dark</span>
      </span>
    </template>
  </ClientOnly>
</template>

<script setup lang="ts">
const colorMode = useColorMode()
const nextMode = computed(() => (colorMode.value === 'dark' ? 'paper' : 'dark'))
const toggle = () => {
  colorMode.preference = colorMode.value === 'dark' ? 'light' : 'dark'
}
</script>

<style scoped>
.color-mode-toggle {
  display: inline-flex;
  align-items: center;
  gap: 7px;
  height: 32px;
  padding: 0 10px;
  margin-left: var(--sp-2);
  background: var(--raceway);
  border: var(--wall) solid var(--gasket);
  border-radius: var(--bend);
  color: var(--ash);
  cursor: pointer;
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  transition: color var(--snap), border-color var(--snap);
}
.color-mode-toggle:hover {
  color: var(--mint-ink);
  border-color: var(--mint);
}

@media (max-width: 620px) {
  .stock { display: none; }
  .color-mode-toggle { padding: 0 8px; }
}
</style>
