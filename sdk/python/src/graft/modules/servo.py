"""GRAFT Servo Module"""

from __future__ import annotations

import struct
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from graft.device import GraftDevice

from graft.core.constants import CMD


class GraftServo:
    def __init__(self, device: GraftDevice, module_id: int = 0) -> None:
        self._device = device
        self._mod_id = module_id

    async def attach(self, pin: int, min_us: int = 544, max_us: int = 2400) -> None:
        payload = bytes([self._mod_id, 0x01, pin,
                         min_us & 0xFF, (min_us >> 8) & 0xFF,
                         max_us & 0xFF, (max_us >> 8) & 0xFF])
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def write(self, angle: int) -> None:
        payload = bytes([self._mod_id, 0x02, angle & 0xFF])
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def write_microseconds(self, us: int) -> None:
        payload = bytes([self._mod_id, 0x03, us & 0xFF, (us >> 8) & 0xFF])
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def detach(self) -> None:
        payload = bytes([self._mod_id, 0x04])
        await self._device._send_command(CMD.MOD_CMD, payload)
