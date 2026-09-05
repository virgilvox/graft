<template>
  <div class="landing">
    <!-- ── meta strip ─────────────────────────────────────────── -->
    <div class="strip">
      <div class="wrap">
        <span><b>protocol:</b> v02</span>
        <span><b>header:</b> 8 bytes</span>
        <span><b>framing:</b> cobs</span>
        <span><b>integrity:</b> crc8</span>
        <span><b>release:</b> v1.0.0</span>
        <span><b>license:</b> mit</span>
      </div>
    </div>

    <div class="wrap">
      <!-- ── hero ─────────────────────────────────────────────── -->
      <header class="hero">
        <div>
          <div class="chiprow">
            <span class="chip chip-mint">open hardware protocol</span>
            <span class="chip">MIT</span>
            <span class="chip">no cloud account</span>
          </div>
          <h1>CON<span class="wm-accent">DUYT</span></h1>
          <p class="lede">Your circuit board becomes an appliance. Plug it in and drive it from a script, the way you drive a printer.</p>
          <p class="sub">Flash the firmware once. After that the board just listens, and everything clever stays on your computer, in the language you already use.</p>
          <div class="cta">
            <NuxtLink class="btn btn-primary" to="/playground">
              <AppIcon name="bolt" :size="14" /> Blink an LED
            </NuxtLink>
            <a class="btn" href="#idea">Why it is faster</a>
          </div>
          <div class="ferrule hero-rule"><i /><s /><em>43 44 02 11 00 0002 XX 0D 01</em><s /></div>
        </div>
        <div class="heromark">
          <ConduytMark :size="300" />
        </div>
      </header>

      <!-- ── definitions ──────────────────────────────────────── -->
      <dl class="defs">
        <div v-for="d in definitions" :key="d.term" class="def">
          <dt>{{ d.term }}</dt>
          <dd>{{ d.gloss }}</dd>
        </div>
      </dl>

      <!-- ── 01 what that changes ─────────────────────────────── -->
      <section id="idea" class="sec">
        <div class="sec-head">
          <span class="num">01</span>
          <h2>What that changes</h2>
          <span class="note">same task, both ways</span>
        </div>
        <p class="sec-lede">The same afternoon of work, before and after.</p>

        <div class="twoup">
          <div class="panel">
            <div class="panel-hd"><AppIcon name="clock" :size="13" /> the usual way</div>
            <div class="panel-bd panel-bd-tight">
              <ol class="flow">
                <li v-for="s in beforeSteps" :key="s.text">
                  <span>{{ s.text }}</span>
                  <span v-if="s.cost" class="slow">{{ s.cost }}</span>
                </li>
              </ol>
            </div>
          </div>
          <div class="panel tapped">
            <div class="panel-hd"><AppIcon name="bolt" :size="13" /> with conduyt</div>
            <div class="panel-bd panel-bd-tight">
              <ol class="flow good">
                <li v-for="s in afterSteps" :key="s.text">
                  <span>{{ s.text }}</span>
                  <span v-if="s.cost" class="fast">{{ s.cost }}</span>
                </li>
              </ol>
            </div>
          </div>
        </div>

        <div class="ferrule"><i /><s /><em>the point</em><s /></div>
        <p class="closer dim">Not because the board got faster. Because you stopped rebuilding its program every time you had a thought.</p>
      </section>

      <!-- ── 02 blink, annotated ──────────────────────────────── -->
      <section id="blink" class="sec">
        <div class="sec-head">
          <span class="num">02</span>
          <h2>Blink an LED, line by line</h2>
          <span class="note">javascript / read the right column</span>
        </div>
        <p class="sec-lede">A complete program. It runs on your computer. Nothing is compiled, nothing is uploaded.</p>

        <div class="walk">
          <div class="walk-hd">
            <span class="t"><AppIcon name="code" :size="13" /> blink.js</span>
            <span class="chip chip-mint">npm install conduyt-js</span>
            <span class="chip walk-hd-end">no compile step</span>
          </div>

          <div v-for="(step, i) in walkthrough" :key="i" class="step">
            <!-- eslint-disable-next-line vue/no-v-html -- static local markup -->
            <div class="code"><pre v-html="step.code" /></div>
            <div class="say">
              <b>{{ i + 1 }}</b>
              <!-- eslint-disable-next-line vue/no-v-html -- static local markup -->
              <p v-html="step.say" />
            </div>
          </div>
        </div>

        <div class="ferrule"><i /><s /><em>same program, other languages</em><s /></div>

        <div class="tabs">
          <button
            v-for="sdk in sdkTabs"
            :key="sdk.name"
            :class="['tab', { on: activeLang === sdk.name }]"
            @click="activeLang = sdk.name"
          >
            {{ sdk.name }}
          </button>
        </div>
        <div class="slab">
          <div class="install">
            <AppIcon name="download" :size="13" class="install-ico" />
            <code>{{ activeSdk.install }}</code>
          </div>
          <!-- eslint-disable-next-line vue/no-v-html -- static local markup -->
          <pre class="code" v-html="activeSdk.code" />
        </div>
      </section>

      <!-- ── 03 four decisions ────────────────────────────────── -->
      <section class="sec">
        <div class="sec-head">
          <span class="num">03</span>
          <h2>Why it is built this way</h2>
          <span class="note">four decisions</span>
        </div>
        <div class="cells">
          <div v-for="(c, i) in decisions" :key="c.title" class="cell">
            <span class="n">{{ String(i + 1).padStart(2, '0') }}</span>
            <h3>{{ c.title }}</h3>
            <p>{{ c.body }}</p>
          </div>
        </div>
      </section>

      <!-- ── 04 the wire ──────────────────────────────────────── -->
      <section id="protocol" class="sec">
        <div class="sec-head">
          <span class="num">04</span>
          <h2>What goes over the wire</h2>
          <span class="note">turn on pin 13</span>
        </div>
        <p class="sec-lede">The entire message for turning on an LED.</p>

        <div class="frame">
          <div v-for="f in packetFields" :key="f.label" :class="['bcell', { magic: f.magic, pay: f.pay }]">
            <u>{{ f.label }}</u><b>{{ f.hex }}</b><em>{{ f.note }}</em>
          </div>
        </div>
        <p class="mono dim frame-note">43 44 spells CD in hex. Every packet starts with it. So does the logo.</p>

        <div class="twoup">
          <div class="panel tapped">
            <div class="panel-hd"><AppIcon name="cpu" :size="13" /> why not JSON</div>
            <div class="panel-bd">
              <p class="para">The chip has kilobytes of memory. Parsing <span class="mono inline-code">{"pin":13,"value":1}</span> costs more than the actual work.</p>
              <p class="para dim">Fixed positions mean byte three says everything. The checksum catches a bad cable before it drives a motor wrong.</p>
            </div>
          </div>
          <div class="panel">
            <div class="panel-hd"><AppIcon name="share" :size="13" /> pick your wire</div>
            <div class="panel-bd">
              <div class="trans">
                <div v-for="t in transports" :key="t.name" class="tport">
                  <AppIcon :name="t.icon" :size="16" />
                  <span>{{ t.name }}<small>{{ t.where }}</small></span>
                </div>
              </div>
            </div>
          </div>
        </div>
      </section>

      <!-- ── 05 handshake ─────────────────────────────────────── -->
      <section class="sec">
        <div class="sec-head">
          <span class="num">05</span>
          <h2>The board introduces itself</h2>
          <span class="note">first packet after connect</span>
        </div>
        <p class="sec-lede">On connect the board describes itself. Your program builds its interface from that.</p>

        <div class="twoup">
          <div class="panel">
            <div class="panel-hd"><AppIcon name="layers" :size="13" /> a sample HELLO_RESP</div>
            <div class="panel-bd mono hello">
              <div class="dim">firmware</div>
              <div>CONDUYT-ESP32 v1.0.0</div>
              <div class="ferrule thin"><i /><s /></div>
              <div class="dim">pins</div>
              <div>34 <span class="dim">/ 16 dimmable, 8 analog in</span></div>
              <div class="ferrule thin"><i /><s /></div>
              <div class="dim">modules compiled in</div>
              <div class="hello-chips">
                <span class="chip chip-mint">servo</span>
                <span class="chip chip-mint">neopixel</span>
                <span class="chip">dht</span>
                <span class="chip">oled</span>
              </div>
              <div class="ferrule thin"><i /><s /></div>
              <div class="dim">named datastreams</div>
              <div>temperature <span class="dim">degC</span> / lux <span class="dim">lx</span></div>
            </div>
          </div>
          <div class="stack">
            <div class="panel">
              <div class="panel-hd"><AppIcon name="terminal" :size="13" /> the app builds itself from that</div>
              <!-- eslint-disable-next-line vue/no-v-html -- static local markup -->
              <pre class="code" v-html="capabilitiesSnippet" />
            </div>
            <div class="panel tapped">
              <div class="panel-bd panel-bd-slim">
                <p class="note-body dim">Write the page once. A bare Arduino shows 20 pins. A loaded ESP32 grows servo and pixel controls on its own.</p>
              </div>
            </div>
          </div>
        </div>
      </section>

      <!-- ── 06 what people build ─────────────────────────────── -->
      <section class="sec">
        <div class="sec-head">
          <span class="num">06</span>
          <h2>What people build with it</h2>
          <span class="note">a few shapes</span>
        </div>
        <div class="builds">
          <div v-for="b in builds" :key="b.title" class="build">
            <AppIcon :name="b.icon" :size="15" class="build-ico" />
            <span><b>{{ b.title }}</b>{{ b.body }}</span>
          </div>
        </div>
      </section>

      <!-- ── 07 comparison ────────────────────────────────────── -->
      <section class="sec">
        <div class="sec-head">
          <span class="num">07</span>
          <h2>How it compares</h2>
          <span class="note">prior art</span>
        </div>
        <div class="panel scroller">
          <table>
            <thead>
              <tr><th>project</th><th>year</th><th>where it stops</th><th>conduyt</th></tr>
            </thead>
            <tbody>
              <tr v-for="c in comparisons" :key="c.name">
                <td>{{ c.name }}</td>
                <td class="dim">{{ c.year }}</td>
                <td class="dim">{{ c.limitation }}</td>
                <td class="ours">{{ c.approach }}</td>
              </tr>
            </tbody>
          </table>
        </div>
      </section>

      <!-- ── 08 libraries ─────────────────────────────────────── -->
      <section id="sdks" class="sec">
        <div class="sec-head">
          <span class="num">08</span>
          <h2>One protocol, six libraries</h2>
          <span class="note">same bytes everywhere</span>
        </div>
        <p class="sec-lede">One firmware library for the board. Five host SDKs for your computer.</p>
        <div class="sdks">
          <NuxtLink v-for="s in sdks" :key="s.name" class="sdk" :to="s.link">
            <h3>{{ s.name }}</h3>
            <span class="lang">{{ s.lang }}</span>
            <code>{{ s.install }}</code>
          </NuxtLink>
        </div>
      </section>

      <!-- ── 09 faq ───────────────────────────────────────────── -->
      <section class="sec">
        <div class="sec-head">
          <span class="num">09</span>
          <h2>Reasonable questions</h2>
        </div>
        <div class="faq">
          <details v-for="(q, i) in faq" :key="q.q" :open="i === 0">
            <summary>{{ q.q }}</summary>
            <p>{{ q.a }}</p>
          </details>
        </div>
      </section>

      <!-- ── closing ──────────────────────────────────────────── -->
      <section class="sec">
        <div class="panel closing">
          <ConduytMark :size="84" />
          <h2 class="display closing-title">Swap the wire.<br>Keep the code.</h2>
          <p class="dim closing-sub">Flash once. Then drive the board from a browser tab, a Python script or a Go service.</p>
          <div class="cta closing-cta">
            <NuxtLink class="btn btn-primary" to="/playground">
              <AppIcon name="bolt" :size="14" /> Start with a blink
            </NuxtLink>
            <NuxtLink class="btn btn-ghost" to="/docs/reference/packet-structure">Read the spec</NuxtLink>
          </div>
        </div>
      </section>
    </div>

    <!-- ── footer ─────────────────────────────────────────────── -->
    <footer class="footer">
      <div class="wrap">
        <div class="fgrid">
          <div class="fcol">
            <div class="fbrand">
              <ConduytMark variant="compact" :size="28" decorative />
              <span class="display fbrand-name">CON<span class="fbrand-accent">DUYT</span></span>
            </div>
            <p class="fblurb dim">An open way for computers and circuit boards to talk. MIT, no cloud. Built at LumenCanvas.</p>
            <div class="badges">
              <span class="chip">MIT</span>
              <span class="chip">no cloud</span>
              <span class="chip">no build step</span>
              <span class="chip chip-mint">43 44</span>
            </div>
          </div>
          <div class="fcol">
            <h2 class="fcol-h">Learn</h2>
            <NuxtLink to="/docs/tutorials/what-is-conduyt">What it is</NuxtLink>
            <NuxtLink to="/docs/tutorials/first-blink">First blink</NuxtLink>
            <NuxtLink to="/docs/reference/packet-structure">Packet structure</NuxtLink>
            <NuxtLink to="/playground">Playground</NuxtLink>
          </div>
          <div class="fcol">
            <h2 class="fcol-h">Build</h2>
            <NuxtLink to="/docs/reference/firmware-api">Firmware API</NuxtLink>
            <NuxtLink to="/docs/reference/js-api">JavaScript API</NuxtLink>
            <NuxtLink to="/docs/reference/python-api">Python API</NuxtLink>
            <NuxtLink to="/docs/boards/esp32">Supported boards</NuxtLink>
          </div>
        </div>
      </div>
    </footer>
  </div>
