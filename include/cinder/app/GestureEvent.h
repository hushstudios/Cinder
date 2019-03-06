/*
 Copyright (c) 2019, The Cinder Project: http://libcinder.org
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
    the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
    the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Quaternion.h"
#include "cinder/Vector.h"
#include "cinder/app/Event.h"

#include <vector>

namespace cinder {
namespace app {

//! Represents a gesture event
class CI_API GestureEvent : public Event {
  public:
	enum Type { PAN, ZOOM, ROTATE, COUNT, UNKNOWN };
	enum Flags { BEGIN = 0x1, INERTIA = 0x2, END = 0x4 };

	GestureEvent() = default;
	explicit GestureEvent( const WindowRef &win, Type type, uint32_t flags = 0, void *native = nullptr )
		: Event( win )
		, mType( type )
		, mNative( native )
		, mFlags( flags )
	{
	}

	//! Returns the gesture type.
	Type getType() const { return mType; }

	//! Returns the position. For multi-touch gestures, this usually is the center position.
	const vec2 &getPos() const { return mPosition; }
	//! Returns the velocity for pan gestures.
	const vec2 &getVelocity() const { return mVelocity; }
	//! Returns the scale for zoom gestures.
	float getScale() const { return mScale; }
	//! Returns the angle in radians for rotate gestures.
	float getAngle() const { return mAngle; }
	//! Returns the rotation quaternion for rotate gestures.
	quat getRotation() const { return rotate( mAngle, vec3( 0, 0, 1 ) ); }
	//! Returns whether this is the first event of the gesture.
	bool isBegin() const { return bool( mFlags & BEGIN ); }
	//! Returns whether this is the last event of the gesture.
	bool isEnd() const { return bool( mFlags & END ); }

	//! Returns a pointer to the OS-native object. This is a UIEvent* on Cocoa Touch, a NSEvent* on OSX, and a nullptr on MSW.
	const void *getNative() const { return mNative; }

  protected:
	Type     mType = UNKNOWN;
	vec2     mPosition{ 0 };
	vec2     mVelocity{ 0 };
	float    mScale{ 1 };
	float    mAngle{ 0 };
	void *   mNative = nullptr;
	uint32_t mFlags = 0;
};

class CI_API GesturePanEvent : public GestureEvent {
  public:
	GesturePanEvent( const WindowRef &win, const vec2 &position, const vec2 &velocity = vec2(), uint32_t flags = 0, void *native = nullptr )
		: GestureEvent( win, PAN, flags, native )
	{
		mPosition = position;
		mVelocity = velocity;
	}
	GesturePanEvent( const GestureEvent &other )
		: GestureEvent( other )
	{
	}
};

class CI_API GestureRotateEvent : public GestureEvent {
  public:
	GestureRotateEvent( const WindowRef &win, const vec2 &center, float radians, uint32_t flags = 0, void *native = nullptr )
		: GestureEvent( win, ROTATE, flags, native )
	{
		mPosition = center;
		mAngle = radians;
	}
	GestureRotateEvent( const GestureEvent &other )
		: GestureEvent( other )
	{
	}
};

class CI_API GestureZoomEvent : public GestureEvent {
  public:
	GestureZoomEvent( const WindowRef &win, const vec2 &center, float scale, uint32_t flags = 0, void *native = nullptr )
		: GestureEvent( win, ZOOM, flags, native )
	{
		mPosition = center;
		mScale = scale;
	}
	GestureZoomEvent( const GestureEvent &other )
		: GestureEvent( other )
	{
	}
};

CI_API inline std::ostream &operator<<( std::ostream &out, const GestureEvent &event )
{
	out << "{" << ( event.getType() == GestureEvent::PAN ? " Pan " : event.getType() == GestureEvent::ROTATE ? " Rotate " : " Zoom " );
	out << "}";
	return out;
}

} // namespace app
} // namespace cinder
