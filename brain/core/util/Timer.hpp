/*  Elysia
 *  Timer.hpp
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

#ifndef _ELYSIA_TIMER_HPP_
#define _ELYSIA_TIMER_HPP_

#include "Platform.hpp"
#include "AtomicTypes.hpp"
#include "Time.hpp"

namespace Elysia {

struct TimerImpl;

class ELYSIA_EXPORT Timer {
    static Elysia::AtomicValue<Duration> sOffset;

public:
    /** Sleep for approximately the given duration.
     *
     *  \note Use this carefully: unless you're running on threads you've
     *  allocated yourself, using this is probably the wrong thing to do.
     */
    static void sleep(const Duration& dt);

    static void setSystemClockOffset(const Duration &skew);
    static Duration getSystemClockOffset();
    static Time getSpecifiedDate(const std::string&datestring);
    // Get offset to convert local time (the standard for most of this API) to UTC.
    static Duration getUTCOffset();

    /** Get the current local time as a string, e.g. 2002-Jan-01
     *  10:00:01.123456789. Useful for reporting the real date when something
     *  happened.
     */
    static String nowAsString();

    /** Get the current time, in UTC, as a string, e.g. 2002-Jan-01
     *  10:00:01.123456789. Useful for reporting the real date when something
     *  happened.
     */
    static String nowUTCAsString();

    Timer();
    ~Timer();

    void start();
    Time getTimerStarted()const;
    static Time now();
    /** Get the value returned by a recent call to now(). Avoids the overhead of
     *  actually checking the time if you know something else is checking it
     *  frequently enough
     */
    static Time recentNow();

    /** Get the (approximate) amount of time elapsed since the process started. */
    static Duration processElapsed();
    /** Get the (approximate) amount of time elapsed since the process started
     *  based on a recent check of the timer. This can be out of date, but is
     *  fairly accurate if you regularly check the time and much faster than
     *  processElapsed().
     */
    static Duration recentProcessElapsed();

    Duration elapsed()const;

private:
    TimerImpl* mStart;
}; // class Timer

} // namespace Elysia

#endif //_ELYSIA_TIMER_HPP_