</template>

<script setup lang="ts">
import { computed, ref } from 'vue'

const definitions = [
  { term: 'Microcontroller', gloss: 'A computer on a chip, a few dollars. Arduino, ESP32, Pico.' },
  { term: 'Firmware', gloss: 'The program on that chip. You load CONDUYT once and leave it.' },
  { term: 'Transport', gloss: 'What carries the messages. USB, Bluetooth, WiFi.' },
  { term: 'Packet', gloss: 'One message. Ten bytes to set a pin.' },
]

const beforeSteps = [
  { text: 'Write C++ for the board', cost: 'every change' },
  { text: 'Compile', cost: '10 to 60 s' },
  { text: 'Flash over USB and wait', cost: '10 to 30 s' },
  { text: 'Squint at a serial monitor', cost: '' },
  { text: 'Change one number, back to step one', cost: '' },
]

const afterSteps = [
  { text: 'Flash the CONDUYT firmware', cost: 'once, ever' },
  { text: 'Write a normal script in JS, Python, Go, Rust or Swift', cost: '' },
  { text: 'Run it. The board reacts as the line executes', cost: 'instant' },
  { text: 'Print values, set breakpoints, use a real debugger', cost: '' },
  { text: 'Change one number, run again', cost: 'instant' },
]

const walkthrough = [
  {
    code: `<span class="k">import</span> { ConduytDevice } <span class="k">from</span> <span class="s">'conduyt-js'</span>
<span class="k">import</span> { SerialTransport } <span class="k">from</span> <span class="s">'conduyt-js/transports/serial'</span>`,
    say: 'Pick how you reach the board. <strong>SerialTransport</strong> is the USB cable. Swap this line for Bluetooth or WiFi later.',
  },
  {
    code: `<span class="k">const</span> device = <span class="k">await</span> ConduytDevice.<span class="f">connect</span>(
  <span class="k">new</span> <span class="f">SerialTransport</span>({ path: <span class="s">'/dev/ttyUSB0'</span> })
)`,
    say: 'Connect. The board reports its own pins and features, so your code never guesses.',
  },
  {
    code: `<span class="k">await</span> device.<span class="f">pin</span>(13).<span class="f">mode</span>(<span class="s">'output'</span>)`,
    say: 'Pin 13 is a metal leg, usually wired to the onboard LED. <strong>Output</strong> means you drive it, not read it.',
  },
  {
    code: `<span class="k">await</span> device.<span class="f">pin</span>(13).<span class="f">write</span>(1)  <span class="c">// on</span>
<span class="k">await</span> device.<span class="f">pin</span>(13).<span class="f">write</span>(0)  <span class="c">// off</span>`,
    say: 'The LED lights, then goes dark. Ten bytes per line. Loop it and you have a blink.',
  },
  {
    code: `<span class="k">const</span> temp = <span class="k">await</span> device.<span class="f">pin</span>(0).<span class="f">read</span>(<span class="s">'analog'</span>)
console.<span class="f">log</span>(<span class="s">'sensor:'</span>, temp)`,
    say: 'It reads back too. Sensor values arrive as plain numbers you can log or chart.',
  },
]

