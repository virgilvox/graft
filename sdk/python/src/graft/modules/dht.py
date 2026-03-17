"""GRAFT DHT Module"""

from __future__ import annotations

import struct
from dataclasses import dataclass
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from graft.device import GraftDevice

from graft.core.constants import CMD


@dataclass
class DHTReading:
    temperature: float
    humidity: float


class GraftDHT:
    def __init__(self, device: GraftDevice, module_id: int = 0) -> None:
        self._device = device
        self._mod_id = module_id

    async def begin(self, pin: int, sensor_type: int = 22) -> None:
        payload = bytes([self._mod_id, 0x01, pin, sensor_type])
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def read(self) -> DHTReading:
        payload = bytes([self._mod_id, 0x02])
        resp = await self._device._send_command(CMD.MOD_CMD, payload)
        if len(resp) >= 9:  # module_id(1) + 2 floats(8)
            temp, hum = struct.unpack_from("<ff", resp, 1)
            return DHTReading(temperature=temp, humidity=hum)
        return DHTReading(temperature=0.0, humidity=0.0)
