export default defineNuxtConfig({
  compatibilityDate: '2025-01-01',

  modules: [
    '@nuxt/content',
  ],

  app: {
    head: {
      title: 'GRAFT — Generic Remote Actuator Firmware Transport',
      meta: [
        { name: 'description', content: 'Open binary protocol for transport-agnostic, capability-first hardware control. Replaces Firmata, Johnny-Five, and Blynk.' },
        { name: 'theme-color', content: '#060608' },
      ],
      link: [
        { rel: 'preconnect', href: 'https://fonts.googleapis.com' },
        { rel: 'preconnect', href: 'https://fonts.gstatic.com', crossorigin: '' },
        { rel: 'stylesheet', href: 'https://fonts.googleapis.com/css2?family=Space+Mono:wght@400;700&family=JetBrains+Mono:wght@400;600;700&family=Oswald:wght@700&display=swap' },
      ],
    },
  },

  css: ['~/assets/css/graft-theme.css'],
})