const sdkTabs = [
  {
    name: 'JavaScript',
    install: 'npm install conduyt-js',
    code: `<span class="k">import</span> { ConduytDevice } <span class="k">from</span> <span class="s">'conduyt-js'</span>
<span class="k">import</span> { SerialTransport } <span class="k">from</span> <span class="s">'conduyt-js/transports/serial'</span>

<span class="k">const</span> device = <span class="k">await</span> ConduytDevice.<span class="f">connect</span>(
  <span class="k">new</span> <span class="f">SerialTransport</span>({ path: <span class="s">'/dev/ttyUSB0'</span> })
)

<span class="k">await</span> device.<span class="f">pin</span>(13).<span class="f">mode</span>(<span class="s">'output'</span>)
<span class="k">await</span> device.<span class="f">pin</span>(13).<span class="f">write</span>(1)

<span class="k">const</span> value = <span class="k">await</span> device.<span class="f">pin</span>(0).<span class="f">read</span>(<span class="s">'analog'</span>)
console.<span class="f">log</span>(<span class="s">'sensor:'</span>, value)`,
  },
  {
    name: 'Python',
    install: 'pip install conduyt-py',
    code: `<span class="k">from</span> conduyt <span class="k">import</span> ConduytDevice
<span class="k">from</span> conduyt.transports.serial <span class="k">import</span> SerialTransport

device = <span class="f">ConduytDevice</span>(<span class="f">SerialTransport</span>(<span class="s">"/dev/ttyUSB0"</span>))
<span class="k">await</span> device.<span class="f">connect</span>()

<span class="k">await</span> device.<span class="f">pin</span>(13).<span class="f">mode</span>(<span class="s">"output"</span>)
<span class="k">await</span> device.<span class="f">pin</span>(13).<span class="f">write</span>(1)

value = <span class="k">await</span> device.<span class="f">pin</span>(0).<span class="f">read</span>(<span class="s">"analog"</span>)
<span class="f">print</span>(<span class="s">f"sensor: {value}"</span>)`,
  },
  {
    name: 'Go',
    install: 'go get github.com/virgilvox/conduyt/sdk/go',
    code: `device := conduyt.<span class="f">NewDevice</span>(transport)
hello, _ := device.<span class="f">Connect</span>(ctx)

device.<span class="f">Pin</span>(13).<span class="f">Mode</span>(ctx, conduyt.PinModeOutput)
device.<span class="f">Pin</span>(13).<span class="f">Write</span>(ctx, 1)

value, _ := device.<span class="f">Pin</span>(0).<span class="f">Read</span>(ctx)
fmt.<span class="f">Println</span>(<span class="s">"sensor:"</span>, value)`,
  },
  {
    name: 'Rust',
    install: 'cargo add conduyt',
    code: `<span class="k">let mut</span> device = Device::<span class="f">new</span>(transport);
device.<span class="f">connect</span>()?;

device.<span class="f">pin_mode</span>(13, PIN_MODE_OUTPUT)?;
device.<span class="f">pin_write</span>(13, 1)?;

<span class="k">let</span> value = device.<span class="f">pin_read</span>(0)?;
<span class="f">println!</span>(<span class="s">"sensor: {}"</span>, value);`,
  },
  {
    name: 'Swift',
    install: 'Swift Package Manager / ConduytKit',
    code: `<span class="k">import</span> ConduytKit

<span class="k">let</span> device = <span class="f">ConduytDevice</span>(transport: <span class="f">BLETransport</span>())
<span class="k">try await</span> device.<span class="f">connect</span>()

<span class="k">try await</span> device.<span class="f">pinMode</span>(13, mode: .output)
<span class="k">try await</span> device.<span class="f">pinWrite</span>(13, value: 1)

<span class="k">let</span> value = <span class="k">try await</span> device.<span class="f">pinRead</span>(0)
<span class="f">print</span>(<span class="s">"sensor: \\(value)"</span>)`,
  },
  {
    name: 'Arduino',
    install: '#include &lt;Conduyt.h&gt;  // the board side, flashed once',
    code: `<span class="k">#define</span> CONDUYT_MODULE_SERVO
<span class="k">#include</span> <span class="s">&lt;Conduyt.h&gt;</span>

ConduytSerial  transport(Serial, 115200);
ConduytDevice  device(<span class="s">"MyBoard"</span>, <span class="s">"1.0.0"</span>, transport);

<span class="k">void</span> <span class="f">setup</span>() {
  device.<span class="f">addModule</span>(<span class="k">new</span> <span class="f">ConduytModuleServo</span>());
  device.<span class="f">begin</span>();
}

<span class="k">void</span> <span class="f">loop</span>() {
  device.<span class="f">poll</span>();   <span class="c">// listen and obey, forever</span>
}`,
  },
]

