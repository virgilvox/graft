<template>
  <div class="landing">
    <!-- HERO -->
    <section class="hero">
      <div class="hero-glow" />
      <svg class="corner corner-tl" width="30" height="30" viewBox="0 0 30 30"><path d="M0,20 L0,0 L20,0" fill="none" stroke="currentColor" stroke-width="1"/></svg>
      <svg class="corner corner-tr" width="30" height="30" viewBox="0 0 30 30"><path d="M0,20 L0,0 L20,0" fill="none" stroke="currentColor" stroke-width="1" transform="scale(-1,1) translate(-30,0)"/></svg>

      <div class="hero-inner">
        <div class="hero-eyebrow">Open Protocol Specification</div>

        <div class="hero-logo">
          <svg width="48" height="48" viewBox="0 0 28 28" fill="none">
            <rect x="2" y="8" width="5" height="12" fill="#00d4aa" opacity="0.9"/>
            <rect x="21" y="8" width="5" height="12" fill="#00d4aa" opacity="0.9"/>
            <rect x="9" y="4" width="10" height="4" fill="#00d4aa" opacity="0.5"/>
            <rect x="9" y="20" width="10" height="4" fill="#00d4aa" opacity="0.5"/>
            <line x1="7" y1="14" x2="21" y2="14" stroke="#00d4aa" stroke-width="1.5" opacity="0.4"/>
            <circle cx="14" cy="14" r="2.5" fill="#00d4aa"/>
            <circle cx="2" cy="8" r="1.5" fill="#e85d3b"/>
            <circle cx="26" cy="20" r="1.5" fill="#e85d3b"/>
          </svg>
          <h1>GRAFT</h1>
        </div>

        <div class="hero-sub">
          Generic Remote Actuator <span>Firmware</span> Transport
        </div>

        <p class="hero-desc">
          A transport-agnostic, capability-first binary protocol for extensible host-device hardware control.
          Runs on any microcontroller. Travels over serial, BLE, MQTT, WebSocket, or CLASP.
          Speaks to any host SDK. Owned by no cloud.
        </p>

        <div class="hero-tags">
          <span class="tag tag-accent">v0.1 Draft</span>
          <span class="tag tag-orange">Arduino / C++</span>
          <span class="tag">Binary Protocol</span>
          <span class="tag">COBS Framing</span>
          <span class="tag">Transport Agnostic</span>
          <span class="tag">MQTT Native</span>
          <span class="tag">Self-Describing</span>
          <span class="tag">Open Protocol</span>
        </div>

        <div class="hero-cta">
          <a href="/docs" class="btn-primary">Read the Docs</a>
          <a href="https://github.com/lumencanvas/graft" class="btn-secondary">GitHub</a>
        </div>
      </div>
    </section>

    <!-- WHAT IT REPLACES -->
    <section class="section">
      <div class="section-header">
        <span class="section-num">01</span>
        <h2>What It Replaces</h2>
      </div>

      <div class="compare-grid">
        <div class="compare-card" v-for="item in replacements" :key="item.name">
          <div class="compare-name">{{ item.name }}</div>
          <div class="compare-year">{{ item.year }}</div>
          <div class="compare-problem">{{ item.problem }}</div>
        </div>
      </div>
    </section>

    <!-- DESIGN GOALS -->
    <section class="section">
      <div class="section-header">
        <span class="section-num">02</span>
        <h2>Design Goals</h2>
      </div>

      <div class="goals-grid">
        <div class="goal-card" v-for="(goal, i) in goals" :key="i">
          <span class="goal-num">G{{ i + 1 }}</span>
          <div>
            <strong>{{ goal.title }}</strong>
            <p>{{ goal.desc }}</p>
          </div>
        </div>
      </div>
    </section>

    <!-- CODE EXAMPLES -->
    <section class="section">
      <div class="section-header">
        <span class="section-num">03</span>
        <h2>Code Examples</h2>
      </div>

      <div class="code-tabs">
        <button
          v-for="lang in codeExamples"
          :key="lang.name"
          :class="['code-tab', { active: activeLang === lang.name }]"
          @click="activeLang = lang.name"
        >{{ lang.name }}</button>
      </div>

      <div class="code-panel" v-for="lang in codeExamples" :key="lang.name" v-show="activeLang === lang.name">
        <div class="code-header">
          <span>{{ lang.name }}</span>
          <span class="code-pkg">{{ lang.pkg }}</span>
        </div>
        <pre>{{ lang.code }}</pre>
        <div class="code-transports">
          <span v-for="t in lang.transports" :key="t" class="tag" :class="{ 'tag-accent': t.active }">{{ t.name }}</span>
        </div>
      </div>
    </section>

    <!-- PACKET STRUCTURE -->
    <section class="section">
      <div class="section-header">
        <span class="section-num">04</span>
        <h2>Wire Format</h2>
      </div>

      <p style="max-width: 720px; margin-bottom: 20px;">
        Every GRAFT packet is a fixed header + variable payload. COBS encoding wraps the entire packet
        for serial/BLE transport, enabling <code>0x00</code> as a reliable delimiter. 8 bytes overhead.
        Full binary. No JSON on the wire.
      </p>

      <div class="packet-diagram">
        <div class="pkt-row">
          <div class="pkt-cell pkt-magic"><div class="pkt-label">MAGIC</div><div class="pkt-val">0x47 0x46</div><div class="pkt-size">2 bytes</div></div>
          <div class="pkt-cell"><div class="pkt-label">VER</div><div class="pkt-val">0x01</div><div class="pkt-size">1 byte</div></div>
          <div class="pkt-cell pkt-type"><div class="pkt-label">TYPE</div><div class="pkt-val">cmd/evt</div><div class="pkt-size">1 byte</div></div>
          <div class="pkt-cell"><div class="pkt-label">SEQ</div><div class="pkt-val pkt-val-purple">0-255</div><div class="pkt-size">1 byte</div></div>
          <div class="pkt-cell"><div class="pkt-label">LEN</div><div class="pkt-val">uint16_le</div><div class="pkt-size">2 bytes</div></div>
          <div class="pkt-cell pkt-payload"><div class="pkt-label">PAYLOAD</div><div class="pkt-val">type-specific</div><div class="pkt-size">N bytes</div></div>
          <div class="pkt-cell pkt-crc"><div class="pkt-label">CRC8</div><div class="pkt-val">Dallas/Maxim</div><div class="pkt-size">1 byte</div></div>
        </div>
      </div>
    </section>

    <!-- TRANSPORT TOPOLOGY -->
    <section class="section">
      <div class="section-header">
        <span class="section-num">05</span>
        <h2>Transport Topology</h2>
      </div>

      <div class="topo-grid">
        <div class="topo-card">
          <h3>Direct</h3>
          <div class="topo-diagram">
            <span class="topo-node">Host SDK</span>
            <span class="topo-arrow">serial / BLE</span>
            <span class="topo-node">Device</span>
          </div>
          <p>Peer-to-peer. Zero infrastructure. USB cable or Bluetooth.</p>
        </div>
        <div class="topo-card">
          <h3>Brokered</h3>
          <div class="topo-diagram">
            <span class="topo-node">Host SDK</span>
            <span class="topo-arrow">MQTT / CLASP</span>
            <span class="topo-node">Broker</span>
            <span class="topo-arrow">MQTT / CLASP</span>
            <span class="topo-node">Device</span>
          </div>
          <p>Multiple hosts, dashboards, Node-RED. Device broadcasts retained HELLO_RESP on connect.</p>
        </div>
      </div>
    </section>

    <!-- SDKs -->
    <section class="section">
      <div class="section-header">
        <span class="section-num">06</span>
        <h2>SDKs</h2>
      </div>

      <div class="sdk-grid">
        <div class="sdk-card" v-for="sdk in sdks" :key="sdk.name">
          <div class="sdk-name">{{ sdk.name }}</div>
          <div class="sdk-lang">{{ sdk.lang }}</div>
          <code>{{ sdk.install }}</code>
        </div>
      </div>
    </section>

    <!-- FOOTER -->
    <footer class="footer">
      <div class="footer-inner">
        <div class="footer-logo">
          <svg width="20" height="20" viewBox="0 0 28 28" fill="none">
            <rect x="2" y="8" width="5" height="12" fill="#00d4aa" opacity="0.8"/>
            <rect x="21" y="8" width="5" height="12" fill="#00d4aa" opacity="0.8"/>
            <rect x="9" y="4" width="10" height="4" fill="#00d4aa" opacity="0.4"/>
            <rect x="9" y="20" width="10" height="4" fill="#00d4aa" opacity="0.4"/>
            <circle cx="14" cy="14" r="2" fill="#00d4aa"/>
          </svg>
          <span>GRAFT</span>
        </div>
        <div class="footer-text">Open Protocol — CC BY 4.0 — <a href="https://github.com/lumencanvas">LumenCanvas</a></div>
      </div>
    </footer>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'

