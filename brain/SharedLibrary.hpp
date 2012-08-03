/*  Sirikata Utilities -- Shared Library Loading
 *  SharedLibrary.hpp
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
 *  * Neither the name of Sirikata nor the names of its contributors may
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

#ifndef _SIRIKATA_SHARED_LIBRARY_HPP_
#define _SIRIKATA_SHARED_LIBRARY_HPP_

#ifdef _WIN32
struct HINSTANCE__;
#  define DL_HANDLE struct HINSTANCE__*
#else
#  define DL_HANDLE void*
#endif

namespace Elysia {

/** SharedLibrary represents a sharedally loadable module. This only handles
 *  loading, unloading, and symbol lookup.  This presumes nothing about the
 *  interface provided by the module.
 */
class BRAIN_CORE_EXPORT SharedLibrary {
public:
    SharedLibrary(const std::string& path);
    ~SharedLibrary();

    bool load();
    bool unload();

    void* symbol(const std::string& name) const;

    /** Get the standard platform-specific library filename prefix. */
    static std::string prefix();
    /** Get the standard, build specific platform-specific library filename postfix. */
    static std::string postfix();
    /** Get the standard platform-specific library filename extension. */
    static std::string extension();
    /** Construct a platform dependent filename for a shared library based
     *  on a path and library name.
     */
    static std::string filename(const std::string& path, const std::string& name);
    /** Construct a platform dependent filename for a shared library.  The
     *  result will be relative to the current path.
     */
    static std::string filename(const std::string& name);
    /**
     * Actually unloads program code of module in use
     * Useful to wait until relevant global variables are purged (c.f. Options)
     */
    static void gc(DL_HANDLE handle=NULL);
private:

    std::string mPath;
    DL_HANDLE mHandle;
}; // class SharedLibrary

} // namespace Sirikata

#endif //_SIRIKATA_SHARED_LIBRARY_HPP_
