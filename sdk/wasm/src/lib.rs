//! CONDUYT Protocol Core — WebAssembly bindings
//!
//! Thin wasm-bindgen wrappers over the conduyt no_std core.
//! Produces a WASM module usable from browsers and Node.js.

use lol_alloc::{AssumeSingleThreaded, FreeListAllocator};

#[global_allocator]
static ALLOCATOR: AssumeSingleThreaded<FreeListAllocator> =
    unsafe { AssumeSingleThreaded::new(FreeListAllocator::new()) };

use wasm_bindgen::prelude::*;
use js_sys::{Object, Reflect, Uint8Array, Array};

use conduyt::cobs::{cobs_encode as cobs_encode_impl, cobs_decode as cobs_decode_impl};
use conduyt::crc8::crc8 as crc8_impl;
use conduyt::wire::{
    wire_encode as wire_encode_impl, wire_decode as wire_decode_impl,
    make_packet as make_packet_impl, wire_find_packet as wire_find_packet_impl,
    Packet, WireError, HEADER_SIZE as HEADER_SIZE_CONST, PROTOCOL_VERSION as PROTO_VER,
    MAGIC as MAGIC_BYTES,
};
use conduyt::constants::*;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

fn packet_to_js(pkt: &Packet) -> JsValue {
    let obj = Object::new();
    Reflect::set(&obj, &"version".into(), &pkt.version.into()).unwrap();
    Reflect::set(&obj, &"type".into(), &pkt.pkt_type.into()).unwrap();
    Reflect::set(&obj, &"seq".into(), &pkt.seq.into()).unwrap();
    let payload = Uint8Array::from(pkt.payload.as_slice());
    Reflect::set(&obj, &"payload".into(), &payload.into()).unwrap();
    obj.into()
}

fn wire_error_to_js(e: &WireError, data: &[u8]) -> String {
    match e {
        WireError::IncompletePkt => {
            format!(
                "Incomplete packet: need at least {} bytes, got {}",
                HEADER_SIZE_CONST,
                data.len()
            )
        }
        WireError::InvalidMagic => {
            if data.len() >= 2 {
                format!(
                    "Invalid magic: expected 0x{:x}{:x}, got 0x{:x}{:x}",
                    MAGIC_BYTES[0], MAGIC_BYTES[1], data[0], data[1]
                )
            } else {
                format!(
                    "Incomplete packet: need at least {} bytes, got {}",
                    HEADER_SIZE_CONST,
                    data.len()
                )
            }
        }
        WireError::VersionMismatch => {
            format!("Version mismatch: expected {}, got {}", PROTO_VER, data[2])
        }
        WireError::CrcMismatch { expected, actual } => {
            format!("CRC mismatch: expected 0x{:x}, got 0x{:x}", expected, actual)
        }
    }
}

// ---------------------------------------------------------------------------
// CRC8
// ---------------------------------------------------------------------------

#[wasm_bindgen(js_name = "crc8")]
pub fn js_crc8(data: &[u8]) -> u8 {
    crc8_impl(data)
}

// ---------------------------------------------------------------------------
// COBS
// ---------------------------------------------------------------------------

#[wasm_bindgen(js_name = "cobsEncode")]
pub fn js_cobs_encode(data: &[u8]) -> Vec<u8> {
    cobs_encode_impl(data)
}

#[wasm_bindgen(js_name = "cobsDecode")]
pub fn js_cobs_decode(data: &[u8]) -> JsValue {
    match cobs_decode_impl(data) {
        Some(v) => Uint8Array::from(v.as_slice()).into(),
        None => JsValue::NULL,
    }
}

// ---------------------------------------------------------------------------
// Wire format
// ---------------------------------------------------------------------------

#[wasm_bindgen(js_name = "wireEncode")]
pub fn js_wire_encode(packet: &JsValue) -> Result<Vec<u8>, JsValue> {
    let version = Reflect::get(packet, &"version".into())
        .map_err(|_| JsValue::from_str("missing version"))?
        .as_f64()
        .ok_or_else(|| JsValue::from_str("version must be number"))? as u8;
    let pkt_type = Reflect::get(packet, &"type".into())
        .map_err(|_| JsValue::from_str("missing type"))?
        .as_f64()
        .ok_or_else(|| JsValue::from_str("type must be number"))? as u8;
    let seq = Reflect::get(packet, &"seq".into())
        .map_err(|_| JsValue::from_str("missing seq"))?
        .as_f64()
        .ok_or_else(|| JsValue::from_str("seq must be number"))? as u8;
    let payload_js = Reflect::get(packet, &"payload".into())
        .map_err(|_| JsValue::from_str("missing payload"))?;
    let payload_arr = Uint8Array::new(&payload_js);
    let payload = payload_arr.to_vec();

    let pkt = Packet {
        version,
        pkt_type,
        seq,
        payload,
    };
    Ok(wire_encode_impl(&pkt))
}

#[wasm_bindgen(js_name = "wireDecode")]
pub fn js_wire_decode(data: &[u8]) -> Result<JsValue, JsValue> {
    match wire_decode_impl(data) {
        Ok(pkt) => Ok(packet_to_js(&pkt)),
        Err(e) => Err(JsValue::from_str(&wire_error_to_js(&e, data))),
    }
}

#[wasm_bindgen(js_name = "makePacket")]
pub fn js_make_packet(pkt_type: u8, seq: u8, payload: Option<Vec<u8>>) -> JsValue {
    let p = payload.unwrap_or_default();
    let pkt = make_packet_impl(pkt_type, seq, &p);
    packet_to_js(&pkt)
}

