import wasm from 'vite-plugin-wasm'
import topLevelAwait from 'vite-plugin-top-level-await'

export default defineNuxtConfig({
  compatibilityDate: '2025-01-01',

  modules: [
    '@nuxt/content',
    '@nuxtjs/color-mode',
  ],

  colorMode: {
    classSuffix: '',
    // RACEWAY ships on paper stock. Dark is the alternate.
    preference: 'light',
    fallback: 'light',
  },

  content: {
    highlight: {
      theme: {
        default: 'github-light',
        dark: 'github-dark',
      },
      langs: ['javascript', 'typescript', 'python', 'go', 'rust', 'cpp', 'bash', 'json'],
    },
  },

  app: {
    head: {
      title: 'CONDUYT - open binary protocol for hardware control',
      meta: [
        { name: 'description', content: 'Flash the firmware once, then drive any circuit board from JavaScript, Python, Go, Rust or Swift over USB, Bluetooth, WiFi or MQTT. MIT, no cloud account.' },
      ],
      link: [
        { rel: 'preconnect', href: 'https://fonts.googleapis.com' },
        { rel: 'preconnect', href: 'https://fonts.gstatic.com', crossorigin: '' },
        { rel: 'stylesheet', href: 'https://fonts.googleapis.com/css2?family=Archivo+Black&family=Archivo:wght@400;500;600&family=JetBrains+Mono:wght@400;500;700;800&display=swap' },
        { rel: 'icon', type: 'image/svg+xml', href: '/favicon.svg' },
      ],
    },
  },

  css: ['~/assets/css/conduyt-theme.css'],

  vue: {
    compilerOptions: {
      isCustomElement: (tag: string) => tag === 'esp-web-install-button',
    },
  },

  vite: {
    optimizeDeps: {
      exclude: ['conduyt-wasm'],
    },
    // Target esnext so esbuild doesn't try to transpile modern syntax
    // (destructuring, top-level await, etc.) emitted by vite-plugin-top-level-await
    // and the wasm-bindgen JS shim. The playground requires WebSerial / WebUSB
    // anyway — all of those browsers support modern ES well past these features.
    build: {
      target: 'esnext',
    },
    esbuild: {
      target: 'esnext',
    },
    plugins: [
      // wasm-pack bundler-target packages need explicit Vite handling.
      wasm(),
      topLevelAwait(),
    ],
  },
})
