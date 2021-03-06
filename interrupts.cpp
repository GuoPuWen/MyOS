#include "interrupts.h"

void printf(const char *str);

InterruptManager::GateDescriptor InterruptManager::inteeruptDescriptorTable[256];

void InterruptManager::SetInteeruptDescriptorTableEntry(
    uint8_t inteeruptNumber,
    uint16_t codeSegmentSelectOffSet,           // 代码段
    void (*handle)(),
    uint8_t DescriptorPrivilegeLevel,       //DPL
    uint8_t DescriptorType) {
        const uint8_t IDT_DESC_PRESENT = 0x80;
        inteeruptDescriptorTable[inteeruptNumber].handleAddressLowBits = ((uint32_t)handle) & 0xffff;
        inteeruptDescriptorTable[inteeruptNumber].handleAddressHighBits = ((uint32_t)handle >> 16) & 0xffff;
        inteeruptDescriptorTable[inteeruptNumber].gdtCodeSegmentSelector = codeSegmentSelectOffSet;
        inteeruptDescriptorTable[inteeruptNumber].access = IDT_DESC_PRESENT | 
                        ((DescriptorPrivilegeLevel & 0x03) << 5) |  DescriptorType;
        inteeruptDescriptorTable[inteeruptNumber].reserved = 0;  
}
//构造函数
InterruptManager::InterruptManager(GlobalDescriptorTable *gdt, uint16_t hardwareInterruptOffset){
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint16_t codeSegment = gdt->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xe; //中断门
    for(uint8_t i = 0;i < 255;i++){
        SetInteeruptDescriptorTableEntry(i, codeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    }
    //同步中断 异常
    SetInteeruptDescriptorTableEntry(0x00, codeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x01, codeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x02, codeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x03, codeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x04, codeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x05, codeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x06, codeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x07, codeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x08, codeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x09, codeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x0A, codeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x0B, codeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x0C, codeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x0D, codeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x0E, codeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x0F, codeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x10, codeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x11, codeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x12, codeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(0x13, codeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    // // 异步中断 中断
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, codeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, codeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, codeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, codeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, codeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, codeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, codeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, codeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, codeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, codeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, codeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, codeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, codeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, codeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);
    SetInteeruptDescriptorTableEntry(hardwareInterruptOffset + 0x31, codeSegment, &HandleInterruptRequest0x31, 0, IDT_INTERRUPT_GATE);

}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp){
    printf("interrupts");
    return esp;
}
