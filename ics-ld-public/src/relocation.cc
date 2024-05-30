#include "relocation.h"
#include <iostream>

#include <sys/mman.h>

void handleRela(std::vector<ObjectFile> &allObject, ObjectFile &mergedObject, bool isPIE)
{
    /* When there is more than 1 objects, 
     * you need to adjust the offset of each RelocEntry
     */
    /* Your code here */
    uint64_t prev = 0;
    for(auto &obj:allObject)
    {
        for(auto &re:obj.relocTable)
        {
            re.offset += prev;
        }
        prev += obj.sections[".text"].size;
    }

    /* in PIE executables, user code starts at 0xe9 by .text section */
    /* in non-PIE executables, user code starts at 0xe6 by .text section */
    uint64_t userCodeStart = isPIE ? 0xe9 : 0xe6;
    uint64_t textOff = mergedObject.sections[".text"].off + userCodeStart;
    uint64_t textAddr = mergedObject.sections[".text"].addr + userCodeStart;
    /* Your code here */
    for(auto &obj:allObject)
    {
        for (auto &re : obj.relocTable) 
        {
            uint64_t relocAddr = re.offset + textOff + (uint64_t)mergedObject.baseAddr;
            uint64_t targetAddr = 0;
            if(isPIE || re.sym->type == 2)
            {
                targetAddr = re.sym->value - (re.offset + textAddr) + re.addend;
            }
            else
            {
                targetAddr = re.sym->value + re.addend;
            }
            *reinterpret_cast<int *>(relocAddr) = targetAddr; 
        }
    }
    
}