use wasm_bindgen_test::*;
use conduyt_wasm::*;

wasm_bindgen_test_configure!(run_in_browser);

#[wasm_bindgen_test]
fn cobs_roundtrip_empty() {
    let enc = js_cobs_encode(&[]);
    let dec = js_cobs_decode(&enc);
    assert!(!dec.is_null());
}

#[wasm_bindgen_test]
fn cobs_roundtrip_mixed() {
    let data = [0x01, 0x00, 0x02, 0x00, 0x03];
    let enc = js_cobs_encode(&data);
    // Verify no zeros in encoded output
    let enc_bytes: Vec<u8> = enc.clone();
    for b in &enc_bytes {
        assert_ne!(*b, 0x00);
    }
    let dec = js_cobs_decode(&enc);
    assert!(!dec.is_null());
}

#[wasm_bindgen_test]
fn crc8_deterministic() {
    let data = [0x01, 0x02, 0x03];
    assert_eq!(js_crc8(&data), js_crc8(&data));
}

#[wasm_bindgen_test]
fn crc8_different_data() {
    assert_ne!(
        js_crc8(&[0x01, 0x02]),
        js_crc8(&[0x03, 0x04])
    );
}

#[wasm_bindgen_test]
fn wire_packet_size() {
    assert_eq!(js_wire_packet_size(0), 8);
    assert_eq!(js_wire_packet_size(10), 18);
}

#[wasm_bindgen_test]
fn constants_cmd() {
    let cmd = js_get_cmd();
    assert!(!cmd.is_null());
    assert!(!cmd.is_undefined());
}

#[wasm_bindgen_test]
fn constants_evt() {
    let evt = js_get_evt();
    assert!(!evt.is_null());
    assert!(!evt.is_undefined());
}

#[wasm_bindgen_test]
fn constants_err() {
    let err = js_get_err();
    assert!(!err.is_null());
    assert!(!err.is_undefined());
}

#[wasm_bindgen_test]
fn err_name_known() {
    assert_eq!(js_err_name(0x01), "UNKNOWN_TYPE");
    assert_eq!(js_err_name(0x02), "CRC_MISMATCH");
}

#[wasm_bindgen_test]
fn err_name_unknown() {
    assert_eq!(js_err_name(0xFF), "UNKNOWN");
}
