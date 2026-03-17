import Foundation

/// Protocol constants
public enum GraftProtocol {
    public static let version: UInt8 = 0x01
    public static let magic: [UInt8] = [0x47, 0x46]
    public static let headerSize = 8
}

/// Decoded GRAFT packet
public struct GraftPacket {
    public let version: UInt8
    public let type: UInt8
    public let seq: UInt8
    public let payload: Data

    public init(version: UInt8 = GraftProtocol.version, type: UInt8, seq: UInt8, payload: Data = Data()) {
        self.version = version
        self.type = type
        self.seq = seq
        self.payload = payload
    }
}

/// Wire format errors
public enum WireError: Error, LocalizedError {
    case incompletePacket
    case invalidMagic
    case versionMismatch
    case crcMismatch(expected: UInt8, actual: UInt8)

    public var errorDescription: String? {
        switch self {
        case .incompletePacket: return "Incomplete packet"
        case .invalidMagic: return "Invalid magic bytes"
        case .versionMismatch: return "Protocol version mismatch"
        case .crcMismatch(let e, let a): return "CRC mismatch: expected 0x\(String(e, radix: 16)), got 0x\(String(a, radix: 16))"
        }
    }
}

/// Encode a GraftPacket into raw wire bytes.
public func wireEncode(_ pkt: GraftPacket) -> Data {
    let payloadLen = pkt.payload.count
    var buf = Data(capacity: GraftProtocol.headerSize + payloadLen)

    buf.append(contentsOf: GraftProtocol.magic)
    buf.append(pkt.version)
    buf.append(pkt.type)
    buf.append(pkt.seq)
    buf.append(UInt8(payloadLen & 0xFF))
    buf.append(UInt8((payloadLen >> 8) & 0xFF))
    buf.append(pkt.payload)

    let crcRegion = Array(buf[2..<(7 + payloadLen)])
    buf.append(CRC8.compute(crcRegion))

    return buf
}

/// Decode raw wire bytes into a GraftPacket.
public func wireDecode(_ data: Data) throws -> GraftPacket {
    guard data.count >= GraftProtocol.headerSize else {
        throw WireError.incompletePacket
    }

    guard data[0] == GraftProtocol.magic[0] && data[1] == GraftProtocol.magic[1] else {
        throw WireError.invalidMagic
    }

    let version = data[2]
    guard version == GraftProtocol.version else {
        throw WireError.versionMismatch
    }

    let type = data[3]
    let seq = data[4]
    let payloadLen = Int(data[5]) | (Int(data[6]) << 8)

    let total = GraftProtocol.headerSize + payloadLen
    guard data.count >= total else {
        throw WireError.incompletePacket
    }

    let crcRegion = Array(data[2..<(7 + payloadLen)])
    let expected = CRC8.compute(crcRegion)
    let actual = data[7 + payloadLen]

    guard expected == actual else {
        throw WireError.crcMismatch(expected: expected, actual: actual)
    }

    let payload = data[7..<(7 + payloadLen)]
    return GraftPacket(version: version, type: type, seq: seq, payload: Data(payload))
}
