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
#include "LKEngine.h"

#include <assert.h>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <math.h>
#include "LKAnimation.h"
#include "LKLayer.h"
#include "LKUtil.h"
#include "platform/gl.h"
#include <stack>
#include <time.h>
using namespace std;

#define foreach BOOST_FOREACH


LKEngine::LKEngine(void)
    : m_yfov(90)
    , m_cameraPos(0, 0, 0)
    , m_root(new LKLayer())
	, m_glIntersectLayers(0)
	, m_animator(new LKLinearAnimator(NULL))
{
	initViewportTexture();
}

LKEngine::LKEngine(LKLayer* root)
    : m_yfov(90)
    , m_cameraPos(0, 0, 0)
    , m_root(root)
	, m_glIntersectLayers(0)
{
	initViewportTexture();
}

LKEngine::~LKEngine(void)
{
}
#define CHECK_FRAMEBUFFER_STATUS() \
{ \
	GLenum status; \
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); \
	switch(status) { \
	case GL_FRAMEBUFFER_COMPLETE_EXT: \
	break; \
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT: \
	/* choose different formats */\
	break; \
	default: \
	/* programming error; will fail on all hardware */\
	assert(0); \
} \
}

void LKEngine::initViewportTexture(void)
{
	glGenTextures(1, &m_viewportTexture);
	GLuint* data = new GLuint[512 * 512 * 4];
	glBindTexture(GL_TEXTURE_2D, m_viewportTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	delete[] data;

	glGenFramebuffersEXT(1, &m_frameBuffer);
	glGenRenderbuffersEXT(1, &m_depthBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBuffer);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_viewportTexture, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 512, 512);

	CHECK_FRAMEBUFFER_STATUS();

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthBuffer);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
	
int LKEngine::getTicks(void)
{
	const static int mult = 1000 / CLOCKS_PER_SEC;
	return static_cast<int>(clock() * mult);
}

LKLayer* LKEngine::root(void) const
{
    return m_root;
}

const double LKEngine::FOV(void) const
{
	return m_yfov;
}

void LKEngine::setFOV(double fov)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLfloat ratio = (GLfloat)(viewport[2] / (GLfloat)viewport[3]);
	m_yfov = fov;
       
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(m_yfov, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void LKEngine::windowDidResize(int width, int height)
{
    if (height == 0)
    	height = 1;
    GLfloat ratio = (GLfloat)(width / (GLfloat)height);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_yfov, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	// create the lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat ambn0[4] = {0, 0.0, 0.0, 1};
	GLfloat lpos0[4] = {0, 0.75, 0.5, 0};
	GLfloat spec0[4] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, lpos0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, spec0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambn0);

	glEnable(GL_COLOR_MATERIAL);
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//float specReflection[] = {0.9, 0.9, 0.9, 1};
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specReflection);
	
	float ambientMat[] = {0, 0, 0, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambientMat);
	
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void LKEngine::callDisplay(void)
{
	render();
}

void LKEngine::render(void)
{
	static int lastTick = 0;
	int currTick = LKEngine::getTicks();
    int ticksPast = currTick - lastTick;
    lastTick = currTick;

	// render the layer tree
	glClearColor(0.75, 0.75, 0.75, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
	glPushMatrix();
	glTranslated(-m_cameraPos.x, -m_cameraPos.y, -m_cameraPos.z);
    
	// render the non transparent layers first, and then the transparent layers
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
	m_root->display(ticksPast);
	glDepthMask(false);
	m_root->display(ticksPast, LKLayer::DRAW_TRANSPARENT);
	m_root->display(ticksPast, LKLayer::POST_DRAW);
	glDepthMask(true);
	glPopAttrib();

	glPopMatrix();
}

GLuint LKEngine::renderToTexture(void)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBuffer);

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	
	gluPerspective(m_yfov, 1280.0f / 960.0f, 0.1f, 100.0f);

	glMatrixMode( GL_MODELVIEW );

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, 512, 512);
	render();
	glPopAttrib();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	
	glMatrixMode( GL_MODELVIEW );
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return m_viewportTexture;
}

vector<LKLayer*> LKEngine::hitTest(Coord2d& p)
{
    // find the GL intersections
    /*GLint  viewport[4];
    GLuint buffer[512];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(512, buffer);

    (void)glRenderMode(GL_SELECT);
    glDisable(GL_DEPTH_TEST);
    glClearDepth( 1.0f );
    glInitNames();
    glPushName(0);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((GLdouble)p.x, (GLdouble)(viewport[3] - p.y), 1.0f, 1.0f, viewport);
		gluPerspective(m_yfov, (viewport[2] - viewport[0]) / (GLfloat)(viewport[3] - viewport[1]), 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		m_root->display();
		glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    
    GLint nHits = glRenderMode(GL_RENDER);

    // loop through detected hits and find the closest target
    m_glIntersectLayers.clear();


    if (nHits > 0){
        int hitTag = -1;
        int depth  = -1;
        for (int i = 0; i < nHits; i++){
            hitTag = buffer[i * 4 + 3];
            depth  = buffer[i * 4 + 1];
            // handle the callbacks
            //m_glIntersectLayers.push_back(hitTag);
			//result.push_back(m_root->sublayers()[hitTag]);
        }
    }
*/	std::vector<LKLayer*> result(0);
    // find the 2D layer intersections
	std::stack<LKLayer*> layerStack;
	result.push_back(m_root);
	layerStack.push(m_root);

	// to do this, recursively iterate through all the layers
	// converting the point on the screen to a point on the
	// same plane as the layer.
	while (!layerStack.empty()){
		LKLayer* layer = layerStack.top();
		layerStack.pop();
		Coord2d c = convertPointToLayer(p, layer);
		if (layer->mouseInRect(c, layer->bounds()))
            result.push_back(layer);
		foreach (LKLayer* subLayer, layer->sublayers())
			layerStack.push(subLayer);
	}
    return result;
}

