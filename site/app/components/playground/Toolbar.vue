<template>
  <div class="toolbar">
    <div class="toolbar-left">
      <button class="tb-btn tb-run" :disabled="running || !serialConnected" @click="$emit('run')" title="Run code">
        <svg width="14" height="14" viewBox="0 0 24 24" fill="currentColor"><polygon points="5,3 19,12 5,21" /></svg>
        <span>Run</span>
      </button>
      <button class="tb-btn tb-stop" :disabled="!running" @click="$emit('stop')" title="Stop execution">
        <svg width="14" height="14" viewBox="0 0 24 24" fill="currentColor"><rect x="4" y="4" width="16" height="16" rx="2" /></svg>
        <span>Stop</span>
      </button>
      <div class="tb-sep" />
      <button class="tb-btn tb-flash" :class="{ active: activePanel === 'flash' }" @click="$emit('panel', 'flash')" title="Flash firmware">
        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <polygon points="13,2 3,14 12,14 11,22 21,10 12,10" />
        </svg>
        <span>Flash</span>
      </button>
      <button class="tb-btn tb-panel" :class="{ active: activePanel === 'panel' }" @click="$emit('panel', 'panel')" title="Open dashboard panel">
        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <rect x="3" y="3" width="7" height="9" rx="1" />
          <rect x="14" y="3" width="7" height="5" rx="1" />
          <rect x="14" y="12" width="7" height="9" rx="1" />
          <rect x="3" y="16" width="7" height="5" rx="1" />
        </svg>
        <span>Panel</span>
      </button>
      <button
        class="tb-btn tb-connect"
        :class="serialConnected ? 'connected' : 'disconnected'"
        @click="$emit('connect')"
        :title="serialConnected ? 'Disconnect' : 'Connect serial port'"
      >
        <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round">
          <path d="M12 2v6m0 8v6M6 8h12a2 2 0 012 2v4a2 2 0 01-2 2H6a2 2 0 01-2-2v-4a2 2 0 012-2z" />
        </svg>
        <span>{{ serialConnected ? 'Disconnect' : 'Connect' }}</span>
      </button>
    </div>
    <div class="toolbar-right">
      <select class="tb-examples" @change="onExampleChange($event)" title="Load example">
        <option value="" disabled selected>Examples</option>
        <option v-for="ex in examples" :key="ex.name" :value="ex.name">{{ ex.name }}</option>
      </select>
      <span v-if="serialConnected" class="tb-status connected">Connected</span>
      <span v-else class="tb-status">No device</span>
    </div>
  </div>
</template>

<script setup lang="ts">
import { examples } from '~/lib/playground-examples'

defineProps<{
  running: boolean
  serialConnected: boolean
  activePanel: string
}>()

const emit = defineEmits<{
  run: []
  stop: []
  connect: []
  panel: [panel: string]
  example: [name: string]
}>()

function onExampleChange(e: Event) {
  const name = (e.target as HTMLSelectElement).value
  if (name) {
    emit('example', name)
    ;(e.target as HTMLSelectElement).selectedIndex = 0
  }
}
</script>

<style scoped>
.toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  height: 44px;
  padding: 0 12px;
  background: var(--surface);
  border-top: 1px solid var(--border);
  gap: 8px;
}

.toolbar-left, .toolbar-right {
  display: flex;
  align-items: center;
  gap: 6px;
}

.tb-btn {
  display: flex;
  align-items: center;
  gap: 5px;
  padding: 5px 10px;
  border: 1px solid var(--border-bright);
  border-radius: 4px;
  background: transparent;
  color: var(--text);
  font-family: var(--mono);
  font-size: 12px;
  cursor: pointer;
  transition: all 0.15s;
}
.tb-btn:hover:not(:disabled) { color: var(--text-bright); border-color: var(--text-dim); }
.tb-btn:disabled { opacity: 0.4; cursor: not-allowed; }

/* Per-button color identity. Each action gets its own hue so the toolbar
 * is scannable at a glance — green=run/connect, red=stop, orange=flash,
 * purple=panel. The "active" tinted bg only shows when the button is
 * the relevant overlay open (Flash/Panel) or the action is live. */
.tb-run {
  color: var(--accent);
  border-color: var(--accent);
}
.tb-run:hover:not(:disabled) {
  background: var(--accent-dim);
  color: var(--accent);
  border-color: var(--accent);
}
.tb-run:disabled { color: var(--text-dim); border-color: var(--border-bright); }

.tb-stop {
  color: var(--red);
  border-color: rgba(224,93,112,0.4);
}
.tb-stop:hover:not(:disabled) {
  background: rgba(224,93,112,0.1);
  color: var(--red);
  border-color: var(--red);
}
.tb-stop:disabled { color: var(--text-dim); border-color: var(--border-bright); }

.tb-flash { color: var(--orange); border-color: rgba(226,101,58,0.4); }
.tb-flash:hover:not(:disabled),
.tb-flash.active {
  background: var(--orange-dim);
  color: var(--orange);
  border-color: var(--orange);
}

.tb-panel { color: var(--purple); border-color: rgba(139,124,244,0.4); }
.tb-panel:hover:not(:disabled),
.tb-panel.active {
  background: rgba(139,124,244,0.1);
  color: var(--purple);
  border-color: var(--purple);
}

.tb-connect.connected { color: var(--accent); border-color: var(--accent); }
.tb-connect.connected:hover { background: var(--accent-dim); }
.tb-connect.disconnected { color: var(--yellow); border-color: rgba(218,165,32,0.4); }
.tb-connect.disconnected:hover { background: var(--yellow-dim); color: var(--yellow); border-color: var(--yellow); }

.tb-sep {
  width: 1px;
  height: 20px;
  background: var(--border);
  margin: 0 4px;
}

.tb-examples {
  padding: 5px 8px;
  border: 1px solid var(--border-bright);
  border-radius: 4px;
  background: var(--surface);
  color: var(--text);
  font-family: var(--mono);
  font-size: 12px;
  cursor: pointer;
}

.tb-status {
  font-family: var(--mono);
  font-size: 11px;
  color: var(--text-dim);
}
.tb-status.connected { color: var(--accent); }

@media (max-width: 700px) {
  .tb-btn span { display: none; }
  .toolbar-right .tb-status { display: none; }
}
</style>
