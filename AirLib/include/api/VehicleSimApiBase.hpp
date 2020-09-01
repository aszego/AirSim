// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_VehicleSimApiBase_hpp
#define air_VehicleSimApiBase_hpp

#include "common/CommonStructs.hpp"
#include "common/UpdatableObject.hpp"
#include "common/ImageCaptureBase.hpp"
#include "physics/Kinematics.hpp"
#include "physics/Environment.hpp"
#include "common/AirSimSettings.hpp"

namespace msr { namespace airlib {

class VehicleSimApiBase : public msr::airlib::UpdatableObject {
public:
    virtual ~VehicleSimApiBase() = default;

    //default implementation so derived class doesn't have to call on UpdatableObject
    virtual void reset() override
    {
        UpdatableObject::reset();
    }
    virtual void update() override
    {
        UpdatableObject::update();
    }

    //this method is called at every render tick when we want to transfer state from
    //physics engine to render engine. As physics engine is halted while 
    //this happens, this method should do minimal processing
    virtual void updateRenderedState(float dt)
    {
        unused(dt);
        //derived class should override if needed
    }
    //called when render changes are required at every render tick
    virtual void updateRendering(float dt)
    {
        unused(dt);
        //derived class should override if needed
    }

    virtual const ImageCaptureBase* getImageCapture() const = 0;
    virtual ImageCaptureBase* getImageCapture()
    {
        return const_cast<ImageCaptureBase*>(static_cast<const VehicleSimApiBase*>(this)->getImageCapture());
    }

    virtual std::vector<ImageCaptureBase::ImageResponse> getImages(const std::vector<ImageCaptureBase::ImageRequest>& request) const = 0;
    virtual std::vector<uint8_t> getImage(const std::string& camera_name, ImageCaptureBase::ImageType image_type) const = 0;

    virtual Pose getPose() const = 0;
    virtual void setPose(const Pose& pose, bool ignore_collision) = 0;
    virtual const Kinematics::State* getGroundTruthKinematics() const = 0;
    virtual const msr::airlib::Environment* getGroundTruthEnvironment() const = 0;

    virtual CameraInfo getCameraInfo(const std::string& camera_name) const = 0;
    virtual void setCameraOrientation(const std::string& camera_name, const Quaternionr& orientation) = 0;

    virtual CollisionInfo getCollisionInfo() const = 0;
    virtual int getRemoteControlID() const = 0; //which RC to use, 0 is first one, -1 means disable RC (use keyborad)
    virtual RCData getRCData() const = 0; //get reading from RC from simulator's host OS
    virtual std::string getVehicleName() const = 0;
    virtual std::string getRecordFileLine(bool is_header_line) const = 0;
    virtual void toggleTrace() = 0;
    virtual void enableBackMirror(bool is_enabled) = 0;
    virtual void enableTopCamera(bool is_enabled) = 0;
	virtual void simSwitchDayLightState(bool is_daylight_on) = 0;
	virtual void simSwitchAutoPilotMod(bool is_autopilot_on) = 0;
	virtual void simSwitchFogMod(bool is_fogmod_on) = 0;
	virtual void simReverseGear() = 0;
	virtual void simForwardGear() = 0;
	virtual void simSetAutoPilotTargetSpeed(float targetSpeed, float deltaSeconds) = 0;
	virtual void simSetAutoPilotTargetSpeed_0f5_4() = 0;
	virtual void simSetAutoPilotTargetSpeed_0_4() = 0;
	virtual void simSetAutoPilotTargetSpeed_1_4() = 0;
	virtual void simSetTrafficLightRed() = 0;
	virtual void simSetTrafficLightRedYellow() = 0;
	virtual void simSetTrafficLightYellow() = 0;
	virtual void simSetTrafficLightGreen() = 0;
    //use pointer here because of derived classes for VehicleSetting
    const AirSimSettings::VehicleSetting* getVehicleSetting() const
    {
        return AirSimSettings::singleton().getVehicleSetting(getVehicleName());
    }

};

} } //namespace
#endif