const activeLang = ref('JavaScript')
const activeSdk = computed(() => sdkTabs.find(s => s.name === activeLang.value) ?? sdkTabs[0]!)

const decisions = [
  { title: 'Flash once, then forget it', body: 'The firmware never changes again. Every experiment is a script edit and a rerun.' },
  { title: 'The board introduces itself', body: 'One dashboard drives a 20 pin Arduino and a 34 pin ESP32, unchanged.' },
  { title: 'Any wire, same code', body: 'USB, Bluetooth, WiFi. Byte for byte identical messages. Swap one line, keep the rest.' },
  { title: 'Small enough for a cheap chip', body: 'Ten checksummed bytes to set a pin. No text to parse, no memory to allocate.' },
]

const packetFields = [
  { label: 'magic', hex: '43 44', note: 'this is a CONDUYT packet', magic: true },
  { label: 'ver', hex: '02', note: 'protocol v2' },
  { label: 'type', hex: '11', note: 'set a pin' },
  { label: 'seq', hex: '00', note: 'message number' },
  { label: 'len', hex: '00 02', note: '2 bytes follow' },
  { label: 'crc', hex: 'XX', note: 'arrived intact' },
  { label: 'payload', hex: '0D 01', note: 'pin 13, value on', pay: true },
]

const transports = [
  { name: 'Serial', where: 'USB cable', icon: 'link' },
  { name: 'BLE', where: 'across the room', icon: 'bluetooth' },
  { name: 'MQTT', where: 'across the internet', icon: 'broadcast' },
  { name: 'WebSocket', where: 'from a web app', icon: 'globe' },
  { name: 'TCP', where: 'on your network', icon: 'server' },
  { name: 'WebSerial', where: 'from a browser tab', icon: 'monitor' },
]

const capabilitiesSnippet = `<span class="k">const</span> caps = device.capabilities

<span class="c">// list the pins this board really has</span>
pinSelect.innerHTML = caps.pins
  .<span class="f">map</span>(p =&gt; <span class="s">\`&lt;option&gt;\${p.pin}&lt;/option&gt;\`</span>).<span class="f">join</span>(<span class="s">''</span>)

<span class="c">// hide the servo controls if this board has none</span>
servoCard.hidden = !caps.modules.<span class="f">find</span>(m =&gt; m.name === <span class="s">'servo'</span>)`

const builds = [
  { title: 'Sensor dashboards', body: 'Charted live in a browser tab, nothing installed.', icon: 'chart' },
  { title: 'Robot control', body: 'Arms and wheels from Python, beside your vision code.', icon: 'robot' },
  { title: 'LED installations', body: 'Pixel patterns in JavaScript, edited live.', icon: 'bulb' },
  { title: 'Classroom labs', body: 'Python on day one, no C++ toolchain week.', icon: 'book' },
  { title: 'Test rigs', body: 'A Go service flipping relays and reading voltages.', icon: 'pulse' },
  { title: 'Home automation', body: 'MQTT to a server you own, no vendor cloud.', icon: 'home' },
]

