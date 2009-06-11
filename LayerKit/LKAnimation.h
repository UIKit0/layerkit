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
#ifndef LKAnimation_h
#define LKAnimation_h

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include "math/Coord.h"
#include "platform/MathExtras.h"
using std::list;


// forward declarations
class  LKLayer;
class  LKAnimator;
struct LKAnimation;


struct LKAnimatable {
	LKAnimatable(LKAnimator* animator, bool autoCleanup=false);

	/** updates the animated property. Returns true if the animation is completed */
	virtual bool update(int ticks) = 0;	
	
	virtual void start(void);
	bool isRunning(void) const;
	virtual void stop(void);

	LKAnimator* m_animator;
	bool m_autoCleanup;
	bool m_isRunning;
};


/** an LKPropertyAnimator defines an arbitrary animation of any class
 *  property through bound methods. */
struct LKAnimation : public LKAnimatable {
	
	LKAnimation(LKAnimator* animator, bool autoCleanup=false);
	virtual ~LKAnimation(void);

	virtual void start(void);

	int  m_startTicks;
};


template <typename T>
struct LKPropertyBaseAnimator : public LKAnimation {
	
	typedef boost::function<T (void)>   GetPropertyDelegate;
	typedef boost::function<void (T t)> SetPropertyDelegate;

	LKPropertyBaseAnimator(void);
	LKPropertyBaseAnimator(LKAnimator* animator, GetPropertyDelegate getDelegate, SetPropertyDelegate setDelegate, T targetValue, int duration=1000);

	void reset(T value);
	bool update(int ticks);

	GetPropertyDelegate m_getDelegate;
	SetPropertyDelegate m_setDelegate;
	T   m_startValue;
	T   m_targetValue;
	int m_duration;
};

typedef LKPropertyBaseAnimator<double>  LKDoubleAnimator;
typedef LKPropertyBaseAnimator<double>  LKPropertyAnimator;
typedef LKPropertyBaseAnimator<Coord2d> LKCoord2dAnimator;
typedef LKPropertyBaseAnimator<Coord3d> LKCoord3dAnimator;


/** an animator that randomly moves a 3D coordinate within a fixed range
 *  of motion */
struct LKRandomGyrationAnimator : public LKAnimation {
	
	typedef boost::function<const Coord3d& (void)>   GetPropertyDelegate;
	typedef boost::function<void (const Coord3d& t)> SetPropertyDelegate;

	LKRandomGyrationAnimator(LKAnimator* animator, 
		GetPropertyDelegate getDelegate, SetPropertyDelegate setDelegate,
		double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);

	bool update(int ticks);
	void setGyration(double v);

	GetPropertyDelegate m_getDelegate;
	SetPropertyDelegate m_setDelegate;
	Coord3d m_limits;
	Coord3d m_acceleration;
	Coord3d m_velocity;
	double  m_maxAcceleration;
	int     m_lastTicks;
	int     m_timeLimit;
};

/** the main animator class. Most of these methods should be reflections
 *  of the LKLayer class */
class LKAnimator {
public:
	friend class LKLayer;

	LKAnimator(LKLayer* target);
    virtual ~LKAnimator(void);

    virtual void update(long millisecondsPast) = 0;

    const Coord3d& position(void) const;
    void setPosition(const Coord3d& pos);
    void setPosition(const double& x, const double& y, const double& z);
    void setXPosition(const double x);
    void setYPosition(const double y);
    void setZPosition(const double z);

    const Coord3d& rotation(void) const;
    void setRotation(const Coord3d& rot);
    void setRotation(const double& x, const double& y, const double& z);

    const Coord3d& scale(void) const;
    void setScale(const double s);
    void setScale(const Coord3d& scale);
    void setScale(const double& sx, const double& sy, const double& sz);

	const double opacity(void) const;
	void setOpacity(const double v);

	void addPropertyAnimator(LKAnimatable* anim);
	LKAnimation* addPropertyAnimator(LKPropertyAnimator::GetPropertyDelegate, 
									 LKPropertyAnimator::SetPropertyDelegate,
									 double targetValue);

	void removePropertyAnimator(LKAnimatable* animator);

	LKAnimatable* positionAnimation(void) const;

protected:
    // the following functions should be used to set the position, orientation
    // etc of the target layer. Using these methods will ensure that the animator
    // is no reset by the accessor methods
    void setlayerPosition(const Coord3d& pos);
    void setlayerPosition(const double& x, const double& y, const double& z);

    void setLayerRotation(const Coord3d& rot);
    void setLayerRotation(const double& x, const double& y, const double& z);
    
    void setLayerScale(const Coord3d& scale);
    void setLayerScale(const double& x, const double& y, const double& z);

	void setLayerOpacity(const double v);

protected:
	LKLayer*     m_target;
    
	LKCoord3dAnimator* m_positionAnimator;
	LKCoord3dAnimator* m_rotationAnimator;
	LKCoord3dAnimator* m_scaleAnimator;
	LKDoubleAnimator*  m_opacityAnimator;

	Coord3d      m_position;
    Coord3d      m_rotation;
    Coord3d      m_scale;
    Coord3<bool> m_positionflag;
    Coord3<bool> m_rotationflag;
    Coord3<bool> m_scaleflag;
	list<LKAnimatable*> m_propertyAnimations;
};


class LKLinearAnimator : public LKAnimator {
public:
    LKLinearAnimator(LKLayer* target);
	void update(long millisecondsPast);
};


#endif