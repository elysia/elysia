/*  Sirikata
 *  SimpleCameraObjectScript.cpp
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


#include <sirikata/oh/Platform.hpp>

#include <sirikata/core/util/KnownServices.hpp>


#include "FoodObjectScript.hpp"
using namespace Sirikata;
namespace Elysia {

#define WORLD_SCALE 20  // Units for zoom/pan
#define DEG2RAD 0.0174532925
FoodObjectScript::FoodObjectScript(HostedObjectPtr ho, const String& args) : WorldObjectScript(ho,args) 
{

}



void FoodObjectScript::heartbeat(){
    std::cout<<"BEAT\n";
    if (rand()%2==1) {
        if (mSelfProxy) {
            std::cout<<"SET MESH\n";
            //String ground("meerkat:///jterrace/ground.DAE/original/0/ground.DAE");
            String ground("meerkat:///palmfreak/duck.DAE/optimized/1/duck.DAE");
            mParent->requestMeshUpdate(mID.space(),mID.object(),ground);
            mSelfProxy->setMesh(Sirikata::Transfer::URI(ground),0);
        }
    }

}
FoodObjectScript::~FoodObjectScript()
{
}

void FoodObjectScript::handleCommunication (const ODP::Endpoint& src, const ODP::Endpoint& dst, MemoryReference payload){

}

} // namespace Elysia