const comparisons = [
  { name: 'Firmata', year: '2006', limitation: 'MIDI encoding, USB cable only', approach: 'binary packets, any transport' },
  { name: 'Johnny-Five', year: '2012', limitation: 'Node only, built on Firmata', approach: 'five SDKs, native protocol' },
  { name: 'Blynk 2.0', year: '2021', limitation: 'needs their cloud', approach: 'runs on your own machine' },
  { name: 'CircuitPython', year: '2017', limitation: 'a language on the chip, not a link to your computer', approach: 'your computer drives, board obeys' },
]

const sdks = [
  { name: 'conduyt-firmware', lang: 'Arduino / C++ / the board side', install: '#include <Conduyt.h>', link: '/docs/reference/firmware-api' },
  { name: 'conduyt-js', lang: 'JavaScript / TypeScript', install: 'npm install conduyt-js', link: '/docs/sdks/javascript' },
  { name: 'conduyt-py', lang: 'Python 3.10+', install: 'pip install conduyt-py', link: '/docs/sdks/python' },
  { name: 'conduyt-go', lang: 'Go', install: 'go get github.com/virgilvox/conduyt/sdk/go', link: '/docs/sdks/go' },
  { name: 'conduyt', lang: 'Rust / no_std core', install: 'cargo add conduyt', link: '/docs/sdks/rust' },
  { name: 'ConduytKit', lang: 'Swift / iOS / macOS', install: 'Swift Package Manager', link: '/docs/sdks/swift' },
]

const faq = [
  {
    q: 'Do I need to know C++?',
    a: 'No. Flash the prebuilt firmware once, then write only in the language you already use. C++ comes up only if you add a new hardware feature.',
  },
  {
    q: 'What happens if my computer disconnects?',
    a: 'The board holds its last state and waits. Anything that must survive alone belongs in the firmware.',
  },
  {
    q: 'Is it fast enough for real work?',
    a: 'Ten bytes, under a millisecond over USB. Fine for dashboards, robots, lighting, test rigs. Microsecond control loops belong on the chip either way.',
  },
  {
    q: 'Does anything phone home?',
    a: 'No account, no server, no telemetry. Over USB it is a cable between two devices. MQTT and WebSocket point at a broker you run.',
  },
  {
    q: 'Which boards work?',
    a: 'Eighteen boards ship prebuilt firmware: Uno R3 and R4, Mega, Nano, Leonardo, the ESP32 family, ESP8266, Pico, Teensy 3.6 through 4.1 and the nRF52840 DK. Anything else the Arduino toolchain targets can be built from source. Transport options vary by board.',
  },
]

useHead({
  title: 'CONDUYT - drive any circuit board from any language',
  meta: [
    {
      name: 'description',
      content: 'An open binary protocol for hardware control. Flash the firmware once, then drive the board from JavaScript, Python, Go, Rust or Swift over USB, Bluetooth, WiFi or MQTT. MIT, no cloud account.',
    },
  ],
})
</script>

<style scoped>
/* ══ shell ═════════════════════════════════════════════════════ */
.landing {
  max-width: 100%;
  overflow-x: hidden;
}

.wrap {
  position: relative;
  z-index: 1;
  max-width: var(--measure);
  margin: 0 auto;
  padding: 0 var(--gutter);
}

/* ══ meta strip ════════════════════════════════════════════════ */
.strip {
  position: relative;
  z-index: 1;
  background: var(--conduit);
  border-bottom: var(--wall) solid var(--gasket);
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--chalk);
}
.strip .wrap {
  display: flex;
  gap: var(--sp-5);
  align-items: center;
  height: 34px;
  overflow-x: auto;
  scrollbar-width: none;
}
.strip .wrap::-webkit-scrollbar { display: none; }
.strip b { color: var(--ash); font-weight: 700; }
.strip span { white-space: nowrap; }

/* ══ primitives ════════════════════════════════════════════════ */
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
  text-decoration: none;
  box-shadow: var(--block-sm);
  transition: transform var(--snap), box-shadow var(--snap), border-color var(--snap);
}
.btn:hover {
  transform: translate(-2px, -2px);
  box-shadow: var(--block-lg);
  border-color: var(--shim);
  text-decoration: none;
}
.btn:active { transform: translate(1px, 1px); box-shadow: 1px 1px 0 var(--chalk); }
.btn-primary {
  background: var(--accent-fill);
  color: var(--accent-on);
  border-color: var(--accent-fill);
}
.btn-primary:hover { border-color: var(--accent-fill); }
.btn-ghost { background: transparent; box-shadow: none; }
.btn-ghost:hover { box-shadow: var(--block-sm); border-color: var(--mint); }

.chip {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  padding: 4px 8px;
  border: var(--rail) solid var(--gasket);
  color: var(--ash);
}
.chip-mint { color: var(--mint-ink); border-color: var(--mint-edge); background: var(--mint-wash); }

.panel {
  background: var(--raceway);
  border: var(--wall) solid var(--gasket);
  box-shadow: var(--block);
}
.panel-hd {
  background: var(--conduit);
  border-bottom: var(--wall) solid var(--gasket);
  padding: var(--sp-3) var(--sp-4);
  font-family: var(--face-mono);
  font-size: var(--t-label);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
  display: flex;
  align-items: center;
  gap: var(--sp-2);
}
.panel-bd { padding: var(--sp-5); }
.panel-bd-tight { padding-top: var(--sp-2); padding-bottom: var(--sp-2); }
.panel-bd-slim { padding: var(--sp-4) var(--sp-5); }
.tapped { border-left: var(--bulkhead) solid var(--mint); }

.ferrule { display: flex; align-items: center; margin: var(--sp-6) 0; }
.ferrule i { width: var(--wall); height: 14px; background: var(--mint); flex: none; }
.ferrule s { flex: 1; height: var(--rail); background: var(--gasket); }
.ferrule::after { content: ''; width: var(--wall); height: 14px; background: var(--gasket); flex: none; }
.ferrule.thin { margin: var(--sp-3) 0; }
.ferrule.thin i, .ferrule.thin::after { height: 8px; }
.ferrule em {
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-style: normal;
  color: var(--ghost);
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  padding: 0 var(--sp-3);
}

