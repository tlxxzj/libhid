#ifndef CFVECTOR_H
#define CFVECTOR_H

#include <vector>

#include <CoreFoundation/CoreFoundation.h>

namespace libhid {
    template<typename T>
    std::vector<T> toVector(const CFSetRef ref) {
        std::vector<T>vec;
        if(ref) {
            vec.resize(CFSetGetCount(ref));
            if(vec.size() > 0)
                CFSetGetValues(ref, (const void **)(&vec[0]));
        }
        return vec;
    }
}

#endif