const activeLang = ref('JavaScript')

const replacements = [
  { name: 'Firmata', year: '2006', problem: 'Pseudo-binary MIDI encoding, serial-only, no real capability negotiation' },
  { name: 'Johnny-Five', year: '2012', problem: 'Great host SDK DX, but inherits all Firmata limits, CommonJS/Node-only' },
  { name: 'Blynk 2.0', year: '2021', problem: 'Solved the right problems, but proprietary cloud required, string-typed virtual pins' },
  { name: 'CircuitPython', year: '2017', problem: 'Runtime not protocol, no clean host-to-device control story' },
]

const goals = [
  { title: 'Transport Agnostic', desc: 'Same binary framing over serial, BLE, MQTT, CLASP, TCP, or WebSocket. Transport is a replaceable plug.' },
  { title: 'Capability First', desc: 'Device speaks first. Host never assumes. HELLO_RESP is the source of truth for everything the device has.' },
  { title: 'Lightweight Binary', desc: 'No JSON, no text parsing on the MCU. Fixed-overhead packets. Runs on ATmega328 at 115200 baud.' },
  { title: 'Module System', desc: 'Low-level pin control and high-level modules (servo, OLED, NeoPixel) coexist as opt-in firmware plugins.' },
  { title: 'Bidirectional Events', desc: 'Device pushes data without polling. Interrupt pins, sensor streams, encoder ticks fire unsolicited events.' },
  { title: 'SEQ + ACK', desc: 'Reliable messaging without requiring a reliable transport. Works on BLE where packet drops happen.' },
  { title: 'No Cloud Dependency', desc: 'Firmware never phones home. No hardcoded endpoints. Auth is the broker\'s problem. Self-hostable by design.' },
  { title: 'Named Typed Datastreams', desc: 'Blynk-style virtual channels, but device-declared, binary-typed, and schema-described in HELLO_RESP.' },
]

