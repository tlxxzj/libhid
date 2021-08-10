#ifndef SHAREDCFRUNLOOP_H
#define SHAREDCFRUNLOOP_H

#include <CoreFoundation/CoreFoundation.h>


namespace libhid {
    class SharedCFRunLoop {
    protected:
        static void initRunLoop();
        static CFRunLoopRef runLoop();
        static void retain();
        static void release();
        
    public:
        SharedCFRunLoop() { initRunLoop(); retain(); };
        SharedCFRunLoop(const SharedCFRunLoop & other) { retain(); };
        SharedCFRunLoop(SharedCFRunLoop && other) { retain(); };
        SharedCFRunLoop & operator=(const SharedCFRunLoop & other) { return *this; };
        SharedCFRunLoop & operator=(SharedCFRunLoop && other) { return *this; };
        ~SharedCFRunLoop() { release(); };

        operator CFRunLoopRef() { return runLoop(); };
        CFRunLoopRef get() { return runLoop(); };

        static int32_t retainCount();
    };
}

#endif