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


#include "BrainObjectScript.hpp"
using namespace Sirikata;
namespace Elysia {

#define WORLD_SCALE 20  // Units for zoom/pan
#define DEG2RAD 0.0174532925
class Heartbeat: public SelfWeakPtr<Heartbeat> {
    BrainObjectScript * mParent;
    Duration mHeartRate;
    std::tr1::function<void()> mCallParentHeartbeat;
public:
    Heartbeat(BrainObjectScript *parent, Duration heartRate):mParent(parent), mHeartRate(heartRate) {
    }
    void init () {
        mCallParentHeartbeat= std::tr1::bind(&Heartbeat::callParentHeartbeat,getWeakPtr());
        if (mParent)
            mParent->context()->ioService->post(mHeartRate,mCallParentHeartbeat);
    }
    static void callParentHeartbeat(const std::tr1::weak_ptr<Heartbeat> &val) {
        std::tr1::shared_ptr<Heartbeat> thus(val.lock());
        if (thus&&thus->mParent) {
            thus->mParent->heartbeat();
            thus->mParent->context()->ioService->post(thus->mHeartRate,thus->mCallParentHeartbeat);
        }
    }
    void destruct(){
        mParent=NULL;
    }
};
BrainObjectScript::BrainObjectScript(HostedObjectPtr ho, const String& args)
 : mParent(ho)
{
    mParent->addListener((SessionEventListener*)this);
    std::tr1::shared_ptr<Heartbeat> tmp(Heartbeat::construct<Heartbeat,BrainObjectScript*,Duration>(this,Duration::seconds(.25)));
    mHeartbeat=tmp;
    mHeartbeat->init();
    std::cout<<"INITIALIZE A SINGLE BRAIN SCRIPT\n";
}



void BrainObjectScript::heartbeat(){
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
BrainObjectScript::~BrainObjectScript()
{
    mParent->removeListener((SessionEventListener*)this);
    if (mHeartbeat) {
        mHeartbeat->destruct();
        std::tr1::weak_ptr<Heartbeat> lockTest(mHeartbeat);
        mHeartbeat=std::tr1::shared_ptr<Heartbeat>();
        //while(lockTest.lock());//if heartbeat() causes this to get deleted--then that loop could prove infinite
    }
}

Context* BrainObjectScript::context() const {
    return mParent->context();
}

void BrainObjectScript::updateAddressable()
{
}

void BrainObjectScript::attachScript(const String& script_name)
{
}

void BrainObjectScript::onConnected(SessionEventProviderPtr from, const SpaceObjectReference& name, int64 token) {
    mID = name;
    mSelfProxy = mParent->self(mID);
//    moveAction(Vector3f(0,0,1),10);
    
}

void BrainObjectScript::onDisconnected(SessionEventProviderPtr from, const SpaceObjectReference& name) {
}


static String fillZeroPrefix(const String& prefill, int32 nwide) {
    String retval = prefill;
    while((int)retval.size() < nwide)
        retval = String("0") + retval;
    return retval;
}

void BrainObjectScript::moveAction(Vector3f dir, float amount)
{
    // Get the updated position
    Time now = context()->simTime();
    Location loc = mSelfProxy->extrapolateLocation(now);
    const Quaternion &orient = loc.getOrientation();

    // Request updates from spcae
    TimedMotionVector3f newloc(now, MotionVector3f(Vector3f(loc.getPosition()), (orient * dir) * amount * WORLD_SCALE * .5) );
    mParent->requestLocationUpdate(mID.space(), mID.object(), newloc);
    // And update our local Proxy's information, assuming the move will be successful
    mSelfProxy->setLocation(newloc, 90000, true);
}

void BrainObjectScript::rotateAction(Vector3f about, float amount)
{
    // Get the updated position
    Time now = context()->simTime();
    Location loc = mSelfProxy->extrapolateLocation(now);
    const Quaternion &orient = loc.getOrientation();

    TimedMotionQuaternion neworient(now, MotionQuaternion(loc.getOrientation(), Quaternion(about, amount)));
    mParent->requestOrientationUpdate(mID.space(), mID.object(), neworient);
    // And update our local Proxy's information, assuming the move will be successful
    mSelfProxy->setOrientation(neworient, 0, true);
}

static bool quat2Euler(Quaternion q, double& pitch, double& roll, double& yaw) {
    /// note that in the 'gymbal lock' situation, we will get nan's for pitch.
    /// for now, in that case we should revert to quaternion
    double q1,q2,q3,q0;
    q2=q.x;
    q3=q.y;
    q1=q.z;
    q0=q.w;
    roll = std::atan2((2*((q0*q1)+(q2*q3))), (1-(2*(std::pow(q1,2.0)+std::pow(q2,2.0)))));
    pitch = std::asin((2*((q0*q2)-(q3*q1))));
    yaw = std::atan2((2*((q0*q3)+(q1*q2))), (1-(2*(std::pow(q2,2.0)+std::pow(q3,2.0)))));
    pitch /= DEG2RAD;
    roll /= DEG2RAD;
    yaw /= DEG2RAD;
    if (std::abs(pitch) > 89.0) {
        return false;
    }
    return true;
}


void BrainObjectScript::stableRotateAction(float dir, float amount)
{
    // Get the updated position
    Time now = context()->simTime();
    Location loc = mSelfProxy->extrapolateLocation(now);
    const Quaternion &orient = loc.getOrientation();

    double p, r, y;
    quat2Euler(orient, p, r, y);
    Vector3f raxis;
    raxis.x = 0;
    raxis.y = std::cos(p*DEG2RAD);
    raxis.z = -std::sin(p*DEG2RAD);

    // Request updates from spcae
    TimedMotionQuaternion neworient(now, MotionQuaternion(loc.getOrientation(), Quaternion(raxis, dir*amount)));
    mParent->requestOrientationUpdate(mID.space(), mID.object(), neworient);
    // And update our local Proxy's information, assuming the move will be successful
    mSelfProxy->setOrientation(neworient, 0, true);
}

} // namespace Elysia