const codeExamples = [
  {
    name: 'JavaScript',
    pkg: 'npm install graft-js',
    code: `import { GraftDevice } from 'graft-js'
import { MQTTTransport } from 'graft-js/transports/mqtt'

const device = await GraftDevice.connect(
  new MQTTTransport({ broker: 'mqtt://broker.local', deviceId: 'my-board' })
)

// Pin control
await device.pin(13).mode('output')
await device.pin(13).write(1)

// Async subscription
for await (const val of device.pin(0).subscribe({ intervalMs: 50 })) {
  console.log('pot:', val)
}

// Datastream
for await (const val of device.datastream('temperature').subscribe()) {
  console.log('temp:', val, '\u00b0C')
}`,
    transports: [
      { name: 'Serial', active: true }, { name: 'BLE', active: true },
      { name: 'MQTT', active: true }, { name: 'CLASP', active: true },
      { name: 'WebSocket', active: true }, { name: 'WebUSB', active: true },
    ]
  },
  {
    name: 'Arduino',
    pkg: '#include <Graft.h>',
    code: `#include <Graft.h>
#include <GraftModuleServo.h>

GraftSerial  transport(Serial, 115200);
GraftDevice  device("MyBoard", "1.0.0", transport);

void setup() {
  device.addModule(new GraftModuleServo());
  device.addDatastream("temperature", GRAFT_FLOAT32, "celsius", false);
  device.begin();
}

void loop() {
  device.poll();
}`,
    transports: [
      { name: 'Serial', active: true }, { name: 'BLE', active: true },
      { name: 'MQTT', active: true }, { name: 'TCP', active: true },
      { name: 'CLASP', active: true },
    ]
  },
  {
    name: 'Python',
    pkg: 'pip install graft-py',
    code: `from graft import GraftDevice
from graft.transports import MQTTTransport
import asyncio

async def main():
    device = GraftDevice(MQTTTransport(
        broker='mqtt://broker.local', device_id='my-board'
    ))
    await device.connect()

    async for value in device.pin(0).subscribe(interval_ms=50):
        print(f'pot: {value}')

asyncio.run(main())`,
    transports: [
      { name: 'Serial', active: true }, { name: 'MQTT', active: true },
      { name: 'CLASP', active: true },
    ]
  },
  {
    name: 'Go',
    pkg: 'go get github.com/graft-io/graft-go',
    code: `transport := transports.NewMQTT(transports.MQTTConfig{
    Broker:   "tcp://broker.local:1883",
    DeviceID: "my-board",
})

device, _ := graft.Connect(ctx, transport)
defer device.Close()

ch, cancel := device.Pin(0).Subscribe(ctx, graft.SubscribeOpts{
    IntervalMs: 50, Threshold: 5,
})
defer cancel()

for val := range ch {
    fmt.Println("pot:", val)
}`,
    transports: [
      { name: 'Serial', active: true }, { name: 'MQTT', active: true },
      { name: 'TCP', active: true }, { name: 'CLASP', active: true },
    ]
  },
  {
    name: 'Rust',
    pkg: 'cargo add graft',
    code: `use graft::{GraftDevice, SubscribeOpts};
use graft::transports::MqttTransport;
use futures::StreamExt;

let transport = MqttTransport::builder()
    .broker("mqtt://broker.local")
    .device_id("my-board")
    .build()?;

let device = GraftDevice::connect(transport).await?;

let mut stream = device.pin(0)
    .subscribe(SubscribeOpts { interval_ms: 50, threshold: Some(5) })
    .await?;

while let Some(val) = stream.next().await {
    println!("pot: {}", val?);
}`,
    transports: [
      { name: 'Serial', active: true }, { name: 'MQTT', active: true },
      { name: 'TCP', active: true },
    ]
  },
]

