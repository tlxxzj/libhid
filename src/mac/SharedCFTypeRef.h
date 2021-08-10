#ifndef SHAREDCFTYPEREF_H
#define SHAREDCFTYPEREF_H

#include <vector>
#include <CoreFoundation/CoreFoundation.h>
#include <iostream>

namespace libhid {
    template<typename T>
    class SharedCFTypeRef {
        protected:
            T m_ref = nullptr;
        
        public:

            SharedCFTypeRef(T ref = nullptr, bool retain = false):
            m_ref(ref) {
                if(ref && retain)
                    CFRetain(m_ref);
            }

            SharedCFTypeRef(const SharedCFTypeRef<T> & other):
            m_ref(other.m_ref) {
                if(m_ref)
                    CFRetain(m_ref);
            }

            SharedCFTypeRef(SharedCFTypeRef<T> && other):
            m_ref(other.m_ref) {
                other.m_ref = nullptr;
            }

            SharedCFTypeRef & operator=(const SharedCFTypeRef<T> & other) {
                if(other.m_ref)
                    CFRetain(other.m_ref);
                if(m_ref)
                    CFRelease(m_ref);
                m_ref = other.m_ref;
                return *this;
            }

            SharedCFTypeRef & operator=(SharedCFTypeRef<T> && other) {
                if(m_ref != other.m_ref) {
                    if(m_ref)
                        CFRelease(m_ref);
                    m_ref = other.m_ref;
                    other.m_ref = nullptr;
                }
                return *this;
            }

            ~SharedCFTypeRef() {
                if(m_ref) {
                    //std::cerr<<"Retain Count: "<<getRetainCount()<<std::endl;
                    CFRelease(m_ref);
                }
            }

            T get() {
                return m_ref;
            }

            int64_t getRetainCount() {
                if(m_ref)
                    return CFGetRetainCount(m_ref);
                return 0;
            }

            operator T() {
                return m_ref;
            }
    };
}

#endif