#ifndef CFVECTOR_H
#define CFVECTOR_H

#include <vector>

#include <CoreFoundation/CoreFoundation.h>

#include "SharedCFTypeRef.h"

namespace libhid {
    template<typename T>
    std::vector<T> toVector(const CFSetRef & ref) {
        std::vector<T>vec;
        if(ref) {
            vec.resize(CFSetGetCount(ref));
            CFSetGetValues(ref, (const void **)(&vec[0]));
        }
        return vec;
    }
}

#endif