const sdks = [
  { name: 'graft-firmware', lang: 'Arduino / C++', install: '#include <Graft.h>' },
  { name: 'graft-js', lang: 'JavaScript / TypeScript', install: 'npm install graft-js' },
  { name: 'graft-py', lang: 'Python 3.10+', install: 'pip install graft-py' },
  { name: 'graft-go', lang: 'Go', install: 'go get github.com/graft-io/graft-go' },
  { name: 'graft-rs', lang: 'Rust (no_std core)', install: 'cargo add graft' },
  { name: 'GraftKit', lang: 'Swift (BLE-first)', install: '.package(url: "github.com/graft-io/GraftKit")' },
]
</script>

<style scoped>
.landing { max-width: 100%; overflow-x: hidden; }

/* ── Hero ──────────────────────────────────────── */
.hero {
  padding: 80px 64px 60px;
  border-bottom: 1px solid var(--border);
  position: relative;
  overflow: hidden;
}
.hero-glow {
  position: absolute;
  top: -80px; right: -80px;
  width: 500px; height: 500px;
  background: radial-gradient(circle, rgba(0,212,170,0.06) 0%, transparent 70%);
  pointer-events: none;
}
.corner { position: absolute; opacity: 0.15; color: var(--text-dim); }
.corner-tl { top: 20px; left: 20px; }
.corner-tr { top: 20px; right: 20px; }
.hero-inner { max-width: 800px; position: relative; z-index: 1; }
.hero-eyebrow {
  font-size: 9px; letter-spacing: 4px; text-transform: uppercase;
  color: var(--accent); margin-bottom: 16px;
}
.hero-logo { display: flex; align-items: center; gap: 16px; margin-bottom: 8px; }
.hero-logo h1 {
  font-family: var(--heading); font-size: 64px; letter-spacing: 10px;
  color: var(--text-bright); line-height: 1;
}
.hero-sub {
  font-family: var(--code); font-size: 13px; color: var(--text-dim);
  letter-spacing: 1px; margin-bottom: 28px;
}
.hero-sub span { color: var(--orange); }
.hero-desc {
  font-size: 14px; color: var(--text); max-width: 640px;
  line-height: 1.9; margin-bottom: 32px;
}
.hero-tags { display: flex; flex-wrap: wrap; gap: 8px; margin-bottom: 36px; }
.hero-cta { display: flex; gap: 12px; }
.btn-primary {
  font-family: var(--code); font-size: 11px; letter-spacing: 2px; text-transform: uppercase;
  padding: 10px 24px; background: var(--accent); color: var(--bg); border: none;
  text-decoration: none; transition: opacity 0.15s;
}
.btn-primary:hover { opacity: 0.9; text-decoration: none; }
.btn-secondary {
  font-family: var(--code); font-size: 11px; letter-spacing: 2px; text-transform: uppercase;
  padding: 10px 24px; background: transparent; color: var(--text);
  border: 1px solid var(--border-bright); text-decoration: none; transition: all 0.15s;
}
.btn-secondary:hover { border-color: var(--accent); color: var(--accent); text-decoration: none; }

