//! COBS — Consistent Overhead Byte Stuffing

extern crate alloc;
use alloc::vec::Vec;

/// Encode data using COBS. Output never contains 0x00.
pub fn cobs_encode(src: &[u8]) -> Vec<u8> {
    if src.is_empty() {
        return alloc::vec![0x01];
    }

    let mut dst = Vec::with_capacity(src.len() + src.len() / 254 + 2);
    dst.push(0); // placeholder
    let mut code_idx = 0usize;
    let mut code: u8 = 1;

    for &b in src {
        if b == 0x00 {
            dst[code_idx] = code;
            code_idx = dst.len();
            dst.push(0);
            code = 1;
        } else {
            dst.push(b);
            code += 1;
            if code == 0xFF {
                dst[code_idx] = code;
                code_idx = dst.len();
                dst.push(0);
                code = 1;
            }
        }
    }
    dst[code_idx] = code;
    dst
}

/// Decode COBS-encoded data. Returns None on decode error.
pub fn cobs_decode(src: &[u8]) -> Option<Vec<u8>> {
    if src.is_empty() {
        return None;
    }

    let mut dst = Vec::with_capacity(src.len());
    let mut idx = 0;

    while idx < src.len() {
        let code = src[idx];
        idx += 1;

        if code == 0x00 {
            return None;
        }

        let count = (code - 1) as usize;
        if idx + count > src.len() {
            return None;
        }

        dst.extend_from_slice(&src[idx..idx + count]);
        idx += count;

        if code < 0xFF && idx < src.len() {
            dst.push(0x00);
        }
    }
    Some(dst)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn roundtrip_empty() {
        let enc = cobs_encode(&[]);
        assert_eq!(enc, &[0x01]);
        assert_eq!(cobs_decode(&enc), Some(alloc::vec![]));
    }

    #[test]
    fn roundtrip_zero() {
        let data = [0x00];
        let enc = cobs_encode(&data);
        assert_eq!(cobs_decode(&enc).unwrap(), data);
    }

    #[test]
    fn roundtrip_mixed() {
        let data = [0x11, 0x22, 0x00, 0x33];
        let enc = cobs_encode(&data);
        assert_eq!(enc, &[0x03, 0x11, 0x22, 0x02, 0x33]);
        assert_eq!(cobs_decode(&enc).unwrap(), data);
    }

    #[test]
    fn no_zeros_in_output() {
        let data = [0, 1, 2, 0, 3, 0, 0, 4, 5];
        let enc = cobs_encode(&data);
        assert!(!enc.contains(&0x00));
        assert_eq!(cobs_decode(&enc).unwrap(), data);
    }

    #[test]
    fn decode_error_empty() {
        assert_eq!(cobs_decode(&[]), None);
    }

    #[test]
    fn decode_error_zero() {
        assert_eq!(cobs_decode(&[0x00, 0x01]), None);
    }
}