/* ══ section rhythm ════════════════════════════════════════════ */
.sec { padding: var(--sp-8) 0; border-top: var(--rail) dashed var(--gasket); }
.sec-head { display: flex; align-items: center; gap: var(--sp-4); margin-bottom: var(--sp-6); flex-wrap: wrap; }
.num {
  font-family: var(--face-mono);
  font-weight: 800;
  font-size: var(--t-meta);
  background: var(--accent-fill);
  color: var(--accent-on);
  padding: 5px 8px;
  line-height: 1;
}
.sec-head h2 {
  font-family: var(--face-display);
  font-weight: 400;
  font-size: var(--t-h2);
  margin: 0;
  text-transform: uppercase;
  letter-spacing: var(--track-tight);
  line-height: 1;
}
.sec-head .note {
  margin-left: auto;
  font-family: var(--face-mono);
  font-size: var(--t-label);
  color: var(--ghost);
  text-transform: uppercase;
  letter-spacing: var(--track-label);
}
.sec-lede { max-width: 62ch; margin: calc(var(--sp-6) * -1 + 2px) 0 var(--sp-6); color: var(--ash); }
.closer { max-width: 64ch; margin: 0; }

/* ══ hero ══════════════════════════════════════════════════════ */
.hero {
  padding: var(--sp-9) 0 var(--sp-7);
  display: grid;
  grid-template-columns: 1.12fr .88fr;
  gap: var(--sp-8);
  align-items: center;
}
.chiprow { display: flex; gap: var(--sp-2); flex-wrap: wrap; }
.hero h1 {
  font-family: var(--face-display);
  font-weight: 400;
  font-size: clamp(50px, 8.6vw, 110px);
  margin: var(--sp-4) 0 var(--sp-5);
  text-transform: uppercase;
  letter-spacing: var(--track-tight);
  line-height: .86;
}
.hero h1 .wm-accent { color: var(--mint-ink); }
.hero .lede { font-size: clamp(17px, 1.55vw, 21px); max-width: 40ch; margin: 0 0 var(--sp-4); line-height: 1.55; }
.hero .sub { font-size: var(--t-base); max-width: 44ch; color: var(--ash); margin: 0 0 var(--sp-6); }
.cta { display: flex; gap: var(--sp-3); flex-wrap: wrap; }
.hero-rule { margin-bottom: 0; }
.heromark { display: flex; justify-content: center; align-items: center; position: relative; }
.heromark::before { content: ''; position: absolute; inset: -6% -10%; border: var(--rail) dashed var(--gasket); }

/* ══ definitions ═══════════════════════════════════════════════ */
.defs {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  border: var(--wall) solid var(--gasket);
  border-left: var(--bulkhead) solid var(--mint);
  background: var(--raceway);
  box-shadow: var(--block);
  margin: 0 0 var(--sp-6);
}
.def { padding: var(--sp-4) var(--sp-5); border-right: var(--rail) dashed var(--gasket); }
.def:last-child { border-right: 0; }
.def dt {
  font-family: var(--face-mono);
  font-size: var(--t-label);
  font-weight: 800;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--mint-ink);
  margin: 0 0 6px;
}
.def dd { margin: 0; font-size: var(--t-sm); color: var(--ash); line-height: 1.6; }

/* ══ two-up ════════════════════════════════════════════════════ */
.twoup { display: grid; grid-template-columns: 1fr 1fr; gap: var(--sp-5); align-items: start; }
.stack { display: flex; flex-direction: column; gap: var(--sp-4); }

.flow { list-style: none; margin: 0; padding: 0; counter-reset: f; }
.flow li {
  counter-increment: f;
  display: flex;
  gap: var(--sp-3);
  align-items: center;
  padding: var(--sp-3) 0;
  border-bottom: var(--rail) dashed var(--gasket);
  font-size: var(--t-sm);
}
.flow li:last-child { border-bottom: 0; }
.flow li::before {
  content: counter(f);
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-weight: 800;
  background: var(--conduit);
  color: var(--ash);
  border: var(--rail) solid var(--gasket);
  padding: 4px 7px;
  line-height: 1;
  flex: none;
  align-self: flex-start;
  margin-top: 3px;
}
.flow.good li::before { background: var(--accent-fill); color: var(--accent-on); border-color: var(--accent-fill); }
.flow .slow, .flow .fast {
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  letter-spacing: .1em;
  margin-left: auto;
  white-space: nowrap;
  padding-left: var(--sp-3);
}
.flow .slow { color: var(--rust); }
.flow .fast { color: var(--sap); }

/* ══ code walkthrough ══════════════════════════════════════════ */
.walk { border: var(--wall) solid var(--gasket); box-shadow: var(--block); background: var(--soot); }
.walk-hd {
  background: var(--conduit);
  border-bottom: var(--wall) solid var(--gasket);
  padding: var(--sp-3) var(--sp-4);
  display: flex;
  align-items: center;
  gap: var(--sp-3);
  flex-wrap: wrap;
}
.walk-hd .t {
  display: inline-flex;
  align-items: center;
  gap: var(--sp-2);
  font-family: var(--face-mono);
  font-size: var(--t-label);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
}
.walk-hd-end { margin-left: auto; }

.step { display: grid; grid-template-columns: 1.05fr .95fr; border-bottom: var(--rail) dashed var(--gasket); }
.step:last-child { border-bottom: 0; }
.step .code { padding: var(--sp-4) var(--sp-5); border-right: var(--rail) dashed var(--gasket); }
.step .say { padding: var(--sp-4) var(--sp-5); background: var(--raceway); display: flex; gap: var(--sp-3); align-items: flex-start; }
.step .say b {
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-weight: 800;
  background: var(--accent-fill);
  color: var(--accent-on);
  padding: 4px 7px;
  line-height: 1;
  flex: none;
  margin-top: 3px;
}
.step .say p { margin: 0; font-size: var(--t-sm); color: var(--ash); line-height: 1.7; }
.step .say :deep(strong) { color: var(--chalk); font-weight: 600; }