#[wasm_bindgen(js_name = "wirePacketSize")]
pub fn js_wire_packet_size(payload_len: usize) -> usize {
    HEADER_SIZE_CONST + payload_len
}

#[wasm_bindgen(js_name = "wireFindPacket")]
pub fn js_wire_find_packet(data: &[u8]) -> JsValue {
    match wire_find_packet_impl(data) {
        Some((pkt, consumed)) => {
            let arr = Array::new_with_length(2);
            arr.set(0, packet_to_js(&pkt));
            arr.set(1, JsValue::from_f64(consumed as f64));
            arr.into()
        }
        None => JsValue::NULL,
    }
}

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------

#[wasm_bindgen(js_name = "PROTOCOL_VERSION")]
pub fn js_protocol_version() -> u8 {
    PROTO_VER
}

#[wasm_bindgen(js_name = "HEADER_SIZE")]
pub fn js_header_size() -> usize {
    HEADER_SIZE_CONST
}

#[wasm_bindgen(js_name = "getCMD")]
pub fn js_get_cmd() -> JsValue {
    let obj = Object::new();
    let s = |k: &str, v: u8| {
        Reflect::set(&obj, &k.into(), &v.into()).unwrap();
    };
    s("PING", CMD_PING);
    s("HELLO", CMD_HELLO);
    s("PIN_MODE", CMD_PIN_MODE);
    s("PIN_WRITE", CMD_PIN_WRITE);
    s("PIN_READ", CMD_PIN_READ);
    s("PIN_SUBSCRIBE", CMD_PIN_SUBSCRIBE);
    s("PIN_UNSUBSCRIBE", CMD_PIN_UNSUBSCRIBE);
    s("I2C_WRITE", CMD_I2C_WRITE);
    s("I2C_READ", CMD_I2C_READ);
    s("I2C_READ_REG", CMD_I2C_READ_REG);
    s("SPI_XFER", CMD_SPI_XFER);
    s("MOD_CMD", CMD_MOD_CMD);
    s("STREAM_START", CMD_STREAM_START);
    s("STREAM_STOP", CMD_STREAM_STOP);
    s("DS_WRITE", CMD_DS_WRITE);
    s("DS_READ", CMD_DS_READ);
    s("DS_SUBSCRIBE", CMD_DS_SUBSCRIBE);
    s("OTA_BEGIN", CMD_OTA_BEGIN);
    s("OTA_CHUNK", CMD_OTA_CHUNK);
    s("OTA_FINALIZE", CMD_OTA_FINALIZE);
    s("RESET", CMD_RESET);
    Object::freeze(&obj);
    obj.into()
}

#[wasm_bindgen(js_name = "getEVT")]
pub fn js_get_evt() -> JsValue {
    let obj = Object::new();
    let s = |k: &str, v: u8| {
        Reflect::set(&obj, &k.into(), &v.into()).unwrap();
    };
    s("PONG", EVT_PONG);
    s("HELLO_RESP", EVT_HELLO_RESP);
    s("ACK", EVT_ACK);
    s("NAK", EVT_NAK);
    s("PIN_EVENT", EVT_PIN_EVENT);
    s("PIN_READ_RESP", EVT_PIN_READ_RESP);
    s("I2C_READ_RESP", EVT_I2C_READ_RESP);
    s("SPI_XFER_RESP", EVT_SPI_XFER_RESP);
    s("MOD_EVENT", EVT_MOD_EVENT);
    s("MOD_RESP", EVT_MOD_RESP);
    s("STREAM_DATA", EVT_STREAM_DATA);
    s("DS_EVENT", EVT_DS_EVENT);
    s("DS_READ_RESP", EVT_DS_READ_RESP);
    s("LOG", EVT_LOG);
    s("FATAL", EVT_FATAL);
    Object::freeze(&obj);
    obj.into()
}

#[wasm_bindgen(js_name = "getERR")]
pub fn js_get_err() -> JsValue {
    let obj = Object::new();
    let s = |k: &str, v: u8| {
        Reflect::set(&obj, &k.into(), &v.into()).unwrap();
    };
    s("UNKNOWN_TYPE", ERR_UNKNOWN_TYPE);
    s("CRC_MISMATCH", ERR_CRC_MISMATCH);
    s("PAYLOAD_TOO_LARGE", ERR_PAYLOAD_TOO_LARGE);
    s("INVALID_PIN", ERR_INVALID_PIN);
    s("PIN_MODE_UNSUPPORTED", ERR_PIN_MODE_UNSUPPORTED);
    s("I2C_NOT_AVAILABLE", ERR_I2C_NOT_AVAILABLE);
    s("I2C_NACK", ERR_I2C_NACK);
    s("MODULE_NOT_LOADED", ERR_MODULE_NOT_LOADED);
    s("UNKNOWN_MODULE_CMD", ERR_UNKNOWN_MODULE_CMD);
    s("MODULE_BUSY", ERR_MODULE_BUSY);
    s("SUB_LIMIT_REACHED", ERR_SUB_LIMIT_REACHED);
    s("OUT_OF_MEMORY", ERR_OUT_OF_MEMORY);
    s("UNKNOWN_DATASTREAM", ERR_UNKNOWN_DATASTREAM);
    s("DATASTREAM_READONLY", ERR_DATASTREAM_READONLY);
    s("OTA_INVALID", ERR_OTA_INVALID);
    s("VERSION_MISMATCH", ERR_VERSION_MISMATCH);
    Object::freeze(&obj);
    obj.into()
}

#[wasm_bindgen(js_name = "errName")]
pub fn js_err_name(code: u8) -> String {
    err_name(code).into()
}
