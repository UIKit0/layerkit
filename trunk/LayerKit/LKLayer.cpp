/*  
 * Copyright (C) 2007-2008 University of South Australia
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
#include "lklayer.h"

#include <algorithm>
#include <boost/foreach.hpp>
#include <iostream>
#include "platform/gl.h"
#include "platform/MathExtras.h"
#include "LKAnimation.h"

#define foreach BOOST_FOREACH


static int  g_ntag       = 1;
static bool g_debugLayer = false;


LKLayer::LKLayer(void)
    : m_tag(g_ntag++) 
    , m_isHidden(false)
    , m_bounds(-1, -1, 1, 1)
	, m_autoComputeBounds(false)
	, m_position(0, 0, 0)
    , m_scale(1, 1, 1)
	, m_opacity(1.0)
    , m_superlayer(NULL)
{
	m_animator = new LKLinearAnimator(this);
}

LKLayer::LKLayer(Coord3d position)
    : m_tag(g_ntag++) 
    , m_isHidden(false)
    , m_bounds(-1, -1, 1, 1)
	, m_autoComputeBounds(false)
	, m_position(position)
    , m_scale(1, 1, 1)
	, m_opacity(1.0)
    , m_superlayer(NULL)
{
	m_animator = new LKLinearAnimator(this);
}

LKLayer::LKLayer(Coord4d bounds)
    : m_tag(g_ntag++) 
    , m_isHidden(false)
    , m_bounds(bounds)
	, m_autoComputeBounds(false)
	, m_position(0, 0, 0)
    , m_scale(1, 1, 1)
	, m_opacity(1.0)
    , m_superlayer(NULL)
{	
	m_animator = new LKLinearAnimator(this);
}

LKLayer::LKLayer(Coord3d position, Coord4d bounds)
    : m_tag(g_ntag++) 
    , m_isHidden(false)
    , m_bounds(bounds)
	, m_autoComputeBounds(false)
	, m_position(position)
    , m_scale(1, 1, 1)
	, m_opacity(1.0)
    , m_superlayer(NULL)
{
	m_animator = new LKLinearAnimator(this);
}

LKLayer::~LKLayer(void)
{
    delete m_animator;
}

void LKLayer::draw(void)
{
}

void LKLayer::postDraw(void)
{
}

int LKLayer::tag(void) const
{
    return m_tag;
}

LKLayer* LKLayer::layerWithTag(const int tag)
{
    if (m_tag == tag)
        return this;
    foreach (LKLayer* layer, m_layers){
        LKLayer* l = layer->layerWithTag(tag);
        if (l)
            return l;
    }
    return NULL;
}

LKLayer* LKLayer::superlayer(void) const
{
    return m_superlayer;
}

std::vector<LKLayer*> LKLayer::sublayers(void) const
{
    return m_layers;
}

void LKLayer::addSublayer(LKLayer* layer)
{
    layer->m_superlayer = this;
    m_layers.push_back(layer);
}

void LKLayer::removeFromSuperlayer(void)
{
    typedef std::vector<LKLayer*> LayerVector;
    for (LayerVector::iterator itr = m_superlayer->m_layers.begin(); itr != m_superlayer->m_layers.end(); ++itr){
        if (*itr == this){
            m_layers.erase(itr);
            break;
        }
    }
}

Coord2d LKLayer::convertPointToLayer(const Coord2d& p, LKLayer& alayer)
{
    //         ..C
    //      ..D  .    DE / AE = CB / AB
    //    ..  .  .    DE = (CB/AB) * AE
    //   A....E..B    
    double y = p.y / m_position.z * alayer.position().z;
    double x = p.x / m_position.z * alayer.position().z;
    x /= alayer.scale().x;
    y /= alayer.scale().y;
    return Coord2d(x, y);
}
/*
Coord2d LKLayer::convertPointToLayer(const Coord2d& aPoint)
{
	Coord3d offset(0,0,0);
	convertFromVWorld(offset);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	GLdouble wx, wy, wz;
	gluProject(0, 0, offset.z, modelview, projection, viewport, &wx, &wy, &wz);

	winX = (float)(aPoint.x);
	winY = (float)viewport[3] - (float)aPoint.y;
	winZ = wz;
		
	int i = gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return Coord2d(posX - offset.x, posY - offset.y);
}
*/
Coord3d LKLayer::convertScreenToLayerWithZValue(const Coord2d aPoint, double zValue)
{
	Coord3d offset(0,0,0);
	convertFromVWorld(offset);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	GLdouble wx, wy, wz;
	gluProject(0, 0, zValue, modelview, projection, viewport, &wx, &wy, &wz);

	winX = float(aPoint.x);
	winY = float(aPoint.y);//viewport[3] - aPoint.y);
	winZ = wz;

	int i = gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return Coord3d(posX, posY, zValue);
}

