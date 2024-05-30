#include "resolve.h"

#include <iostream>

#define FOUND_ALL_DEF 0
#define MULTI_DEF 1
#define NO_DEF 2

std::string errSymName;

int callResolveSymbols(std::vector<ObjectFile> &allObjects);

void resolveSymbols(std::vector<ObjectFile> &allObjects) {
    int ret = callResolveSymbols(allObjects);
    if (ret == MULTI_DEF) {
        std::cerr << "multiple definition for symbol " << errSymName << std::endl;
        abort();
    } else if (ret == NO_DEF) {
        std::cerr << "undefined reference for symbol " << errSymName << std::endl;
        abort();
    }
}

/* bind each undefined reference (reloc entry) to the exact valid symbol table entry
 * Throw correct errors when a reference is not bound to definition,
 * or there is more than one definition.
 */
int callResolveSymbols(std::vector<ObjectFile> &allObjects)
{
    /* Your code here */
    // if found multiple definition, set the errSymName to problematic symbol name and return MULTIDEF;
    // if no definition is found, set the errSymName to problematic symbol name and return NODEF;
    for(auto &obj:allObjects)
    {
        for(auto &re:obj.relocTable)
        {
            int flag = 0;
            int flag_weak = 0;
            for(auto &o:allObjects)
            {
                for(auto &sym:o.symbolTable)
                {
                    if(re.name == sym.name && sym.bind == STB_GLOBAL && sym.index == SHN_COMMON && flag == 0)
                    {
                        re.sym = &sym;
                        flag_weak = 1;
                    }
                    if(re.name == sym.name && sym.bind == STB_GLOBAL && sym.index != SHN_UNDEF && sym.index != SHN_COMMON)
                    {
                        re.sym = &sym;
                        flag++;
                    }
                }
            }
            if(flag > 1)
            {
                errSymName = re.name;
                return MULTI_DEF;
            }
            if(flag == 0 && flag_weak == 1)
            {
                continue;
            }
            if(flag == 0)
            {
                errSymName = re.name;
                return NO_DEF;
            }
        }
    }

    return FOUND_ALL_DEF;
}
