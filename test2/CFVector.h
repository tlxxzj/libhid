//
//  CFSetVector.hpp
//  test2
//
//  Created by Lang Tian on 2021/7/30.
//

#ifndef CFSetVector_h
#define CFSetVector_h

#include <vector>

#include <stdio.h>

#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>

#include <map>

#include <iostream>


template<typename T, typename... Args>
class CFVector: public std::vector<T>
{
};


template<typename T>
class CFVector<T>: public std::vector<T>
{
public:
    CFVector(const CFSetRef cf_set)
    {
        if(cf_set)
        {
            size_t size = CFSetGetCount(cf_set);
            this->resize(size);
            CFSetGetValues(cf_set, (const void **)(this->data()));
            CFRelease(cf_set);
        }
    }
    
};


template<typename T1, typename T2>
class CFVector<T1, T2>: public std::vector<T2>
{
public:
    CFVector(const CFSetRef cf_set)
    {
        if(cf_set)
        {
            size_t size = CFSetGetCount(cf_set);
            std::vector<T1> cf_set_values(size);
            CFSetGetValues(cf_set, (const void **)(cf_set_values.data()));
            CFRelease(cf_set);
            this->reserve(size);
            for(auto value: cf_set_values)
            {
                this->push_back(T2(value));
            }
        }
    }
};







#endif /* CFSetVector_h */
