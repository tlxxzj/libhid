#ifndef SHAREDCFTYPEREF_H
#define SHAREDCFTYPEREF_H

#include <vector>
#include <iostream>

#include <CoreFoundation/CoreFoundation.h>


namespace libhid {
    template<typename T>
    class SharedCFTypeRef {
        protected:
            T mRef = nullptr;
        
        public:

            SharedCFTypeRef(T ref = nullptr, bool retain = false):
            mRef(ref) {
                if(ref && retain)
                    CFRetain(mRef);
            }

            SharedCFTypeRef(const SharedCFTypeRef<T> & other):
            mRef(other.mRef) {
                if(mRef)
                    CFRetain(mRef);
            }

            SharedCFTypeRef(SharedCFTypeRef<T> && other):
            mRef(other.mRef) {
                other.mRef = nullptr;
            }

            SharedCFTypeRef & operator=(const SharedCFTypeRef<T> & other) {
                if(other.mRef)
                    CFRetain(other.mRef);
                if(mRef)
                    CFRelease(mRef);
                mRef = other.mRef;
                return *this;
            }

            SharedCFTypeRef & operator=(SharedCFTypeRef<T> && other) {
                if(mRef != other.mRef) {
                    if(mRef)
                        CFRelease(mRef);
                    mRef = other.mRef;
                    other.mRef = nullptr;
                }
                return *this;
            }

            ~SharedCFTypeRef() {
                if(mRef) {
                    //std::cerr<<"Retain Count: "<<getRetainCount()<<std::endl;
                    CFRelease(mRef);
                }
            }

            T get() {
                return mRef;
            }

            int64_t getRetainCount() {
                if(mRef)
                    return CFGetRetainCount(mRef);
                return 0;
            }

            operator T() {
                return mRef;
            }
    };
}

#endif