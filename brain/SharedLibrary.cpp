/*  Sirikata Utilities -- Shared Library Loading
 *  SharedLibrary.cpp
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
#include "Platform.hpp"
#include "SharedLibrary.hpp"

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#else
#  include <dlfcn.h>
#endif

namespace Sirikata {

SharedLibrary::SharedLibrary(const std::string& path)
 : mPath(path),
   mHandle(NULL)
{
}
static bool unloadLibrary(DL_HANDLE lib) {
#ifdef _WIN32
    bool success = !FreeLibrary(lib);
#else
    bool success = dlclose(lib)==0;
#endif
    return success;
}
void SharedLibrary::gc(DL_HANDLE handle) {
    static std::vector<DL_HANDLE> *sHandles=new std::vector<DL_HANDLE>;
    if (handle) {
        if (sHandles==NULL)
            sHandles=new std::vector<DL_HANDLE>;
        sHandles->push_back(handle);
    }else if(sHandles){
        for (std::vector<DL_HANDLE>::iterator i=sHandles->begin();i!=sHandles->end();++i) {
            unloadLibrary(*i);
        }
        delete sHandles;
        sHandles=NULL;
    }
}
SharedLibrary::~SharedLibrary() {
    if (mHandle != NULL)
        gc(mHandle);
}

bool SharedLibrary::load() {
    if (mHandle != NULL)
        return true;

#ifdef _WIN32
    mHandle = LoadLibrary(mPath.c_str());
    if (mHandle == NULL) {
        DWORD errnum = GetLastError();
        std::cerr<<"Failed to open library "<<mPath<<": "<<errnum<<std::endl;
    }
#else
    mHandle = dlopen(mPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (mHandle == NULL) {
        const char *errorstr = dlerror();
        std::cerr<<"Failed to open library "<<mPath<<": "<<errorstr<<std::endl;
    }
#endif

    return (mHandle != NULL);
}

bool SharedLibrary::unload() {
    if (mHandle == NULL)
        return true;
    bool success=unloadLibrary(mHandle);
    mHandle=NULL;
    return success;
}

void* SharedLibrary::symbol(const std::string& name) const {
#ifdef _WIN32
    return (void*)GetProcAddress(mHandle, name.c_str());
#else
    return (void*)dlsym(mHandle, name.c_str());
#endif
}

std::string SharedLibrary::prefix() {
#ifdef _WIN32
    return "";
#else
    return "lib";
#endif
}

std::string SharedLibrary::postfix() {
#ifdef _DEBUG 
    return "_d";
#else
    return "";
#endif
}

std::string SharedLibrary::extension() {
#ifdef _WIN32
    return ".dll";
#elif defined(__APPLE__)
    return ".dylib";
#else
    return ".so";
#endif
}

std::string SharedLibrary::filename(const std::string& name) {
    return ( prefix() + name + postfix() + extension() );
}

std::string SharedLibrary::filename(const std::string& path, const std::string& name) {
#ifdef _WIN32
    const char file_separator = '\\';
#else
    const char file_separator = '/';
#endif

    std::string result = path;
    if (result.size() > 0 && result[result.size()-1] != file_separator)
        result += file_separator;
    result += filename(name);

    return result;
}


} // namespace Sirikata
