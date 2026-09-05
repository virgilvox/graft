<template>
  <svg
    v-if="variant === 'compact'"
    class="mark"
    :width="size"
    :height="size"
    viewBox="-8 -8 216 216"
    xmlns="http://www.w3.org/2000/svg"
    :role="decorative ? undefined : 'img'"
    :aria-hidden="decorative ? 'true' : undefined"
    :aria-label="decorative ? undefined : label"
  >
    <path
      d="M 20 60 L 40 40 L 160 40 L 180 60 L 180 140 L 160 160 L 40 160 L 20 140 Z"
      fill="none" stroke="var(--chalk)" stroke-width="18" stroke-linejoin="miter"
    />
    <line x1="50" y1="100" x2="150" y2="100" stroke="var(--mint-ink)" stroke-width="14" />
    <circle cx="100" cy="100" r="20" fill="var(--void)" stroke="var(--chalk)" stroke-width="9" />
    <circle cx="100" cy="100" r="8" fill="var(--mint-ink)" />
  </svg>

  <svg
    v-else
    class="mark"
    :width="size"
    :height="size"
    viewBox="-8 -8 216 216"
    xmlns="http://www.w3.org/2000/svg"
    :role="decorative ? undefined : 'img'"
    :aria-hidden="decorative ? 'true' : undefined"
    :aria-label="decorative ? undefined : label"
  >
    <defs v-if="print">
      <filter :id="filterId" x="-15%" y="-15%" width="130%" height="130%">
        <feTurbulence type="fractalNoise" baseFrequency="0.045" numOctaves="4" result="n" />
        <feDisplacementMap in="SourceGraphic" in2="n" scale="2.2" />
      </filter>
    </defs>
    <g :filter="print ? `url(#${filterId})` : undefined">
      <path
        d="M 24 62 L 42 44 L 158 44 L 176 62 L 176 138 L 158 156 L 42 156 L 24 138 Z"
        fill="none" stroke="var(--chalk)" stroke-width="13" stroke-linejoin="miter"
      />
      <line x1="56" y1="72" x2="144" y2="72" stroke="var(--mint)" stroke-width="7" stroke-linecap="round" />
      <line x1="56" y1="100" x2="144" y2="100" stroke="var(--chalk)" stroke-width="7" stroke-linecap="round" />
      <line x1="56" y1="128" x2="144" y2="128" stroke="var(--mint)" stroke-width="7" stroke-linecap="round" />
      <circle cx="100" cy="100" r="13" fill="var(--void)" stroke="var(--chalk)" stroke-width="5.5" />
      <circle cx="100" cy="100" r="5.5" fill="var(--mint)" />
    </g>
  </svg>
</template>

<script setup lang="ts">
/**
 * The Shell mark. A length of conduit seen end on, chamfered at all four
 * corners, three relay lines running through it, and a hub node for the
 * HELLO handshake: ring is the host, filled centre is the device.
 *
 * Below 32px use `variant="compact"` - one bar, fat hub, no print filter.
 * Pass `decorative` wherever a CONDUYT wordmark already sits beside it, so
 * screen readers do not announce the name twice.
 */
withDefaults(defineProps<{
  size?: number
  variant?: 'full' | 'compact'
  print?: boolean
  label?: string
  decorative?: boolean
}>(), {
  size: 96,
  variant: 'full',
  print: true,
  label: 'CONDUYT',
  decorative: false,
})

// useId keeps the filter id stable across SSR and hydration.
const filterId = `mark-${useId()}`
</script>

<style scoped>
.mark { display: block; flex: none; }
</style>
