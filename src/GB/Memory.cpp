#include "Memory.h"
#include "Cartridge.h"
#include "GPU.h"
#include <format>

Memory::Memory()
    : workRam(0x8000, 0), highRam(0x7F, 0), ioRegisters(0x80, 0), cart(nullptr), bootRomEnabled(true)
{
    
}

Memory::~Memory()
{
    
}

u8 Memory::Read(u16 address) const
{
    MemoryArea currentMemoryArea = GetMemoryArea(address);

    if (currentMemoryArea == MemoryArea::BOOTROM)
    {
        return bootRom[address];
    }
    else if (currentMemoryArea == MemoryArea::ROM)
    {
        return cart ? cart->ReadRom(address) : 0xFF;
    }
    else if (currentMemoryArea == MemoryArea::VIDEO_RAM)
    {
        return gpu ? gpu->ReadVRAM(address) : 0xFF;
    }
    else if (currentMemoryArea == MemoryArea::EXTERNAL_RAM)
    {
        return cart ? cart->ReadRAM(address) : 0xFF;
    }
    else if (currentMemoryArea == MemoryArea::WORK_RAM)
    {
        return workRam[address - 0xC000];
    }
    else if (currentMemoryArea == MemoryArea::ECHO_RAM)
    {
        return workRam[address - 0xE000];
    }
    else if (currentMemoryArea == MemoryArea::OAM)
    {
        return gpu ? gpu->ReadOAM(address) : 0xFF;
    }
    else if (currentMemoryArea == MemoryArea::PROHIBITIED)
    {
        return 0xFF;
    }
    else if (currentMemoryArea == MemoryArea::IO_REGISTERS)
    {
        return ioRegisters[address - 0xFF00];
    }
    else if (currentMemoryArea == MemoryArea::HIGH_RAM)
    {
        return highRam[address - 0xFF80];
    }
    else if (currentMemoryArea == MemoryArea::INTERRUPT_ENABLE_REGISTER)
    {
        // Joypad Input
        if (address == 0xFF00)
        {

        }
        // Serial Transfer
        else if (address > 0xFF00 && address <= 0xFF02)
        {

        }
        // Timer and Divider
        else if (address > 0xFF03 && address <= 0xFF07)
        {

        }
        // Interrupts
        else if (address == 0xFF0F)
        {

        }
        // Audio
        else if (address > 0xFF0F && address <= 0xFF26)
        {

        }
        // Wave Pattern
        else if (address > 0xFF2F && address <= 0xFF3F)
        {

        }
        // GPU Registers
        else if (address > 0xFF3F && address <= 0xFF4B)
        {
            return gpu->ReadRegister(address);
        }
        // VRAM Bank Select
        else if (address == 0xFF4F)
        {

        }
        // Boot Rom Mapping Control
        if(address == 0xFF50)
        {

        }
        // VRAM DMA
        else if (address > 0xFF50 && address <= 0xFF55)
        {

        }
        // BG / OBJ Palettes
        else if (address > 0xFF67 && address <= 0xFF6B)
        {

        }
        // WRAM Bank Select
        else if (address == 0xFF70)
        {

        }
        //return ioRegisters[0x7F];
    }

    return 0xFF;
}