pre {
  margin: 0;
  background: transparent;
  border: 0;
  padding: 0;
  overflow-x: auto;
  font-family: var(--face-mono);
  font-size: var(--t-sm);
  line-height: 1.85;
  color: var(--chalk);
}
.code :deep(.k) { color: var(--flux); }
.code :deep(.s) { color: var(--mint-ink); }
.code :deep(.c) { color: var(--ghost); }
.code :deep(.f) { color: var(--amber); }

/* ══ language tabs ═════════════════════════════════════════════ */
.tabs { display: flex; flex-wrap: wrap; border: var(--wall) solid var(--gasket); border-bottom: 0; box-shadow: var(--block-sm); }
.tab {
  font-family: var(--face-mono);
  font-size: var(--t-meta);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  padding: 12px 16px;
  background: var(--conduit);
  color: var(--ash);
  border: 0;
  border-right: var(--wall) solid var(--gasket);
  cursor: pointer;
  transition: color var(--snap), background var(--snap);
}
.tab:last-child { border-right: 0; }
.tab.on { background: var(--accent-fill); color: var(--accent-on); }
.tab:hover:not(.on) { color: var(--chalk); }

.slab { border: var(--wall) solid var(--gasket); background: var(--soot); box-shadow: var(--block); }
.slab .install {
  display: flex;
  align-items: center;
  gap: var(--sp-3);
  padding: 11px var(--sp-4);
  border-bottom: var(--wall) solid var(--gasket);
  font-family: var(--face-mono);
  font-size: var(--t-meta);
  color: var(--ash);
  background: var(--raceway);
}
.slab .install code {
  color: var(--mint-ink);
  background: transparent;
  border: 0;
  padding: 0;
  overflow-x: auto;
  white-space: nowrap;
  font-size: var(--t-meta);
}
.install-ico { color: var(--mint-ink); }
.slab pre { padding: var(--sp-5); }
.panel > pre { padding: var(--sp-5); }

/* ══ feature cells ═════════════════════════════════════════════ */
.cells {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  border: var(--wall) solid var(--gasket);
  box-shadow: var(--block);
}
.cell {
  padding: var(--sp-6) var(--sp-5);
  border-right: var(--wall) solid var(--gasket);
  background: var(--raceway);
  transition: background var(--snap);
}
.cell:last-child { border-right: 0; }
.cell:hover { background: var(--conduit); }
.cell .n {
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-weight: 800;
  letter-spacing: var(--track-label);
  color: var(--mint-ink);
  display: block;
  margin-bottom: var(--sp-4);
}
.cell h3 {
  font-family: var(--face-display);
  font-weight: 400;
  font-size: var(--t-lg);
  text-transform: uppercase;
  margin: 0 0 var(--sp-3);
  letter-spacing: var(--track-tight);
  line-height: 1.15;
}
.cell p { margin: 0; font-size: var(--t-sm); color: var(--ash); line-height: 1.75; }

/* ══ byte frame ════════════════════════════════════════════════ */
.frame { display: flex; flex-wrap: wrap; gap: var(--sp-2); margin-bottom: var(--sp-4); }
.bcell {
  border: var(--wall) solid var(--gasket);
  background: var(--soot);
  min-width: 58px;
  text-align: center;
  box-shadow: var(--block-sm);
  transition: transform var(--snap), border-color var(--snap);
}
.bcell:hover { transform: translate(-2px, -2px); border-color: var(--shim); }
.bcell u {
  display: block;
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  font-weight: 700;
  letter-spacing: var(--track-label);
  text-decoration: none;
  color: var(--ash);
  background: var(--conduit);
  padding: 4px 8px;
  border-bottom: var(--rail) solid var(--gasket);
}
.bcell b { display: block; font-family: var(--face-mono); font-size: var(--t-sm); font-weight: 700; padding: 10px; color: var(--chalk); }
.bcell em {
  display: block;
  font-style: normal;
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  color: var(--ghost);
  padding: 0 8px 8px;
  letter-spacing: .06em;
}
.bcell.magic { border-color: var(--mint); }
.bcell.magic u { background: var(--accent-fill); color: var(--accent-on); }
.bcell.magic b { color: var(--mint-ink); }
.bcell.pay { flex: 1; min-width: 140px; }
.frame-note { font-size: var(--t-meta); margin: 0 0 var(--sp-6); }

.para { margin: 0 0 var(--sp-3); }
.para:last-child { margin-bottom: 0; }
.inline-code { font-size: var(--t-sm); }
.note-body { margin: 0; font-size: var(--t-sm); }

/* ══ transports ════════════════════════════════════════════════ */
.trans { display: grid; grid-template-columns: repeat(auto-fit, minmax(148px, 1fr)); gap: var(--sp-3); }
.tport {
  border: var(--wall) solid var(--gasket);
  background: var(--raceway);
  padding: var(--sp-3) var(--sp-4);
  box-shadow: var(--block-sm);
  display: flex;
  align-items: center;
  gap: var(--sp-3);
  color: var(--mint-ink);
  transition: transform var(--snap), box-shadow var(--snap), border-color var(--snap);
}
.tport:hover { transform: translate(-2px, -2px); box-shadow: var(--block-lg); border-color: var(--mint-edge); }
.tport span {
  font-family: var(--face-mono);
  font-size: var(--t-meta);
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: .12em;
  color: var(--chalk);
}
.tport small {
  display: block;
  font-family: var(--face-body);
  font-size: var(--t-micro);
  color: var(--ghost);
  letter-spacing: 0;
  text-transform: none;
  font-weight: 400;
}

/* ══ hello panel ═══════════════════════════════════════════════ */
.hello { font-size: var(--t-sm); line-height: 2; }
.hello-chips { display: flex; gap: 6px; flex-wrap: wrap; margin-top: 6px; }

/* ══ builds ════════════════════════════════════════════════════ */
.builds { display: grid; grid-template-columns: repeat(auto-fit, minmax(240px, 1fr)); gap: var(--sp-3); }
.build {
  display: flex;
  gap: var(--sp-3);
  align-items: flex-start;
  border: var(--rail) solid var(--gasket);
  background: var(--raceway);
  padding: var(--sp-4);
}
.build-ico { color: var(--mint-ink); margin-top: 4px; }
.build span { font-size: var(--t-sm); color: var(--ash); line-height: 1.6; }
.build span b { color: var(--chalk); font-weight: 600; display: block; }

