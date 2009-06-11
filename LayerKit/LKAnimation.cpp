/*  
 * Copyright (C) 2007 University of South Australia
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * Contributors: 
 *   Andrew Cunningham <andrewcunningham@mac.com>
 */
#include "LKAnimation.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include "LKlayer.h"
#include "LKEngine.h"
#include "LKUtil.h"
#include "platform/MathExtras.h"
#include <iostream>
using namespace std;
using boost::bind;

#define foreach BOOST_FOREACH


template <typename T>
inline T sclip(const T& v, const T& min, const T& max)
{
    if (v > 0 && v > max)
        return max;
    else if (v < 0 && v < min)
        return min;
    return v;
}


/********************************************************************/
/**                                                                **/
/**                        LKAnimation Class                       **/
/**                                                                **/
/********************************************************************/
LKAnimatable::LKAnimatable(LKAnimator* animator, bool autoCleanup)
	: m_animator(animator)
	, m_isRunning(false)
{
}

void LKAnimatable::start(void)
{
	if (m_isRunning)
		return;
	m_isRunning  = true;
	m_animator->addPropertyAnimator(this);
}

bool LKAnimatable::isRunning(void) const
{
	return m_isRunning;
}

void LKAnimatable::stop(void)
{
	if (m_isRunning)
		m_animator->removePropertyAnimator(this);
	m_isRunning = false;
}

////////////////////////////////////////////////////////////////////

LKAnimation::LKAnimation(LKAnimator* animator, bool autoCleanup)
	: LKAnimatable(animator, autoCleanup)
	, m_startTicks(LKEngine::getTicks())
{ 
}

LKAnimation::~LKAnimation(void)
{
}

void LKAnimation::start(void)
{
	if (m_isRunning)
		return;
	m_startTicks = LKEngine::getTicks();
	m_isRunning  = true;
	m_animator->addPropertyAnimator(this);
}

////////////////////////////////////////////////////////////////////

template <typename T>
LKPropertyBaseAnimator<T>::LKPropertyBaseAnimator(void)
	: LKAnimation(NULL) 
{
}

template <typename T>
LKPropertyBaseAnimator<T>::LKPropertyBaseAnimator(LKAnimator* animator, 
												  GetPropertyDelegate getDelegate, 
												  SetPropertyDelegate setDelegate, 
												  T targetValue, 
												  int duration)
	: LKAnimation(animator)
	, m_getDelegate(getDelegate)
	, m_setDelegate(setDelegate)
	, m_startValue(getDelegate())
	, m_targetValue(targetValue)
	, m_duration(duration)
{	
}

template <typename T>
void LKPropertyBaseAnimator<T>::reset(T value)
{
	m_startValue  = m_getDelegate();
	m_targetValue = value;
	m_startTicks  = LKEngine::getTicks();
}

template <typename T>
bool LKPropertyBaseAnimator<T>::update(int ticks)
{
	int dtick = ticks - m_startTicks;
	
	if (dtick > m_duration){
		m_setDelegate(m_targetValue);
		return true;
	} else {
		double nd = dtick / float(m_duration); // normalised delta 
		double v  = (1 - cos(nd * M_PI)) / 2;
		T      r  = m_startValue + (m_targetValue - m_startValue) * v;
		m_setDelegate(r);
	}

	return false;
}

template struct LKPropertyBaseAnimator<Coord2d>;
template struct LKPropertyBaseAnimator<double>;

////////////////////////////////////////////////////////////////////

