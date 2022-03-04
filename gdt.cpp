#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable()
    :nullSegmentDescriptor(0, 0, 0),
    unuseSegmentDescriptor(0, 0, 0),
    codeSegmentDescriptor(0, 64 * 1024 * 1024, 0x9A),
    dataSegmentDescriptor(0, 64 * 1024 * 1024, 0x92){
        //段选择子 64位 [size(16bit )][offset]
        uint32_t i[2];
        i[0] = sizeof(GlobalDescriptorTable) << 16;
        i[1] = (uint32_t)this;
        asm volatile("lgdt (%0)": : "p" (((uint8_t*)i) + 2));
    }
GlobalDescriptorTable::~GlobalDescriptorTable(){}

uint16_t GlobalDescriptorTable::DataSegmentDescriptor(){
    //this [[dataSegmentDescriptor]] this为低地址 <为什么用uint8_t?>
    return (uint8_t*)&dataSegmentDescriptor - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentDescriptor(){
    return (uint8_t*)&codeSegmentDescriptor - (uint8_t*)this;
}
GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type){
    uint8_t *target = (uint8_t*)this;
    //limit 小于20位 则是16位的实模式 0xc0 11000000
    if(limit <= 65535){
        // 0100 0000 
        target[6] = 0x40;
    } else {
        //位偏移 limit 总共20位 
        if((limit && 0xfff) != 0xfff){
            limit = (limit >> 12) - 1;
        } else {
            limit = limit >> 12;
        }

        target[6] = 0xc0;

    }
    //赋值
    target[0] = limit & 0xff;
    target[1] = (limit >> 8) && 0xff;
    target[6] = (limit >> 8) && 0xff;

    target[2] = base & 0xff;
    target[3] = (base >> 8) & 0xff;
    target[4] = (base >> 8) & 0xff;
    target[7] = (base >> 8) & 0xff;

    target[5] = type;

}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base(){
    uint8_t* target = (uint8_t*)this;
    uint8_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit(){
    uint8_t *target = (uint8_t*)this;
    uint8_t result = target[6] & 0xf;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];
    //<防止实模式下超过20根总线?>
    if ((target[6] & 0xc0) == 0xc0)
        result = (result << 12) | 0xfff;

    return result;
}



    