Coord3d LKLayer::convertLayerToScreen(const Coord3d aPoint)
{
	Coord3d offset(0,0,0);
	convertFromVWorld(offset);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble wx, wy, wz;
	gluProject(aPoint.x, aPoint.y, aPoint.z, modelview, projection, viewport, &wx, &wy, &wz);

	return Coord3d(wx, wy, wz);
}

Coord3d LKLayer::projectLayerToZValue(const Coord3d aPoint, double zValue)
{
	Coord3d offset(0,0,0);
	//convertFromVWorld(offset);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	GLdouble wx, wy, wz;
	gluProject(aPoint.x, aPoint.y, aPoint.z + offset.z, modelview, projection, viewport, &wx, &wy, &wz);

	int i = gluUnProject(wx, wy, wz, modelview, projection, viewport, &posX, &posY, &posZ);

	return Coord3d(posX, posY, zValue);
}

bool LKLayer::mouseInRect(const Coord2d& p, const Coord4d& rect)
{
    return (p.x > rect.t && p.x < rect.v && p.y > rect.u && p.y < rect.w);
}

const Coord4d LKLayer::bounds(void) const
{
    return m_bounds * m_scale.x;
}

void LKLayer::setBounds(const Coord4d& bounds)
{
    m_bounds = bounds;
}

void LKLayer::setBounds(const double& t, const double& u, const double& v, const double& w)
{
    m_bounds.set(t, u, v, w);
}

bool LKLayer::autoComputeBounds(void)
{
	return m_autoComputeBounds;
}

void LKLayer::setAutoComputeBounds(bool v)
{
	m_autoComputeBounds = v;
}

const Coord3d& LKLayer::position(void) const
{
    return m_position;
}

void LKLayer::setPosition(const Coord3d& pos)
{
    m_position = pos;
	if (m_animator->m_positionAnimator->isRunning())
		m_animator->m_positionAnimator->stop();
}

void LKLayer::setPosition(const double& x, const double& y, const double& z)
{
    m_position.set(x, y, z);
    //m_animator->setPosition(m_position);
}

void LKLayer::setXPosition(const double x)
{
	m_position.set(x, m_position.y, m_position.z);
}

void LKLayer::setYPosition(const double y)
{
	m_position.set(m_position.x, y, m_position.z);
}

void LKLayer::setZPosition(const double z)
{
	m_position.set(m_position.x, m_position.y, z);
}

void LKLayer::setRelativePosition(const double& dx, const double& dy, const double& dz)
{
    m_position.x += dx;
    m_position.y += dy;
    m_position.z += dz;
}

const Coord3d& LKLayer::positionOffset(void) const
{
	return m_positionOffset;
}

void LKLayer::setPositionOffset(const Coord3d& pos)
{
	m_positionOffset = pos;
}

const Coord3d& LKLayer::rotation(void) const
{
    return m_rotation;
}

void LKLayer::setRotation(const Coord3d& pos)
{
    m_rotation = pos;
    //m_animator->setRotation(m_rotation);
}

void LKLayer::setRotation(const double& x, const double& y, const double& z)
{
    m_rotation.set(x, y, z);
    //m_animator->setRotation(m_rotation);
}

void LKLayer::setRelativeRotation(const double& dx, const double& dy, const double& dz)
{
    m_rotation.x += dx;
    m_rotation.y += dy;
    m_rotation.z += dz;
}

const Coord3d& LKLayer::scale(void) const
{
    return m_scale;
}
   
void LKLayer::setScaleS(const double s)
{
    m_scale.set(s, s, s);
}

void LKLayer::setScale(const Coord3d& scale)
{
    m_scale = scale;
}

void LKLayer::setScale(const double& sx, const double& sy, const double& sz)
{
    m_scale.set(sx, sy, sz);
}

const double LKLayer::opacity(void) const
{
	return m_opacity;
}

void LKLayer::setOpacity(double v) 
{
	m_opacity = v;
}

void LKLayer::display(void)
{
    display(0);
}

