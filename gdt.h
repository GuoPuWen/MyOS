#ifndef __GDT_H
#define __GDH_H

#include "types.h"

class GlobalDescriptorTable {
public:
    class SegmentDescriptor {
    public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
        uint32_t Base();
        uint32_t Limit();
    private:
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_hi;
        uint8_t type;
        uint8_t flag_limit;
        uint8_t base_vhi;
    } __attribute__((packed));

    SegmentDescriptor nullSegmentDescriptor;
    SegmentDescriptor unuseSegmentDescriptor;
    SegmentDescriptor codeSegmentDescriptor;
    SegmentDescriptor dataSegmentDescriptor;


    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();


};
#endif