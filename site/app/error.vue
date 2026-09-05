<template>
  <div class="err">
    <div class="err-inner">
      <ConduytMark :size="96" />
      <div class="code">{{ error?.statusCode || 500 }}</div>
      <h1 class="title">{{ headline }}</h1>
      <p class="body">{{ blurb }}</p>
      <div class="ferrule"><i /><s /><em>43 44 {{ String(error?.statusCode || 500).padStart(4, '0') }}</em><s /></div>
      <div class="cta">
        <button class="btn btn-primary" @click="go('/')">Back to the start</button>
        <button class="btn" @click="go('/docs')">Read the docs</button>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
/**
 * Without this, a bad URL falls through to Nuxt's stock navy error page and
 * the one route a visitor reaches by accident is the one route that is not
 * on the design system.
 */
const props = defineProps<{ error?: { statusCode?: number, statusMessage?: string } }>()

const isMissing = computed(() => props.error?.statusCode === 404)
const headline = computed(() => (isMissing.value ? 'No such page' : 'Something broke'))
const blurb = computed(() =>
  isMissing.value
    ? 'That address does not lead anywhere. The link may be old, or the page may have moved.'
    : 'The server could not render this page. Nothing on your board was touched.',
)

// Navigating away from an error page needs the error cleared first.
const go = (to: string) => clearError({ redirect: to })

useHead({ title: computed(() => `${props.error?.statusCode || 500} - CONDUYT`) })
</script>

<style scoped>
.err {
  position: relative;
  z-index: 1;
  min-height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: var(--sp-7) var(--gutter);
}

.err-inner {
  display: flex;
  flex-direction: column;
  align-items: center;
  text-align: center;
  max-width: 46ch;
}

.code {
  font-family: var(--face-mono);
  font-size: var(--t-label);
  font-weight: 800;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  background: var(--accent-fill);
  color: var(--accent-on);
  padding: 5px 9px;
  line-height: 1;
  margin: var(--sp-5) 0 var(--sp-4);
}

.title {
  font-family: var(--face-display);
  font-weight: 400;
  font-size: clamp(30px, 5vw, 48px);
  text-transform: uppercase;
  letter-spacing: var(--track-tight);
  line-height: .94;
  margin: 0 0 var(--sp-4);
}

.body {
  color: var(--ash);
  margin: 0;
}

.ferrule { display: flex; align-items: center; margin: var(--sp-6) 0; align-self: stretch; }
.ferrule i { width: var(--wall); height: 14px; background: var(--mint); flex: none; }
.ferrule s { flex: 1; height: var(--rail); background: var(--gasket); }
.ferrule::after { content: ''; width: var(--wall); height: 14px; background: var(--gasket); flex: none; }
.ferrule em {
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-style: normal;
  color: var(--ghost);
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  padding: 0 var(--sp-3);
}

.cta { display: flex; gap: var(--sp-3); flex-wrap: wrap; justify-content: center; }

.btn {
  display: inline-flex;
  align-items: center;
  gap: var(--sp-2);
  font-family: var(--face-mono);
  font-size: var(--t-meta);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  padding: 13px 20px;
  border: var(--wall) solid var(--gasket);
  border-radius: var(--bend);
  background: var(--conduit);
  color: var(--chalk);
  cursor: pointer;
  box-shadow: var(--block-sm);
  transition: transform var(--snap), box-shadow var(--snap), border-color var(--snap);
}
.btn:hover { transform: translate(-2px, -2px); box-shadow: var(--block-lg); border-color: var(--shim); }
.btn:active { transform: translate(1px, 1px); box-shadow: 1px 1px 0 var(--chalk); }
.btn-primary { background: var(--accent-fill); color: var(--accent-on); border-color: var(--accent-fill); }
.btn-primary:hover { border-color: var(--accent-fill); }
</style>