void LKLayer::display(long millisecondsPast, RenderStage renderStage)
{
	bool shouldRender = ((renderStage == DRAW) && (this->opacity() == 1.0)) ||
						((renderStage == DRAW_TRANSPARENT) && (this->opacity() != 1.0));

	if (shouldRender && millisecondsPast != 0 && m_animator)
		this->m_animator->update(millisecondsPast);
			
    glPushMatrix();
    glTranslated(m_position.x + m_positionOffset.x, 
				 m_position.y + m_positionOffset.y, 
				 m_position.z + m_positionOffset.z);
    glLoadName(tag());
    glScaled(m_scale.x, m_scale.y, m_scale.z);
    glRotated(m_rotation.z, 0, 0, 1.0);
    glPushMatrix();
	glRotated(m_rotation.y, 0, 1.0, 0);
    glPushMatrix();
    glRotated(m_rotation.x, 1.0, 0, 0);
	if (shouldRender && this->m_position.z <= 0)
		//if (renderStage == DRAW || renderStage == DRAW_TRANSPARENT)
			this->draw();
	else if (renderStage == POST_DRAW)
			this->postDraw();
    glPopMatrix();

	// draw sublayers
    foreach (LKLayer* l, m_layers){
        if (l->isHidden())
			continue;
        l->display(millisecondsPast, renderStage);
    }

    glPopMatrix();
    glPopMatrix();
	 
	// auto compute bounds if necessary
	if (m_autoComputeBounds){
		m_bounds.set(1000, 1000, -1000, -1000);
		foreach (LKLayer* l, m_layers){
			m_bounds.t = std::min(m_bounds.t, l->position().x + l->bounds().t);
			m_bounds.u = std::min(m_bounds.u, l->position().y + l->bounds().u);
			m_bounds.v = std::max(m_bounds.v, l->position().x + l->bounds().v);
			m_bounds.w = std::max(m_bounds.w, l->position().y + l->bounds().w);
		}
	}

	if (g_debugLayer){
		glLoadName(0);

		Coord4d bound = bounds();

		glDisable(GL_DEPTH_TEST);
		if (m_autoComputeBounds)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		glVertex3d(m_position.x + bound.t, m_position.y + bound.u, m_position.z); 
		glVertex3d(m_position.x + bound.v, m_position.y + bound.u, m_position.z);
				
		if (m_autoComputeBounds)
			glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3d(m_position.x + bound.v, m_position.y + bound.w, m_position.z);
		glVertex3d(m_position.x + bound.t, m_position.y + bound.w, m_position.z); 
		glEnd();
		glEnable(GL_DEPTH_TEST);
	}
}

bool LKLayer::isHidden(void) const
{
    return m_isHidden;
}

void LKLayer::setIsHidden(const bool b)
{
    m_isHidden = b;
}

LKLayer* LKLayer::hitTest(Coord2d& c)
{
	return NULL;
}

void LKLayer::convertFromVWorld(Coord2d& c)
{
	if (m_superlayer != NULL)
		m_superlayer->convertFromVWorld(c);
	c.x = c.x - m_position.x;
	c.y = c.y - m_position.y;	
}

void LKLayer::convertFromVWorld(Coord3d& c)
{
	if (m_superlayer != NULL)
		m_superlayer->convertFromVWorld(c);
	c.x = c.x + m_position.x;
	c.y = c.y + m_position.y;	
	c.z = c.z + m_position.z;
}

bool LKLayer::mouseDown(LKEvent* event)
{
    return LK_CONTINUE_EVENT;
}

bool LKLayer::mouseDragged(LKEvent* event)
{ 
    return LK_CONTINUE_EVENT;
}

bool LKLayer::mouseUp(LKEvent* event)
{ 
    return LK_CONTINUE_EVENT;
}

bool LKLayer::mouseMoved(LKEvent* event)
{
    return LK_CONTINUE_EVENT;
}

bool LKLayer::mouseEntered(LKEvent* event)
{ 
    return LK_CONTINUE_EVENT;
}

bool LKLayer::mouseExited(LKEvent* event)
{  
    return LK_CONTINUE_EVENT;
}

bool LKLayer::scrollWheel(LKEvent* event)
{
	return LK_CONTINUE_EVENT;
}

bool LKLayer::keydown(LKEvent* event)
{
	return LK_CONTINUE_EVENT;
}

LKAnimator* LKLayer::animator(void)
{
    return m_animator;
}

const bool LKLayer::debugLayer(void)
{
	return g_debugLayer;
}

void LKLayer::setDebugLayer(bool debug)
{
	g_debugLayer = debug;
}