LKRandomGyrationAnimator::LKRandomGyrationAnimator(LKAnimator* animator, 
	GetPropertyDelegate getDelegate, SetPropertyDelegate setDelegate,
	double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
	: LKAnimation(animator)
	, m_getDelegate(getDelegate)
	, m_setDelegate(setDelegate)
	, m_limits(xmax, ymax, zmax)
	, m_maxAcceleration(0.0025)
	, m_timeLimit(600)
{	
}

bool LKRandomGyrationAnimator::update(int ticks)
{
	int dtick = ticks - m_lastTicks;
	Coord3d curpos = m_getDelegate();

	if (dtick > m_timeLimit){
		double rx = (rand() % 100) / 100.0;
		double ry = (rand() % 100) / 100.0;
		double rz = (rand() % 100) / 100.0;
		Coord3d target(rx * m_limits.x, ry * m_limits.y, rz * m_limits.z);
		m_lastTicks    = ticks;
		m_acceleration = (target - curpos) * m_maxAcceleration;
	}

	m_velocity *= 0.9;
	m_velocity += m_acceleration;
	curpos     += m_velocity;

	m_setDelegate(curpos);

	return false;
}

void LKRandomGyrationAnimator::setGyration(double v)
{
	m_timeLimit = 300 + 300 * (1 - v);
	m_maxAcceleration = 0.005 * v;
}

/********************************************************************/
/**                                                                **/
/**                         LKAnimator Class                       **/
/**                                                                **/
/********************************************************************/
LKAnimator::LKAnimator(LKLayer* target)
    : m_target(target)
	, m_positionAnimator(NULL)
	, m_rotationAnimator(NULL)
	, m_scaleAnimator(NULL)
	, m_opacityAnimator(NULL)
    , m_position(target->m_position)
    , m_rotation(target->m_rotation)
    , m_positionflag(false, false, false)
    , m_rotationflag(false, false, false)
{
	m_opacityAnimator = new LKDoubleAnimator(
							this,
							boost::bind(&LKLayer::opacity, m_target),
							boost::bind(&LKLayer::setOpacity, m_target, _1),
							m_target->opacity(),
							1500);
	m_positionAnimator = new LKCoord3dAnimator(
							this,
							boost::bind(&LKLayer::position, m_target),
							boost::bind(&LKAnimator::setlayerPosition, this, _1),
							m_target->m_position);
	m_rotationAnimator = new LKCoord3dAnimator(
							this,
							boost::bind(&LKLayer::rotation, m_target),
							boost::bind(&LKAnimator::setLayerRotation, this, _1),
							m_target->m_rotation);
	m_scaleAnimator = new LKCoord3dAnimator(
						this,
						boost::bind(&LKLayer::scale, m_target),
						boost::bind(&LKAnimator::setLayerScale, this, _1),
						m_target->m_scale);
}

LKAnimator::~LKAnimator(void)
{
}

const Coord3d& LKAnimator::position(void) const
{
    return m_position;
}

void LKAnimator::setPosition(const Coord3d& pos)
{	
	if (m_target->m_position == pos){
		if (m_positionAnimator->isRunning())
			m_positionAnimator->stop();
		return;
	}

	if (m_positionAnimator->isRunning() && m_positionAnimator->m_targetValue == pos)
		return;

	m_positionAnimator->m_startValue  = m_target->m_position;
	m_positionAnimator->m_targetValue = pos;

	if (!m_positionAnimator->isRunning())
		m_positionAnimator->start();
}

void LKAnimator::setPosition(const double& x, const double& y, const double& z)
{
	setPosition(Coord3d(x, y, z));
}

void LKAnimator::setXPosition(const double x)
{
    m_position.x = x;
    m_positionflag.x = true;
}

void LKAnimator::setYPosition(const double y)
{
    m_position.y = y;
    m_positionflag.y = true;
}

void LKAnimator::setZPosition(const double z)
{
    m_position.z = z;
    m_positionflag.z = true;
}

const Coord3d& LKAnimator::rotation(void) const
{
    return m_rotation;
}

void LKAnimator::setRotation(const Coord3d& rot)
{	
	if (m_target->m_rotation == rot){
		m_rotationAnimator->stop();
		return;
	}

	if (m_rotationAnimator->isRunning() && m_rotationAnimator->m_targetValue == rot)
		return;

	m_rotationAnimator->m_startValue  = m_target->m_rotation;
	m_rotationAnimator->m_targetValue = rot;

	if (!m_rotationAnimator->isRunning())
		m_rotationAnimator->start();
}

void LKAnimator::setRotation(const double& x, const double& y, const double& z)
{	
	setRotation(Coord3d(x, y, z));
}

const Coord3d& LKAnimator::scale(void) const
{
    return m_scale;
}

void LKAnimator::setScale(const double s)
{
	setScale(s, s, s);
}

void LKAnimator::setScale(const double& x, const double& y, const double& z)
{
	setScale(Coord3d(x, y, z));
}

void LKAnimator::setScale(const Coord3d& scale)
{
	if (m_target->m_scale == scale){
		if (m_scaleAnimator->isRunning())
			m_scaleAnimator->stop();
		return;
	}

	if (m_scaleAnimator->isRunning() && m_scaleAnimator->m_targetValue == scale)
		return;

	m_scaleAnimator->m_startValue  = m_target->m_scale;
	m_scaleAnimator->m_targetValue = scale;

	if (!m_scaleAnimator->isRunning())
		m_scaleAnimator->start();
}
	
void LKAnimator::setOpacity(const double v)
{
	if (m_opacityAnimator->isRunning() && m_opacityAnimator->m_targetValue == v)
		return;

	if (m_opacityAnimator->isRunning())
		m_opacityAnimator->stop();
	
	if (m_target->m_opacity == v)
		return;

	m_opacityAnimator->reset(v);
	m_opacityAnimator->start();
}
	
LKAnimatable* LKAnimator::positionAnimation(void) const
{
	return m_positionAnimator;
}
	
void LKAnimator::setlayerPosition(const Coord3d& pos)
{
    m_target->m_position = pos;
}

void LKAnimator::setlayerPosition(const double& x, const double& y, const double& z)
{	
    m_target->m_position.set(x, y, z);
}

void LKAnimator::setLayerRotation(const Coord3d& rot)
{
    m_target->m_rotation = rot;
}

void LKAnimator::setLayerRotation(const double& x, const double& y, const double& z)
{
    m_target->m_rotation.set(x, y, z);
}

void LKAnimator::setLayerScale(const Coord3d& scale)
{
    m_target->m_scale = scale;
}

void LKAnimator::setLayerScale(const double& x, const double& y, const double& z)
{
    m_target->m_scale.set(x, y, z);
}

void LKAnimator::setLayerOpacity(const double v)
{
	m_target->m_opacity = v;
}

void LKAnimator::addPropertyAnimator(LKAnimatable* anim)
{
	m_propertyAnimations.push_back(anim);
}

LKAnimation* LKAnimator::addPropertyAnimator(LKPropertyAnimator::GetPropertyDelegate getDel,
											 LKPropertyAnimator::SetPropertyDelegate setDel,
											 double targetValue)
{
	//if (getDel() == targetValue)
	//	return NULL;

	LKPropertyAnimator* anim = new LKPropertyAnimator(this, getDel, setDel, targetValue);
	anim->start();
	return anim;
}

void LKAnimator::removePropertyAnimator(LKAnimatable* animation)
{
	list<LKAnimatable*> out;
	foreach (LKAnimatable* anim, m_propertyAnimations)
		if (anim != animation)
			out.push_back(anim);
	m_propertyAnimations = out;
}

/********************************************************************/
/**                                                                **/
/**                      LKLinearAnimator Class                    **/
/**                                                                **/
/********************************************************************/
LKLinearAnimator::LKLinearAnimator(LKLayer* target)
    : LKAnimator(target)
{
}

/** calcuates the deltas for a linear animation. */
inline bool calcLinearAnimation(const Coord3d& src, 
								const Coord3d& dst, 
								Coord3<bool>& activeFlags, 
								long millisecondsPast, 
								Coord3d& out, 
								const double maxspeed, 
								bool isRotation = false)
{
    if (!activeFlags.x && !activeFlags.y && !activeFlags.z)
        return false;
    double x = (activeFlags.x) ? src.x + sclip(dst.x - src.x, -maxspeed, maxspeed) * (millisecondsPast / 1000.0) : src.x;
    double y = (activeFlags.y) ? src.y + sclip(dst.y - src.y, -maxspeed, maxspeed) * (millisecondsPast / 1000.0) : src.y;
    double z = (activeFlags.z) ? src.z + sclip(dst.z - src.z, -maxspeed, maxspeed) * (millisecondsPast / 1000.0) : src.z;
    activeFlags.x = activeFlags.x && (x != src.x);
    activeFlags.y = activeFlags.y && (y != src.y);
    activeFlags.z = activeFlags.z && (z != src.z);
    out.set(x, y, z);
    return true;
}

void LKLinearAnimator::update(long millisecondsPast)
{
    Coord3d out;
    if (calcLinearAnimation(m_target->rotation(), m_rotation, m_rotationflag, millisecondsPast, out, 360.0))
        setLayerRotation(out.x, out.y, out.z);
    if (calcLinearAnimation(m_target->scale(), m_scale, m_scaleflag, millisecondsPast, out, 5.85))
        setLayerScale(out.x, out.y, out.z);

	// handle property animators
	if (!m_propertyAnimations.empty()){
		
		std::list<LKAnimatable*> deleteList;
		int ticks = LKEngine::getTicks();
		
		foreach (LKAnimatable* anim, m_propertyAnimations)
			if (anim->update(ticks))
				deleteList.push_back(anim);
		foreach (LKAnimatable* anim, deleteList)
			anim->stop();
	}
}