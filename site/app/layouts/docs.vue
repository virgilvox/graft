<template>
  <div class="docs-layout">
    <aside class="docs-sidebar">
      <div class="sidebar-header">
        <NuxtLink to="/" class="sidebar-logo">
          <svg width="20" height="20" viewBox="0 0 28 28" fill="none">
            <rect x="2" y="8" width="5" height="12" fill="#00d4aa" opacity="0.9"/>
            <rect x="21" y="8" width="5" height="12" fill="#00d4aa" opacity="0.9"/>
            <rect x="9" y="4" width="10" height="4" fill="#00d4aa" opacity="0.5"/>
            <rect x="9" y="20" width="10" height="4" fill="#00d4aa" opacity="0.5"/>
            <circle cx="14" cy="14" r="2" fill="#00d4aa"/>
          </svg>
          <span>GRAFT</span>
        </NuxtLink>
        <div class="sidebar-version">v0.1</div>
      </div>

      <nav class="sidebar-nav">
        <div class="nav-group" v-for="group in navGroups" :key="group.title">
          <div class="nav-group-title">{{ group.title }}</div>
          <NuxtLink
            v-for="item in group.items"
            :key="item.path"
            :to="item.path"
            class="nav-link"
          >
            <span class="nav-dot" />
            {{ item.title }}
          </NuxtLink>
        </div>
      </nav>
    </aside>

    <main class="docs-main">
      <slot />
    </main>
  </div>
</template>

<script setup lang="ts">
const navGroups = [
  {
    title: 'Getting Started',
    items: [
      { path: '/docs/getting-started/introduction', title: 'Introduction' },
      { path: '/docs/getting-started/quick-start-arduino', title: 'Quick Start: Arduino' },
      { path: '/docs/getting-started/quick-start-js', title: 'Quick Start: JavaScript' },
      { path: '/docs/getting-started/quick-start-python', title: 'Quick Start: Python' },
    ],
  },
  {
    title: 'Protocol',
    items: [
      { path: '/docs/protocol/packet-structure', title: 'Packet Structure' },
      { path: '/docs/protocol/packet-types', title: 'Packet Types' },
      { path: '/docs/protocol/hello-resp', title: 'HELLO_RESP' },
      { path: '/docs/protocol/datastreams', title: 'Datastreams' },
      { path: '/docs/protocol/error-codes', title: 'Error Codes' },
    ],
  },
  {
    title: 'Firmware',
    items: [
      { path: '/docs/firmware/device-setup', title: 'Device Setup' },
      { path: '/docs/firmware/transport-adapters', title: 'Transport Adapters' },
      { path: '/docs/firmware/module-system', title: 'Module System' },
      { path: '/docs/firmware/built-in-modules', title: 'Built-in Modules' },
    ],
  },
  {
    title: 'SDK Guides',
    items: [
      { path: '/docs/sdk-guides/javascript', title: 'JavaScript / TypeScript' },
      { path: '/docs/sdk-guides/python', title: 'Python' },
    ],
  },
  {
    title: 'IoT',
    items: [
      { path: '/docs/iot/mqtt-schema', title: 'MQTT Schema' },
      { path: '/docs/iot/broker-setup', title: 'Broker Setup' },
    ],
  },
]
</script>

<style scoped>
.docs-layout {
  display: flex;
  min-height: 100vh;
}

.docs-sidebar {
  width: 260px;
  position: fixed;
  top: 0; left: 0; bottom: 0;
  background: var(--surface);
  border-right: 1px solid var(--border);
  overflow-y: auto;
  z-index: 100;
  display: flex;
  flex-direction: column;
}

.sidebar-header {
  padding: 20px;
  border-bottom: 1px solid var(--border);
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.sidebar-logo {
  display: flex;
  align-items: center;
  gap: 8px;
  font-family: var(--heading);
  font-size: 16px;
  letter-spacing: 4px;
  color: var(--text-bright);
  text-decoration: none;
}

.sidebar-version {
  font-family: var(--code);
  font-size: 9px;
  color: var(--accent);
  letter-spacing: 1px;
}

.sidebar-nav {
  padding: 12px 0;
  flex: 1;
}

.nav-group { margin-bottom: 8px; }

.nav-group-title {
  font-size: 8px;
  letter-spacing: 3px;
  text-transform: uppercase;
  color: var(--text-dim);
  padding: 12px 20px 4px;
  font-family: var(--mono);
}

.nav-link {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 5px 20px;
  color: var(--text-dim);
  text-decoration: none;
  font-size: 11px;
  letter-spacing: 0.5px;
  border-left: 2px solid transparent;
  transition: color 0.15s, background 0.15s;
}

.nav-link:hover { color: var(--text); background: rgba(255,255,255,0.02); }
.nav-link.router-link-active {
  color: var(--accent);
  border-left-color: var(--accent);
  background: var(--accent-dim);
}

.nav-dot {
  width: 4px; height: 4px;
  border-radius: 50%;
  background: currentColor;
  opacity: 0.5;
  flex-shrink: 0;
}

.docs-main {
  margin-left: 260px;
  flex: 1;
  padding: 48px 64px;
  max-width: 800px;
}

.docs-main :deep(h1) {
  font-family: var(--heading);
  font-size: 28px;
  letter-spacing: 4px;
  margin-bottom: 16px;
}

.docs-main :deep(h2) {
  font-family: var(--heading);
  font-size: 18px;
  letter-spacing: 3px;
  margin: 32px 0 12px;
  padding-bottom: 6px;
  border-bottom: 1px dashed var(--border);
  color: var(--accent);
}

.docs-main :deep(h3) {
  font-family: var(--code);
  font-size: 13px;
  color: var(--text-bright);
  margin: 24px 0 8px;
}

.docs-main :deep(p) {
  line-height: 1.8;
  margin-bottom: 12px;
}

.docs-main :deep(pre) {
  background: #070709;
  border: 1px solid var(--border);
  border-left: 2px solid var(--accent);
  padding: 16px 20px;
  overflow-x: auto;
  font-family: var(--code);
  font-size: 11.5px;
  line-height: 1.65;
  margin: 16px 0;
}

.docs-main :deep(table) {
  width: 100%;
  border-collapse: collapse;
  font-family: var(--code);
  font-size: 11.5px;
  margin: 16px 0;
}

.docs-main :deep(th) {
  text-align: left;
  padding: 8px 16px;
  font-size: 8px;
  letter-spacing: 2px;
  text-transform: uppercase;
  color: var(--text-dim);
  border-bottom: 1px solid var(--border-bright);
}

.docs-main :deep(td) {
  padding: 8px 16px;
  border-bottom: 1px solid var(--border);
}

@media (max-width: 900px) {
  .docs-sidebar { display: none; }
  .docs-main { margin-left: 0; padding: 32px 24px; }
}
</style>
