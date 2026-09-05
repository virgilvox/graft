<template>
  <header class="site-header">
    <div class="header-inner">
      <div class="header-left">
        <button v-if="showHamburger" class="hamburger" aria-label="Toggle navigation" @click="mobileNav.toggle()">
          <AppIcon name="menu" :size="18" />
        </button>
        <NuxtLink to="/" class="brand">
          <ConduytMark variant="compact" :size="26" decorative />
          <span class="wordmark">CON<span class="wm-accent">DUYT</span></span>
        </NuxtLink>
      </div>
      <nav class="header-nav">
        <NuxtLink to="/docs" class="header-link">Docs</NuxtLink>
        <NuxtLink to="/playground" class="header-link">Playground</NuxtLink>
        <a href="https://github.com/virgilvox/conduyt" class="header-link header-link-out" target="_blank" rel="noopener" aria-label="GitHub">
          <AppIcon name="github" :size="13" />
          <span>GitHub</span>
        </a>
        <ColorModeToggle />
      </nav>
    </div>
  </header>
</template>

<script setup lang="ts">
import { useMobileNav } from '~/composables/useMobileNav'

const mobileNav = useMobileNav()
const route = useRoute()
const showHamburger = computed(() => route.path.startsWith('/docs'))
</script>

<style scoped>
.site-header {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  height: var(--header-h);
  background: var(--conduit);
  border-bottom: var(--wall) solid var(--gasket);
  z-index: 200;
}

.header-inner {
  display: flex;
  align-items: center;
  justify-content: space-between;
  height: 100%;
  padding: 0 var(--sp-5);
  max-width: 100%;
}

.header-left {
  display: flex;
  align-items: center;
  gap: var(--sp-3);
}

.hamburger {
  display: none;
  align-items: center;
  justify-content: center;
  width: 34px;
  height: 34px;
  background: var(--raceway);
  border: var(--wall) solid var(--gasket);
  border-radius: var(--bend);
  color: var(--chalk);
  cursor: pointer;
  transition: border-color var(--snap), color var(--snap);
}
.hamburger:hover { border-color: var(--mint); color: var(--mint-ink); }

.brand {
  display: flex;
  align-items: center;
  gap: var(--sp-3);
  color: var(--chalk);
  text-decoration: none;
}
.brand:hover { text-decoration: none; }

.wordmark {
  font-family: var(--face-display);
  font-size: 17px;
  text-transform: uppercase;
  letter-spacing: .04em;
  line-height: 1;
}
.wordmark .wm-accent { color: var(--mint-ink); }

.header-nav {
  display: flex;
  align-items: center;
  gap: var(--sp-1);
}

.header-link {
  display: inline-flex;
  align-items: center;
  gap: 7px;
  font-family: var(--face-mono);
  font-size: var(--t-meta);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
  text-decoration: none;
  padding: 8px 11px;
  border: var(--wall) solid transparent;
  transition: color var(--snap), border-color var(--snap);
}
.header-link:hover,
.header-link.router-link-active {
  color: var(--chalk);
  border-color: var(--gasket);
  text-decoration: none;
}
.header-link.router-link-active { color: var(--mint-ink); border-color: var(--mint-edge); }

@media (max-width: 900px) {
  .hamburger { display: flex; }
}

@media (max-width: 620px) {
  .header-inner { padding: 0 var(--sp-4); }
  .header-link-out span { display: none; }
  .header-link { padding: 8px; letter-spacing: .12em; }
}
</style>