Coord2d LKEngine::convertPointToLayer(Coord2d& aPoint, LKLayer* aView)
{/*
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

	Coord3d offset(0,0,0);
	aView->convertFromVWorld(offset);

    double hw =  viewport[2] / 2.0;
    double hh =  viewport[3] / 2.0;
    double x  =  (aPoint.x - hw) / hw;
    double y  = -(aPoint.y - hh) / hh;
    Coord2d c(x * tan(m_yfov / 2.0) * (m_cameraPos.z - offset.z) - offset.x,
              y * tan(m_yfov / 2.0) * (m_cameraPos.z - offset.z) * (viewport[3] / (double)viewport[2]) - offset.y);

	return c;*/
	Coord3d offset(0,0,0);
	aView->convertFromVWorld(offset);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	GLdouble wx, wy, wz;
	gluProject(0, 0, offset.z - m_cameraPos.z, modelview, projection, viewport, &wx, &wy, &wz);

	winX = (float)(aPoint.x);
	winY = (float)viewport[3] - (float)aPoint.y;
	winZ = wz;
		
	int i = gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return Coord2d(posX - offset.x, posY - offset.y);
}

bool LKEngine::mouseIsInGLLayerContents(LKLayer* layer)
{
    foreach (int& i, m_glIntersectLayers)
        if (i == layer->tag())
            return true;
    return false;
}

void LKEngine::handleLKEvent(LKEvent* srcEvent)
{
    typedef list<LKLayer*>::iterator LKLayerListItr;

    LKEvent evt = *srcEvent;

	if ((srcEvent->type & DEV_KEY) != 0){
		std::stack<LKLayer*> layerStack;
		layerStack.push(m_root);
		while (!layerStack.empty()){
			LKLayer* layer = layerStack.top();
			layerStack.pop();
			layer->keydown(srcEvent);
			foreach (LKLayer* subLayer, layer->sublayers())
				layerStack.push(subLayer);
		}
	}

	if ((srcEvent->type & DEV_SCROLL_Y) != 0){
		foreach (LKLayer* layer, hitTest(srcEvent->screenLocation)){
            evt.devLocation = convertPointToLayer(srcEvent->screenLocation, layer);     
			layer->scrollWheel(&evt);
		}
	}
	// button down
    else if ((srcEvent->type & DEV_BUTTON_DOWN) != 0){
        foreach (LKLayer* layer, hitTest(srcEvent->screenLocation)){ 
            evt.devLocation = convertPointToLayer(srcEvent->screenLocation, layer);     
            layer->mouseDown(&evt);
        } 
    // button up
    } else if ((srcEvent->type & DEV_BUTTON_UP) != 0){
        foreach (LKLayer* layer, hitTest(srcEvent->screenLocation)){
            evt.devLocation = convertPointToLayer(srcEvent->screenLocation, layer); 
            if (!layer->mouseUp(&evt))
				break;
        }
    // dev motion and dev dragged events
    } else if ((srcEvent->type & DEV_MOTION) != 0 || (srcEvent->type & DEV_BUTTON_DRAGGED) != 0){
		
		LKLayerList* layersMouseIn  = &m_layersMouseIn[srcEvent->deviceID];
        vector<LKLayer*> hitTargets = hitTest(srcEvent->screenLocation);   

		// handle mouse entered, moved and dragged events
        foreach (LKLayer* layer, vector<LKLayer*>(hitTargets)){    
			evt = *srcEvent;                
			evt.devLocation = convertPointToLayer(srcEvent->screenLocation, layer); 

			// test if the mouse has not already been observed within
			// this layer. If not, call the mouseEntered event
			if (!contains(*layersMouseIn, layer)){
				layersMouseIn->push_back(layer);
				if (!layer->mouseEntered(&evt))
					break;
            } else {
                if ((srcEvent->type & DEV_MOTION) != 0)
                   layer->mouseMoved(&evt);
				if ((srcEvent->type & DEV_BUTTON_DRAGGED) != 0)
					layer->mouseDragged(&evt);
            }
        }// end foreach hitTarget

        // handle mouse exited events. find all the layers the mouse 
		// was previously observed in but is no longer in and call 
		// the mouse exited handler				
		if (!layersMouseIn->empty()){
			LKLayerList adjustedList;
			bool isModified = false;
			foreach (LKLayer* layer, *layersMouseIn){
				if (!contains(hitTargets, layer)){
					evt.devLocation = convertPointToLayer(srcEvent->screenLocation, layer);
					layer->mouseExited(&evt);
					isModified = true;
				} else {
					// else the mouse is still in the layer and must be remembered 
					adjustedList.push_back(layer);
				}
			}
			if (isModified)
				*layersMouseIn = adjustedList;
		}
	}// end if
}