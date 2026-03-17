"""GRAFT PID Controller Module"""

from __future__ import annotations

import struct
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from graft.device import GraftDevice

from graft.core.constants import CMD


class GraftPID:
    def __init__(self, device: GraftDevice, module_id: int = 0) -> None:
        self._device = device
        self._mod_id = module_id

    async def config(self, kp: float, ki: float, kd: float) -> None:
        payload = bytes([self._mod_id, 0x01]) + struct.pack("<fff", kp, ki, kd)
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def set_target(self, value: float) -> None:
        payload = bytes([self._mod_id, 0x02]) + struct.pack("<f", value)
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def set_input(self, pin: int) -> None:
        payload = bytes([self._mod_id, 0x03, pin])
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def set_output(self, pin: int) -> None:
        payload = bytes([self._mod_id, 0x04, pin])
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def enable(self) -> None:
        payload = bytes([self._mod_id, 0x05, 1])
        await self._device._send_command(CMD.MOD_CMD, payload)

    async def disable(self) -> None:
        payload = bytes([self._mod_id, 0x05, 0])
        await self._device._send_command(CMD.MOD_CMD, payload)
