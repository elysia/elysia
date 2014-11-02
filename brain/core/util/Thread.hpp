/*  Elysia Utilities -- Elysia Synchronization Utilities
 *  Thread.hpp
 *
 *  Copyright (c) 2009, Ewen Cheslack-Postava
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Elysia nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _ELYSIA_UTIL_THREAD_HPP_
#define _ELYSIA_UTIL_THREAD_HPP_

#include "Platform.hpp"

#if ELYSIA_PLATFORM != ELYSIA_PLATFORM_WINDOWS
#include <sys/time.h>
#endif

namespace Elysia {

/** Thread is a thin wrapper around std::thread which handles common
 *  thread initialization processes.  It is not intended to hide
 *  std::thread, but rather handle things like setting the thread up
 *  for profiling and crash handling.
 */
class Thread {
public:
    typedef std::function<void()> ThreadMainFunc;

    Thread() {
    }
    ~Thread() {
    }

    explicit Thread(String name, ThreadMainFunc f)
     : mImpl(
#if ELYSIA_PLATFORM != ELYSIA_PLATFORM_WINDOWS
         std::bind(
             &Thread::initThread, this, name, f
             , Thread::getProfilerData()
         )
#else
         std::bind(
             &Thread::initThread, this, name, f
         )
#endif
       )
    {
    }

    std::thread::id get_id() const {
        return mImpl.get_id();
    }

    bool joinable() const {
        return mImpl.joinable();
    }
    void join() {
        mImpl.join();
    }
    bool timed_join(const std::system_time& wait_until) {
        return mImpl.timed_join(wait_until);
    }

    template<typename TimeDuration>
    bool timed_join(TimeDuration const& rel_time) {
        return mImpl.timed_join(rel_time);
    }

    void detach() {
        mImpl.detach();
    }

    static unsigned hardware_concurrency() {
        return std::thread::hardware_concurrency();
    }

    void interrupt() {
        mImpl.interrupt();
    }

    bool interruption_requested() const {
        return mImpl.interruption_requested();
    }

    static void yield() {
        std::thread::yield();
    }

    static void sleep(const std::system_time& xt) {
        std::thread::sleep(xt);
    }
private:
#if ELYSIA_PLATFORM != ELYSIA_PLATFORM_WINDOWS
    typedef struct itimerval ProfilerData;
    static ProfilerData getProfilerData() {
        ProfilerData prof_data;
        getitimer(ITIMER_PROF, &prof_data);
        return prof_data;
    }

    static void setProfilerData(ProfilerData pd) {
        setitimer(ITIMER_PROF, &pd, NULL);
    }

    void initThread(String name, ThreadMainFunc f, ProfilerData pd) {
        setProfilerData(pd);
        f();
    }
#else
    void initThread(String name, ThreadMainFunc f) {
        f();
    }
#endif

    Thread(Thread& rhs);

    std::thread mImpl;
}; // class Thread

} // namespace Elysia

#endif //_ELYSIA_UTIL_THREAD_HPP_
