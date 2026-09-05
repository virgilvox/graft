<template>
  <div class="docs-layout">
    <!-- Mobile backdrop -->
    <div v-if="mobileNav.isOpen.value" class="sidebar-backdrop" @click="mobileNav.close()" />

    <aside class="docs-sidebar" :class="{ open: mobileNav.isOpen.value }">
      <div class="sidebar-header">
        <NuxtLink to="/" class="sidebar-logo">
          <ConduytMark variant="compact" :size="22" decorative />
          <span>CON<span class="wm-accent">DUYT</span></span>
        </NuxtLink>
        <div class="sidebar-version">v1.0</div>
      </div>

      <nav class="sidebar-nav">
        <div class="nav-group" v-for="group in navGroups" :key="group.title">
          <div class="nav-group-title">{{ group.title }}</div>
          <NuxtLink
            v-for="item in group.items"
            :key="item.path"
            :to="item.path"
            class="nav-link"
            @click="mobileNav.close()"
          >
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
import { useMobileNav } from '~/composables/useMobileNav'

const mobileNav = useMobileNav()

const navGroups = [
  {
    title: 'Getting Started',
    items: [
      { path: '/docs', title: 'Overview' },
      { path: '/docs/getting-started/quickstart-playground', title: 'Quick Start: Playground' },
      { path: '/docs/getting-started/quickstart-arduino-ide', title: 'Quick Start: Arduino IDE' },
      { path: '/docs/tutorials/first-blink', title: 'Quick Start: PlatformIO' },
    ],
  },
  {
    title: 'Tutorials',
    items: [
      { path: '/docs/tutorials/what-is-conduyt', title: 'What is Conduyt?' },
      { path: '/docs/tutorials/sensor-dashboard', title: 'Sensor Dashboard' },
    ],
  },
  {
    title: 'Boards',
    items: [
      { path: '/docs/boards/arduino-uno-r3', title: 'Arduino Uno R3' },
      { path: '/docs/boards/arduino-uno-r4', title: 'Arduino Uno R4 Minima' },
      { path: '/docs/boards/arduino-uno-r4-wifi', title: 'Arduino Uno R4 WiFi' },
      { path: '/docs/boards/arduino-mega-2560', title: 'Arduino Mega 2560' },
      { path: '/docs/boards/arduino-nano', title: 'Arduino Nano' },
      { path: '/docs/boards/arduino-nano-every', title: 'Arduino Nano Every' },
      { path: '/docs/boards/arduino-nano-esp32', title: 'Arduino Nano ESP32' },
      { path: '/docs/boards/arduino-leonardo', title: 'Arduino Leonardo' },
      { path: '/docs/boards/esp32', title: 'ESP32' },
      { path: '/docs/boards/esp32-s2', title: 'ESP32-S2' },
      { path: '/docs/boards/esp32-s3', title: 'ESP32-S3' },
      { path: '/docs/boards/esp32-c3', title: 'ESP32-C3' },
      { path: '/docs/boards/nodemcu-v2', title: 'NodeMCU v2 (ESP8266)' },
      { path: '/docs/boards/rp2040-pico', title: 'Raspberry Pi Pico' },
      { path: '/docs/boards/nrf52840-dk', title: 'nRF52840 DK' },
      { path: '/docs/boards/teensy-3-6', title: 'Teensy 3.6' },
      { path: '/docs/boards/teensy-4-0', title: 'Teensy 4.0' },
      { path: '/docs/boards/teensy-4-1', title: 'Teensy 4.1' },
    ],
  },
  {
    title: 'Modules',
    items: [
      { path: '/docs/modules/servo', title: 'Servo' },
      { path: '/docs/modules/neopixel', title: 'NeoPixel' },
      { path: '/docs/modules/oled', title: 'OLED (SSD1306)' },
      { path: '/docs/modules/dht', title: 'DHT (Temperature)' },
      { path: '/docs/modules/encoder', title: 'Encoder' },
      { path: '/docs/modules/stepper', title: 'Stepper' },
      { path: '/docs/modules/pid', title: 'PID Controller' },
      { path: '/docs/modules/i2c-passthrough', title: 'I2C Passthrough' },
    ],
  },
  {
    title: 'How-To Guides',
    items: [
      { path: '/docs/how-to/connect-serial', title: 'Connect over Serial' },
      { path: '/docs/how-to/connect-mqtt', title: 'Connect over MQTT' },
      { path: '/docs/how-to/connect-ble', title: 'Connect over BLE' },
      { path: '/docs/how-to/use-datastreams', title: 'Use Datastreams' },
      { path: '/docs/how-to/flash-ota', title: 'Flash OTA' },
      { path: '/docs/how-to/add-module', title: 'Write a Module' },
      { path: '/docs/how-to/broker-setup', title: 'Set Up MQTT Broker' },
      { path: '/docs/how-to/troubleshooting', title: 'Troubleshooting' },
    ],
  },
  {
    title: 'SDKs',
    items: [
      { path: '/docs/sdks/javascript', title: 'JavaScript' },
      { path: '/docs/sdks/python', title: 'Python' },
      { path: '/docs/sdks/go', title: 'Go' },
      { path: '/docs/sdks/rust', title: 'Rust' },
      { path: '/docs/sdks/swift', title: 'Swift' },
      { path: '/docs/sdks/wasm', title: 'WASM (Browser)' },
    ],
  },
  {
    title: 'API Reference',
    items: [
      { path: '/docs/reference/js-api', title: 'JavaScript API' },
      { path: '/docs/reference/python-api', title: 'Python API' },
      { path: '/docs/reference/firmware-api', title: 'Firmware (C++)' },
    ],
  },
  {
    title: 'Reference',
    items: [
      { path: '/docs/reference/packet-structure', title: 'Packet Structure' },
      { path: '/docs/reference/packet-types', title: 'Packet Types' },
      { path: '/docs/reference/hello-resp', title: 'HELLO_RESP' },
      { path: '/docs/reference/error-codes', title: 'Error Codes' },
      { path: '/docs/reference/datastream-types', title: 'Datastream Types' },
    ],
  },
  {
    title: 'Concepts',
    items: [
      { path: '/docs/concepts/why-binary', title: 'Why Binary' },
      { path: '/docs/concepts/transport-architecture', title: 'Transport Architecture' },
      { path: '/docs/concepts/capability-model', title: 'Capability Model' },
    ],
  },
]
</script>