void Memory::Write(u16 address, u8 value)
{
    MemoryArea currentMemoryArea = GetMemoryArea(address);

    if (currentMemoryArea == MemoryArea::ROM)
    {
        if (cart)
        {
            cart->WriteRom(address, value);
        }
    }
    else if (currentMemoryArea == MemoryArea::VIDEO_RAM)
    {
        if (gpu)
        {
            gpu->WriteVRAM(address, value);
        }
    }
    else if (currentMemoryArea == MemoryArea::EXTERNAL_RAM)
    {
        if (cart)
        {
            cart->WriteRAM(address, value);
        }
    }
    else if (currentMemoryArea == MemoryArea::WORK_RAM)
    {
        workRam[address - 0xC000] = value;
    }
    else if (currentMemoryArea == MemoryArea::ECHO_RAM)
    {
        workRam[address - 0xE000] = value;
    }
    else if (currentMemoryArea == MemoryArea::OAM)
    {
        if(gpu)
        {
            gpu->WriteOAM(address, value);
        }
    }
    else if (currentMemoryArea == MemoryArea::PROHIBITIED)
    {
        return;
    }
    else if (currentMemoryArea == MemoryArea::IO_REGISTERS)
    {
        ioRegisters[address - 0xFF00] = value;

        // Joypad Input
        if (address == 0xFF00)
        {

        }
        // Serial Transfer
        else if (address > 0xFF00 && address <= 0xFF02)
        {

        }
        // Timer and Divider
        else if (address > 0xFF03 && address <= 0xFF07)
        {

        }
        // Interrupts
        else if (address == 0xFF0F)
        {

        }
        // Audio
        else if (address > 0xFF0F && address <= 0xFF26)
        {

        }
        // Wave Pattern
        else if (address > 0xFF2F && address <= 0xFF3F)
        {

        }
        // GPU Registers
        else if (address > 0xFF3F && address <= 0xFF4B)
        {
            gpu->SetRegister(address, value);
        }
        // VRAM Bank Select
        else if (address == 0xFF4F)
        {

        }
        // Boot Rom Mapping Control
        if (address == 0xFF50)
        {
            DisableBootRom();
        }
        // VRAM DMA
        else if (address > 0xFF50 && address <= 0xFF55)
        {

        }
        // BG / OBJ Palettes
        else if (address > 0xFF67 && address <= 0xFF6B)
        {

        }
        // WRAM Bank Select
        else if (address == 0xFF70)
        {

        }
    }
    else if (currentMemoryArea == MemoryArea::HIGH_RAM)
    {
        highRam[address - 0xFF80] = value;
    }
    else if (currentMemoryArea == MemoryArea::INTERRUPT_ENABLE_REGISTER)
    {
        ioRegisters[0x7F] = value;
    }
}

bool Memory::LoadBootRom(const std::string& bootRomPath)
{
    std::ifstream boot(bootRomPath, std::ios::binary | std::ios::ate);
    if (!boot)
    {
        return false;
    }

    size_t size = boot.tellg();
    bootRom.assign(size, 0);
    boot.seekg(0, std::ios::beg);

    boot.read(reinterpret_cast<char*>(bootRom.data()), size);

    output.open("logging/bootrom.txt");
    for (int i = 0; i < 256; i++)
    {
        output << "Position: " << std::format("{:#04X}", i) << " | Byte: " << std::format("{:#04X}", bootRom[i]) << "\n";
    }
    output.close();

    return true;
}

void Memory::SetCart(Cartridge* cart)
{
    this->cart = cart;
}

void Memory::SetGpu(GPU* gpu)
{
    this->gpu = gpu;
}

constexpr MemoryArea Memory::GetMemoryArea(const u16 address) const
{
    if (address < 0x100 && bootRomEnabled && !bootRom.empty())
    {
        return MemoryArea::BOOTROM;
    }
    else if (address < 0x8000)
    {
        return MemoryArea::ROM;
    }
    else if (address >= 0x8000 && address < 0xA000)
    {
        return MemoryArea::VIDEO_RAM;
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        return MemoryArea::EXTERNAL_RAM;
    }
    else if (address >= 0xC000 && address < 0xE000)
    {
        return MemoryArea::WORK_RAM;
    }
    else if (address >= 0xE000 && address < 0xFE00)
    {
        return MemoryArea::ECHO_RAM;
    }
    else if (address >= 0xFE00 && address < 0xFEA0)
    {
        return MemoryArea::OAM;
    }
    else if (address >= 0xFEA0 && address < 0xFF00)
    {
        return MemoryArea::PROHIBITIED;
    }
    else if (address >= 0xFF00 && address < 0xFF80)
    {
        return MemoryArea::IO_REGISTERS;
    }
    else if (address >= 0xFF80 && address < 0xFFFF)
    {
        return MemoryArea::HIGH_RAM;
    }
    else if (address == 0xFFFF)
    {
        return MemoryArea::INTERRUPT_ENABLE_REGISTER;
    }

    return MemoryArea::INVALID;
}
