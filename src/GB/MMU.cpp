#include "MMU.h"

constexpr MemoryArea MMU::GetMemoryArea(const u16 address) const
{
    if (address <= 0x3FFF)
    {
        return MemoryArea::ROM_BANK_0;
    }
    else if (address > 0x3FFF && address <= 0x7FFF)
    {
        return MemoryArea::ROM_BANK_N;
    }
    else if (address > 0x7FFF && address <= 0x9FFF)
    {
        return MemoryArea::VIDEO_RAM;
    }
    else if (address > 0x9FFF && address <= 0xBFFF)
    {
        return MemoryArea::EXTERNAL_RAM;
    }
    else if (address > 0xBFFF && address <= 0xCFFF)
    {
        return MemoryArea::WORK_RAM_BANK_0;
    }
    else if (address > 0xCFFF && address <= 0xDFFF)
    {
        return MemoryArea::WORK_RAM_BANK_1;
    }
    else if (address > 0xDFFF && address <= 0xFDFF)
    {
        return MemoryArea::ECHO_RAM;
    }
    else if (address > 0xFDFF && address <= 0xFE9F)
    {
        return MemoryArea::OAM;
    }
    else if (address > 0xFE9F && address <= 0xFEFF)
    {
        return MemoryArea::PROHIBITIED;
    }
    else if (address > 0xFEFF && address <= 0xFF7F)
    {
        return MemoryArea::IO_REGISTERS;
    }
    else if (address > 0xFF7F && address <= 0xFFFE)
    {
        return MemoryArea::HIGH_RAM;
    }
    else if (address == 0xFFFF)
    {
        return MemoryArea::INTERRUPT_ENABLE_REGISTER;
    }
}