/* ══ table ═════════════════════════════════════════════════════ */
.scroller { overflow-x: auto; }
table { width: 100%; border-collapse: collapse; font-family: var(--face-mono); font-size: var(--t-sm); }
th {
  text-align: left;
  font-size: var(--t-micro);
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
  padding: 11px 13px;
  background: var(--conduit);
  border-bottom: var(--wall) solid var(--gasket);
  white-space: nowrap;
}
td { padding: 13px; }
tbody tr { border-bottom: var(--rail) solid var(--gasket); }
tbody tr:last-child { border-bottom: 0; }
td.ours { color: var(--mint-ink); }

/* ══ sdk grid ══════════════════════════════════════════════════ */
.sdks { display: grid; grid-template-columns: repeat(auto-fit, minmax(255px, 1fr)); gap: var(--sp-4); }
.sdk {
  border: var(--wall) solid var(--gasket);
  border-left: var(--bulkhead) solid var(--gasket);
  background: var(--raceway);
  box-shadow: var(--block);
  padding: var(--sp-5);
  display: block;
  color: var(--chalk);
  text-decoration: none;
  transition: transform var(--snap), box-shadow var(--snap), border-color var(--snap);
}
.sdk:hover {
  transform: translate(-2px, -2px);
  box-shadow: var(--block-lg);
  border-left-color: var(--mint);
  text-decoration: none;
}
.sdk h3 { font-family: var(--face-mono); font-size: var(--t-base); font-weight: 800; margin: 0 0 6px; letter-spacing: 0; text-transform: none; }
.sdk .lang {
  font-family: var(--face-mono);
  font-size: var(--t-micro);
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
}
.sdk code {
  display: block;
  margin-top: var(--sp-4);
  font-family: var(--face-mono);
  font-size: var(--t-meta);
  color: var(--mint-ink);
  background: var(--soot);
  border: var(--rail) solid var(--gasket);
  padding: 9px 11px;
  overflow-x: auto;
  white-space: nowrap;
}

/* ══ faq ═══════════════════════════════════════════════════════ */
.faq { border: var(--wall) solid var(--gasket); box-shadow: var(--block); background: var(--raceway); }
.faq details { border-bottom: var(--rail) dashed var(--gasket); }
.faq details:last-child { border-bottom: 0; }
.faq summary {
  cursor: pointer;
  padding: var(--sp-4) var(--sp-5);
  font-weight: 600;
  font-size: var(--t-base);
  display: flex;
  align-items: center;
  gap: var(--sp-3);
  list-style: none;
}
.faq summary::-webkit-details-marker { display: none; }
.faq summary::before { content: '+'; font-family: var(--face-mono); font-weight: 800; color: var(--mint-ink); flex: none; }
.faq details[open] summary::before { content: '\2212'; }
.faq summary:hover { background: var(--conduit); }
.faq p {
  margin: 0;
  padding: 0 var(--sp-5) var(--sp-5) calc(var(--sp-5) + 22px);
  color: var(--ash);
  font-size: var(--t-sm);
  max-width: 76ch;
}

/* ══ closing ═══════════════════════════════════════════════════ */
.closing { text-align: center; padding: var(--sp-8) var(--sp-5); display: flex; flex-direction: column; align-items: center; }
.closing-title { font-size: clamp(25px, 4vw, 42px); margin: var(--sp-5) 0 var(--sp-4); }
.closing-sub { max-width: 42ch; margin: 0 auto var(--sp-6); }
.closing-cta { justify-content: center; }

/* ══ footer ════════════════════════════════════════════════════ */
.footer {
  position: relative;
  z-index: 1;
  border-top: var(--wall) solid var(--gasket);
  margin-top: var(--sp-8);
  padding: var(--sp-7) 0 var(--sp-8);
  background: var(--soot);
}
.fgrid { display: grid; grid-template-columns: 1.4fr 1fr 1fr; gap: var(--sp-6); }
.fbrand { display: flex; align-items: center; gap: var(--sp-3); margin-bottom: var(--sp-3); }
.fbrand-name { font-size: 22px; }
.fbrand-accent { color: var(--mint-ink); }
.fblurb { font-size: var(--t-sm); max-width: 36ch; margin: 0; }
.badges { display: flex; gap: var(--sp-2); flex-wrap: wrap; margin-top: var(--sp-4); }
.fcol-h {
  font-family: var(--face-mono);
  font-size: var(--t-label);
  text-transform: uppercase;
  letter-spacing: var(--track-label);
  color: var(--ash);
  margin: 0 0 var(--sp-3);
  font-weight: 700;
}
.fcol a {
  display: block;
  font-family: var(--face-mono);
  font-size: var(--t-sm);
  color: var(--chalk);
  padding: 5px 0;
  text-decoration: none;
  transition: color var(--snap);
}
.fcol a:hover { color: var(--mint-ink); text-decoration: none; }

/* ══ responsive ════════════════════════════════════════════════ */
@media (max-width: 1000px) {
  .cell { border-right: 0; border-bottom: var(--wall) solid var(--gasket); }
  .cell:last-child { border-bottom: 0; }
}

@media (max-width: 900px) {
  .hero { grid-template-columns: 1fr; gap: var(--sp-6); padding-top: var(--sp-7); }
  .heromark { order: -1; justify-content: flex-start; }
  .heromark :deep(svg) { width: 190px; height: 190px; }
  .twoup { grid-template-columns: 1fr; }
  .defs { grid-template-columns: repeat(2, 1fr); }
  .def:nth-child(2) { border-right: 0; }
}

@media (max-width: 820px) {
  .step { grid-template-columns: 1fr; }
  .step .code { border-right: 0; border-bottom: var(--rail) dashed var(--gasket); }
}

@media (max-width: 800px) {
  .fgrid { grid-template-columns: 1fr; }
}

@media (max-width: 520px) {
  .defs { grid-template-columns: 1fr; }
  .def { border-right: 0; border-bottom: var(--rail) dashed var(--gasket); }
  .def:last-child { border-bottom: 0; }
  .sec { padding: var(--sp-7) 0; }
}
</style>
