#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "SharedCFRunLoop.h"

namespace libhid {

    static CFRunLoopRef m_run_loop = nullptr;
    static std::thread m_run_loop_thread;
    static int32_t m_retain_count = 0;
    static std::mutex m_mutex;
    static std::condition_variable m_cv;


    // static functions

    CFRunLoopRef SharedCFRunLoop::runLoop() {
        std::lock_guard<std::mutex>lock(m_mutex);
        return m_run_loop;
    }

    void SharedCFRunLoop::initRunLoop() {
        std::unique_lock<std::mutex>lock(m_mutex);
        if(!m_run_loop) {
            m_retain_count = 0;
            m_run_loop_thread = std::thread([](){
                m_run_loop = (CFRunLoopRef)CFRetain(CFRunLoopGetCurrent());
                m_cv.notify_one();
                //CFRunLoopRun();
                CFTimeInterval interval = 10; /* seconds */
                while(true) {
                    CFRunLoopRunResult retsult = CFRunLoopRunInMode(kCFRunLoopDefaultMode, interval, false);
                    if(retsult == kCFRunLoopRunStopped)
                        break;
                }
                //std::cerr<<"Run loop exited!"<<std::endl;
            });
            m_run_loop_thread.detach();
            m_cv.wait(lock, [](){
                return m_run_loop != nullptr;
            });
        }
    }

    void SharedCFRunLoop::retain() {
        std::lock_guard<std::mutex>lock(m_mutex);
        m_retain_count++;
    }

    void SharedCFRunLoop::release() {
        std::lock_guard<std::mutex>lock(m_mutex);
        m_retain_count--;
        if(m_retain_count == 0) {
            CFRunLoopStop(m_run_loop);
            m_run_loop = nullptr;
        }
    }

    int32_t SharedCFRunLoop::retainCount() {
        std::lock_guard<std::mutex>lock(m_mutex);
        return m_retain_count;
    }

}