---
title: Set Up MQTT Broker
description: Run a Mosquitto MQTT broker with Docker Compose for Conduyt devices
---

# Set Up MQTT Broker

Conduyt uses standard MQTT 3.1.1+ for IoT transport. This guide sets up Eclipse Mosquitto with Docker Compose.

## Docker Compose

Create a `docker-compose.yml`:

```yaml
version: '3.8'

services:
  mosquitto:
    image: eclipse-mosquitto:2
    ports:
      - "1883:1883"
      - "9001:9001"
    volumes:
      - ./mosquitto/config:/mosquitto/config
      - ./mosquitto/data:/mosquitto/data
      - ./mosquitto/log:/mosquitto/log
    restart: unless-stopped
```

## Mosquitto Configuration

Create `mosquitto/config/mosquitto.conf`:

```
listener 1883
listener 9001
protocol websockets

allow_anonymous false
password_file /mosquitto/config/passwd

persistence true
persistence_location /mosquitto/data/

log_dest file /mosquitto/log/mosquitto.log
```

Port 1883 handles standard MQTT connections. Port 9001 handles WebSocket connections for browser-based hosts.

## Create Directories and Start

```bash
mkdir -p mosquitto/config mosquitto/data mosquitto/log
docker compose up -d
```

Expected output:

```
[+] Running 1/1
 ✔ Container mosquitto  Started
```

## Create Users

```bash
# Create the first user (the -c flag creates the password file)
docker exec -it mosquitto mosquitto_passwd -c /mosquitto/config/passwd conduyt-device

# Add more users (no -c flag, appends to existing file)
docker exec -it mosquitto mosquitto_passwd /mosquitto/config/passwd conduyt-host
```

Restart the broker after adding users:

```bash
docker compose restart
```

Expected output:

```
[+] Restarting 1/1
 ✔ Container mosquitto  Started
```

## Verify the Broker

Subscribe in one terminal:

```bash
docker exec -it mosquitto mosquitto_sub -u conduyt-host -P yourpassword -t "test/#"
```

Publish in another:

```bash
docker exec -it mosquitto mosquitto_pub -u conduyt-device -P yourpassword -t "test/hello" -m "working"
```

The subscriber terminal displays:

```
working
```

If nothing appears, check that both users have the correct password and that the broker restarted after user creation.

## Production Hardening

For production deployments, apply these changes to `mosquitto.conf`:

```
# TLS on port 8883
listener 8883
cafile /mosquitto/config/ca.crt
certfile /mosquitto/config/server.crt
keyfile /mosquitto/config/server.key

# WebSocket with TLS
listener 9001
protocol websockets
cafile /mosquitto/config/ca.crt
certfile /mosquitto/config/server.crt
keyfile /mosquitto/config/server.key

allow_anonymous false
password_file /mosquitto/config/passwd

# Match Conduyt max packet size
max_packet_size 512

persistence true
persistence_location /mosquitto/data/
```

Use per-device credentials or client certificates. Configure ACLs to restrict each device to its own topic prefix (`conduyt/{deviceId}/#`). Monitor broker health with a Prometheus exporter like `mosquitto-exporter`.
