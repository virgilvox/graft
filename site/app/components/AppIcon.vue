<template>
  <svg
    class="ico"
    :width="size"
    :height="size"
    viewBox="0 0 24 24"
    fill="none"
    stroke="currentColor"
    :stroke-width="strokeWidth"
    stroke-linecap="round"
    stroke-linejoin="round"
    aria-hidden="true"
    focusable="false"
    v-html="PATHS[name] ?? ''"
  />
</template>

<script lang="ts">
export const PATHS: Record<string, string> = {
  bolt: '<polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2" />',
  clock: '<circle cx="12" cy="12" r="9" /><polyline points="12 7 12 12 15.5 14" />',
  link: '<path d="M10 13a5 5 0 0 0 7.54.54l3-3a5 5 0 0 0-7.07-7.07l-1.72 1.71" /><path d="M14 11a5 5 0 0 0-7.54-.54l-3 3a5 5 0 0 0 7.07 7.07l1.71-1.71" />',
  bluetooth: '<polyline points="6.5 6.5 17.5 17.5 12 23 12 1 17.5 6.5 6.5 17.5" />',
  broadcast: '<circle cx="12" cy="12" r="2" /><path d="M16.24 7.76a6 6 0 0 1 0 8.49" /><path d="M7.76 16.25a6 6 0 0 1 0-8.49" /><path d="M19.07 4.93a10 10 0 0 1 0 14.14" /><path d="M4.93 19.07a10 10 0 0 1 0-14.14" />',
  globe: '<circle cx="12" cy="12" r="9" /><line x1="3" y1="12" x2="21" y2="12" /><path d="M12 3a14 14 0 0 1 3.6 9A14 14 0 0 1 12 21a14 14 0 0 1-3.6-9A14 14 0 0 1 12 3z" />',
  server: '<rect x="2.5" y="3" width="19" height="7" /><rect x="2.5" y="14" width="19" height="7" /><line x1="6.5" y1="6.5" x2="6.6" y2="6.5" /><line x1="6.5" y1="17.5" x2="6.6" y2="17.5" />',
  monitor: '<rect x="2.5" y="3.5" width="19" height="13" /><line x1="8" y1="20.5" x2="16" y2="20.5" /><line x1="12" y1="16.5" x2="12" y2="20.5" />',
  cpu: '<rect x="5" y="5" width="14" height="14" /><rect x="9.5" y="9.5" width="5" height="5" /><line x1="9" y1="1.5" x2="9" y2="5" /><line x1="15" y1="1.5" x2="15" y2="5" /><line x1="9" y1="19" x2="9" y2="22.5" /><line x1="15" y1="19" x2="15" y2="22.5" /><line x1="19" y1="9" x2="22.5" y2="9" /><line x1="19" y1="15" x2="22.5" y2="15" /><line x1="1.5" y1="9" x2="5" y2="9" /><line x1="1.5" y1="15" x2="5" y2="15" />',
  share: '<circle cx="18" cy="5" r="2.6" /><circle cx="6" cy="12" r="2.6" /><circle cx="18" cy="19" r="2.6" /><line x1="8.3" y1="13.4" x2="15.7" y2="17.7" /><line x1="15.7" y1="6.3" x2="8.3" y2="10.6" />',
  code: '<polyline points="16 18 22 12 16 6" /><polyline points="8 6 2 12 8 18" />',
  chart: '<polyline points="22 7 14 15 10 11 2 19" /><polyline points="16 7 22 7 22 13" />',
  robot: '<rect x="3" y="8" width="18" height="12" /><circle cx="8.5" cy="14" r="1.4" /><circle cx="15.5" cy="14" r="1.4" /><line x1="12" y1="8" x2="12" y2="4.5" /><circle cx="12" cy="3.5" r="1.4" />',
  bulb: '<path d="M9.5 21h5" /><path d="M10 18h4" /><path d="M12 2.5a6 6 0 0 0-3.6 10.8c.5.4.8 1 .9 1.6l.1.6h5.2l.1-.6c.1-.6.4-1.2.9-1.6A6 6 0 0 0 12 2.5z" />',
  book: '<path d="M4 19.5A2.5 2.5 0 0 1 6.5 17H20" /><path d="M6.5 2.5H20v19H6.5A2.5 2.5 0 0 1 4 19V5a2.5 2.5 0 0 1 2.5-2.5z" />',
  pulse: '<polyline points="22 12 18 12 15 21 9 3 6 12 2 12" />',
  home: '<path d="M3.5 9.5 12 2.8l8.5 6.7V20a1.5 1.5 0 0 1-1.5 1.5H5A1.5 1.5 0 0 1 3.5 20z" /><polyline points="9.5 21.5 9.5 13.5 14.5 13.5 14.5 21.5" />',
  download: '<path d="M21 15.5V19a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-3.5" /><polyline points="7.5 10.5 12 15 16.5 10.5" /><line x1="12" y1="15" x2="12" y2="3" />',
  copy: '<rect x="9" y="9" width="12.5" height="12.5" /><path d="M5.5 15H4.5A2 2 0 0 1 2.5 13V4.5a2 2 0 0 1 2-2H13a2 2 0 0 1 2 2v1" />',
  check: '<polyline points="20 6.5 9.5 17.5 4 12" />',
  github: '<path d="M9 19c-4.7 1.4-4.7-2.4-6.6-2.9M15 22v-3.9a3.4 3.4 0 0 0-.9-2.6c3-.3 6.1-1.5 6.1-6.6a5.2 5.2 0 0 0-1.4-3.6 4.8 4.8 0 0 0-.1-3.6s-1.1-.3-3.7 1.4a12.6 12.6 0 0 0-6.6 0C5.8 1 4.7 1.3 4.7 1.3a4.8 4.8 0 0 0-.1 3.6 5.2 5.2 0 0 0-1.4 3.6c0 5.1 3.1 6.3 6.1 6.6a3.4 3.4 0 0 0-.9 2.6V22" />',
  terminal: '<polyline points="4.5 17 10.5 11 4.5 5" /><line x1="12.5" y1="19" x2="20" y2="19" />',
  layers: '<polygon points="12 2.5 22 7.5 12 12.5 2 7.5 12 2.5" /><polyline points="2 16.5 12 21.5 22 16.5" /><polyline points="2 12 12 17 22 12" />',
  spec: '<path d="M14 2.5H6.5a2 2 0 0 0-2 2v15a2 2 0 0 0 2 2h11a2 2 0 0 0 2-2V8z" /><polyline points="14 2.5 14 8 19.5 8" /><line x1="8.5" y1="13" x2="15.5" y2="13" /><line x1="8.5" y1="17" x2="13" y2="17" />',
  sun: '<circle cx="12" cy="12" r="4.5" /><line x1="12" y1="1.5" x2="12" y2="3.5" /><line x1="12" y1="20.5" x2="12" y2="22.5" /><line x1="4.2" y1="4.2" x2="5.7" y2="5.7" /><line x1="18.3" y1="18.3" x2="19.8" y2="19.8" /><line x1="1.5" y1="12" x2="3.5" y2="12" /><line x1="20.5" y1="12" x2="22.5" y2="12" /><line x1="4.2" y1="19.8" x2="5.7" y2="18.3" /><line x1="18.3" y1="5.7" x2="19.8" y2="4.2" />',
  moon: '<path d="M21 12.8A9 9 0 1 1 11.2 3 7 7 0 0 0 21 12.8z" />',
  menu: '<line x1="3" y1="6" x2="21" y2="6" /><line x1="3" y1="12" x2="21" y2="12" /><line x1="3" y1="18" x2="21" y2="18" />',
}
</script>

<script setup lang="ts">
/**
 * The house icon set. Hand-kept inline so the site ships no icon font and
 * no third-party stylesheet: a protocol that runs on a 16 MHz chip should
 * not need a CDN to draw a plug socket.
 *
 * 24x24 grid, 1.6 stroke, round caps. Add sparingly.
 */
withDefaults(defineProps<{
  name: string
  size?: number
  strokeWidth?: number
}>(), {
  size: 16,
  strokeWidth: 1.7,
})
</script>


<style scoped>
.ico { display: block; flex: none; }
</style>