/* ── Sections ──────────────────────────────────── */
.section { padding: 56px 64px; border-bottom: 1px solid var(--border); }
.section-header { display: flex; align-items: baseline; gap: 16px; margin-bottom: 36px; }
.section-num { font-family: var(--code); font-size: 10px; color: var(--text-dim); letter-spacing: 2px; }
.section h2 { font-size: 24px; letter-spacing: 5px; }

/* ── Compare Grid ──────────────────────────────── */
.compare-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 12px; }
.compare-card {
  background: var(--card); border: 1px dashed var(--border); padding: 20px;
}
.compare-name { font-family: var(--code); font-size: 13px; color: var(--text-bright); font-weight: 700; }
.compare-year { font-size: 10px; color: var(--text-dim); margin-bottom: 8px; }
.compare-problem { font-size: 12px; color: var(--text); line-height: 1.6; }

/* ── Goals Grid ────────────────────────────────── */
.goals-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 12px; }
.goal-card {
  background: var(--card); border: 1px dashed var(--border); padding: 16px 20px;
  display: flex; gap: 14px; align-items: flex-start;
}
.goal-num { font-family: var(--code); font-size: 10px; color: var(--accent); font-weight: 700; min-width: 24px; }
.goal-card strong {
  font-family: var(--code); font-size: 12px; color: var(--text-bright);
  display: block; margin-bottom: 4px; letter-spacing: 0.5px;
}
.goal-card p { font-size: 12px; line-height: 1.6; margin: 0; }

/* ── Code Tabs ─────────────────────────────────── */
.code-tabs { display: flex; gap: 0; margin-bottom: 0; border-bottom: 1px solid var(--border); }
.code-tab {
  font-family: var(--mono); font-size: 10px; letter-spacing: 1.5px; text-transform: uppercase;
  padding: 10px 20px; background: transparent; color: var(--text-dim);
  border: none; cursor: pointer; border-bottom: 2px solid transparent;
  transition: all 0.15s;
}
.code-tab:hover { color: var(--text); }
.code-tab.active { color: var(--accent); border-bottom-color: var(--accent); }
.code-panel { background: var(--card); border: 1px solid var(--border); border-top: none; }
.code-header {
  display: flex; justify-content: space-between; align-items: center;
  padding: 12px 20px; border-bottom: 1px solid var(--border); background: rgba(255,255,255,0.02);
}
.code-header span:first-child {
  font-family: var(--heading); font-size: 14px; letter-spacing: 3px; color: var(--text-bright);
}
.code-pkg { font-family: var(--code); font-size: 9px; color: var(--text-dim); }
.code-panel pre { background: transparent; border: none; border-left: none; margin: 0; font-size: 11px; }
.code-transports { display: flex; flex-wrap: wrap; gap: 4px; padding: 10px 20px; border-top: 1px solid var(--border); }