<style scoped>
.docs-layout {
  display: flex;
  min-height: 100vh;
  padding-top: var(--header-h);
}

.docs-sidebar {
  width: 268px;
  position: fixed;
  top: var(--header-h);
  left: 0;
  bottom: 0;
  background: var(--conduit);
  border-right: var(--wall) solid var(--gasket);
  overflow-y: auto;
  z-index: 100;
  display: flex;
  flex-direction: column;
  transition: transform .25s ease;
}

.sidebar-backdrop { display: none; }

.sidebar-header {
  padding: var(--sp-4) var(--sp-5);
  border-bottom: var(--wall) solid var(--gasket);
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: var(--sp-3);
}

.sidebar-logo {
  display: flex;
  align-items: center;
  gap: var(--sp-2);
  font-family: var(--face-display);
  font-size: 15px;
  text-transform: uppercase;
  letter-spacing: .04em;
  color: var(--chalk);
  text-decoration: none;
}
.sidebar-logo:hover { text-decoration: none; }
.sidebar-logo .wm-accent { color: var(--mint-ink); }

.sidebar-version {
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--mint-ink);
  border: var(--rail) solid var(--mint-edge);
  background: var(--mint-wash);
  padding: 3px 7px;
}

.sidebar-nav { padding: var(--sp-3) 0 var(--sp-7); flex: 1; }

.nav-group { margin-bottom: var(--sp-2); }

.nav-group-title {
  font-family: var(--face-mono);
  font-size: var(--t-label);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ghost);
  padding: var(--sp-4) var(--sp-5) var(--sp-1);
}

.nav-link {
  display: block;
  padding: 5px var(--sp-5);
  color: var(--ash);
  text-decoration: none;
  font-size: var(--t-sm);
  border-left: var(--bulkhead) solid transparent;
  transition: color var(--snap), background var(--snap);
}

.nav-link:hover { color: var(--chalk); background: var(--hover-bg); text-decoration: none; }
.nav-link.router-link-active {
  color: var(--mint-ink);
  border-left-color: var(--mint);
  background: var(--mint-wash);
  font-weight: 600;
  text-decoration: none;
}

.docs-main {
  margin-left: 268px;
  flex: 1;
  padding: var(--sp-7) var(--sp-8);
  max-width: 880px;
  position: relative;
  z-index: 1;
}

/* ── prose ───────────────────────────────────────────────────── */
.docs-main :deep(h1) {
  font-family: var(--face-display);
  font-weight: 400;
  font-size: clamp(28px, 3.6vw, 38px);
  letter-spacing: var(--track-tight);
  line-height: 1.05;
  margin-bottom: var(--sp-4);
  color: var(--chalk);
}

