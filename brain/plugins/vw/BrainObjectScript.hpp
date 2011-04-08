/*  Sirikata
 *  SimpleCameraObjectScript.hpp
 *
 *  Copyright (c) 2010, Ewen Cheslack-Postava
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

#ifndef _SIRIKATA_SIMPLECAMERA_OBJECT_SCRIPT_HPP_
#define _SIRIKATA_SIMPLECAMERA_OBJECT_SCRIPT_HPP_

#include <sirikata/oh/ObjectScript.hpp>
#include <sirikata/oh/ObjectScriptManager.hpp>
#include <sirikata/oh/HostedObject.hpp>
#include <sirikata/proxyobject/SessionEventListener.hpp>
#include <sirikata/proxyobject/Invokable.hpp>
//#include "InputBinding.hpp"

namespace Elysia {

class BrainObjectScript :
        public Sirikata::ObjectScript,
        Sirikata::SessionEventListener
{
public:
    BrainObjectScript(HostedObjectPtr ho, const String& args);
    virtual ~BrainObjectScript();

    virtual void updateAddressable();
    virtual void attachScript(const Sirikata::String&);

    // SessionEventListener Interface
    virtual void onConnected(Sirikata::SessionEventProviderPtr from, const Sirikata::SpaceObjectReference& name, Sirikata::int64 token);
    virtual void onDisconnected(Sirikata::SessionEventProviderPtr from, const Sirikata::SpaceObjectReference& name);

private:
    Context* context() const;

    void suspendAction();
    void resumeAction();
    void toggleSuspendAction();
    void quitAction();

    void moveAction(Sirikata::Vector3f dir, float amount);
    void rotateAction(Sirikata::Vector3f about, float amount);
    void stableRotateAction(float dir, float amount);

    void screenshotAction();


    Sirikata::HostedObjectPtr mParent;
    Sirikata::SpaceObjectReference mID; // SimpleCamera only handles one presence
    Sirikata::ProxyObjectPtr mSelfProxy;

    Sirikata::Invokable* mGraphics;

//    Sirikata::InputBinding::InputResponseMap mInputResponses;
//    Sirikata::InputBinding mInputBinding;
};

} // namespace Elysia

#endif //_SIRIKATA_SIMPLECAMERA_OBJECT_SCRIPT_HPP_
