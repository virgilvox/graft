import XCTest
@testable import GraftKit

final class WireTests: XCTestCase {
    func testPingRoundTrip() throws {
        let pkt = GraftPacket(type: 0x01, seq: 7)
        let encoded = wireEncode(pkt)
        XCTAssertEqual(encoded.count, GraftProtocol.headerSize)

        let decoded = try wireDecode(encoded)
        XCTAssertEqual(decoded.type, 0x01)
        XCTAssertEqual(decoded.seq, 7)
        XCTAssertTrue(decoded.payload.isEmpty)
    }

    func testPinWriteRoundTrip() throws {
        let payload = Data([5, 128])
        let pkt = GraftPacket(type: 0x11, seq: 255, payload: payload)
        let encoded = wireEncode(pkt)
        let decoded = try wireDecode(encoded)

        XCTAssertEqual(decoded.type, 0x11)
        XCTAssertEqual(decoded.seq, 255)
        XCTAssertEqual(decoded.payload, payload)
    }

    func testRejectIncomplete() {
        XCTAssertThrowsError(try wireDecode(Data([0x47, 0x46])))
    }

    func testRejectBadMagic() {
        XCTAssertThrowsError(try wireDecode(Data([0, 0, 1, 1, 0, 0, 0, 0])))
    }

    func testRejectCRCMismatch() {
        let pkt = GraftPacket(type: 0x01, seq: 0)
        var encoded = wireEncode(pkt)
        encoded[7] ^= 0xFF
        XCTAssertThrowsError(try wireDecode(encoded))
    }

    func testCRC8Basic() {
        XCTAssertEqual(CRC8.compute([]), 0x00)
        XCTAssertEqual(CRC8.compute([0x00]), 0x00)
        XCTAssertEqual(CRC8.compute([0x01]), 0x31)
    }
}