.docs-main :deep(h2) {
  font-family: var(--face-body);
  font-size: 21px;
  font-weight: 600;
  letter-spacing: var(--track-tight);
  margin: var(--sp-7) 0 var(--sp-3);
  padding-bottom: var(--sp-2);
  border-bottom: var(--wall) solid var(--gasket);
  color: var(--chalk);
}

.docs-main :deep(h3) {
  font-family: var(--face-body);
  font-size: 16px;
  font-weight: 600;
  letter-spacing: 0;
  color: var(--chalk);
  margin: var(--sp-5) 0 var(--sp-2);
}

.docs-main :deep(h4) {
  font-family: var(--face-mono);
  font-size: var(--t-meta);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
  margin: var(--sp-5) 0 var(--sp-2);
}

.docs-main :deep(p) {
  line-height: 1.78;
  margin-bottom: var(--sp-3);
  color: var(--chalk);
}

/* Same reason: the UA sheet's bold is 700 and Archivo has no 700 face. */
.docs-main :deep(strong),
.docs-main :deep(b) { font-weight: 600; }

.docs-main :deep(a) {
  color: var(--mint-ink);
  border-bottom: var(--rail) solid var(--mint-edge);
  text-decoration: none;
}
.docs-main :deep(a:hover) { border-bottom-color: var(--mint); text-decoration: none; }

/* Nuxt Content wraps heading text in an anchor. It is a heading, not a link. */
.docs-main :deep(h1 a),
.docs-main :deep(h2 a),
.docs-main :deep(h3 a),
.docs-main :deep(h4 a) {
  color: inherit;
  border-bottom: 0;
  text-decoration: none;
}
.docs-main :deep(h2 a:hover),
.docs-main :deep(h3 a:hover),
.docs-main :deep(h4 a:hover) { color: var(--mint-ink); }

.docs-main :deep(pre) {
  background: var(--soot);
  border: var(--wall) solid var(--gasket);
  border-left: var(--bulkhead) solid var(--gasket);
  border-radius: var(--bend);
  padding: var(--sp-4) var(--sp-5);
  overflow-x: auto;
  font-family: var(--face-mono);
  font-size: var(--t-sm);
  line-height: 1.8;
  margin: var(--sp-4) 0;
}

.docs-main :deep(pre code) {
  background: transparent;
  border: none;
  padding: 0;
  font-size: inherit;
  color: inherit;
}

.docs-main :deep(blockquote) {
  border-left: var(--bulkhead) solid var(--mint);
  background: var(--mint-wash);
  padding: var(--sp-3) var(--sp-4);
  margin: var(--sp-4) 0;
}
.docs-main :deep(blockquote p:last-child) { margin-bottom: 0; }

.docs-main :deep(table) {
  width: 100%;
  border-collapse: collapse;
  font-family: var(--face-mono);
  font-size: var(--t-sm);
  margin: var(--sp-4) 0;
  border: var(--wall) solid var(--gasket);
  background: var(--raceway);
}

.docs-main :deep(th) {
  text-align: left;
  padding: 10px var(--sp-4);
  font-size: var(--t-micro);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
  background: var(--conduit);
  border-bottom: var(--wall) solid var(--gasket);
  white-space: nowrap;
}

.docs-main :deep(td) {
  padding: 10px var(--sp-4);
  border-bottom: var(--rail) solid var(--gasket);
  color: var(--chalk);
  overflow-wrap: break-word;
}
.docs-main :deep(tr:last-child td) { border-bottom: 0; }
.docs-main :deep(td code) { font-size: var(--t-sm); }

.docs-main :deep(ul),
.docs-main :deep(ol) {
  padding-left: var(--sp-5);
  margin-bottom: var(--sp-3);
}

.docs-main :deep(li) {
  margin-bottom: var(--sp-1);
  line-height: 1.78;
  color: var(--chalk);
}

.docs-main :deep(hr) {
  border: 0;
  height: var(--rail);
  background: var(--gasket);
  margin: var(--sp-6) 0;
}

.docs-main :deep(img) { max-width: 100%; }

@media (max-width: 900px) {
  .docs-sidebar {
    transform: translateX(-100%);
    z-index: 150;
  }
  .docs-sidebar.open { transform: translateX(0); }
  .sidebar-backdrop {
    display: block;
    position: fixed;
    inset: 0;
    top: var(--header-h);
    background: rgba(0, 0, 0, .55);
    z-index: 140;
  }
  .docs-main { margin-left: 0; padding: var(--sp-6) var(--sp-5); }
  /* Narrow enough that scrolling the table beats squeezing it, even at the
     cost of the table role in some screen readers. */
  .docs-main :deep(table) { display: block; overflow-x: auto; }
}
</style>