/* ── Packet Diagram ────────────────────────────── */
.packet-diagram {
  font-family: var(--code); background: #070709;
  border: 1px solid var(--border); padding: 24px; overflow-x: auto;
}
.pkt-row { display: flex; }
.pkt-cell {
  border: 1px solid var(--border-bright); padding: 12px 14px;
  text-align: center; font-size: 10px; flex-shrink: 0;
}
.pkt-label { font-size: 8px; text-transform: uppercase; letter-spacing: 1.5px; color: var(--text-dim); margin-bottom: 4px; }
.pkt-val { color: var(--text-bright); font-weight: 600; }
.pkt-val-purple { color: var(--purple); }
.pkt-size { font-size: 8px; color: var(--text-dim); margin-top: 3px; }
.pkt-magic { background: rgba(0,212,170,0.05); border-color: var(--accent); }
.pkt-magic .pkt-val { color: var(--accent); }
.pkt-type { background: rgba(232,93,59,0.05); }
.pkt-type .pkt-val { color: var(--orange); }
.pkt-payload { flex: 1; background: rgba(155,124,244,0.04); }
.pkt-payload .pkt-val { color: var(--purple); }
.pkt-crc { background: rgba(240,192,64,0.05); }
.pkt-crc .pkt-val { color: var(--yellow); }

/* ── Topology ──────────────────────────────────── */
.topo-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 16px; }
.topo-card {
  background: var(--card); border: 1px dashed var(--border); padding: 24px;
}
.topo-card h3 { margin-bottom: 16px; }
.topo-diagram {
  display: flex; align-items: center; gap: 12px; flex-wrap: wrap;
  font-family: var(--code); font-size: 11px; margin-bottom: 12px;
}
.topo-node {
  color: var(--accent); font-weight: 600; padding: 6px 12px;
  border: 1px solid rgba(0,212,170,0.3); background: var(--accent-dim);
}
.topo-arrow { color: var(--text-dim); font-size: 10px; }
.topo-card p { font-size: 12px; margin: 0; }

/* ── SDK Grid ──────────────────────────────────── */
.sdk-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 12px; }
.sdk-card {
  background: var(--card); border: 1px dashed var(--border); padding: 16px 20px;
}
.sdk-name { font-family: var(--code); font-size: 13px; color: var(--accent); font-weight: 700; margin-bottom: 2px; }
.sdk-lang { font-size: 11px; color: var(--text-dim); margin-bottom: 8px; }
.sdk-card code { display: block; font-size: 10px; margin-top: 4px; }

/* ── Footer ────────────────────────────────────── */
.footer { padding: 32px 64px; border-top: 1px solid var(--border); }
.footer-inner { display: flex; justify-content: space-between; align-items: center; }
.footer-logo {
  display: flex; align-items: center; gap: 10px;
  font-family: var(--heading); font-size: 14px; letter-spacing: 4px; color: var(--text-bright);
}
.footer-text { font-size: 10px; color: var(--text-dim); letter-spacing: 1px; }

/* ── Responsive ────────────────────────────────── */
@media (max-width: 900px) {
  .hero { padding: 40px 24px; }
  .hero-logo h1 { font-size: 40px; }
  .section { padding: 36px 24px; }
  .topo-grid { grid-template-columns: 1fr; }
  .footer { padding: 24px; }
  .footer-inner { flex-direction: column; gap: 12px; }
}
</style>
