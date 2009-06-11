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
#ifndef LKEngine_h
#define LKEngine_h

#include "platform/gl.h"
#include <list>
#include <vector>
#include "math/Coord.h"
using std::vector;


/** define the maximum number of mouse devices in the system */
#define N_MOUSE_DEVICES 24

class  LKAnimator;
class  LKLayer;
struct LKEvent;
struct TextureImage;

class LKEngine {
public:
    typedef std::list<LKLayer*>   LKLayerList;
    typedef std::vector<LKLayer*> LKLayerVector;

    LKEngine(void);
    LKEngine(LKLayer* root);
    ~LKEngine(void);

	void initViewportTexture(void);

	/** returns the number of ticks passed since the LKEngine
	 *  was created */
	static int getTicks(void);
		
	/** returns the root layer of the engine */
    LKLayer* root(void) const;

    void   callDisplay(void);
	void   render(void);
	GLuint renderToTexture(void);

	const double FOV(void) const;
	void setFOV(double fov);

	void windowDidResize(int width, int height);

	LKLayerVector hitTest(Coord2d& p);
    Coord2d convertPointToLayer(Coord2d& aPoint, LKLayer* aView);
	//Coord3d convertPointtoOpenGLCoords(Coord2d& aPoint);
	bool mouseIsInGLLayerContents(LKLayer* layer);

	/** updates the mouse position trail with the new position */
	void updateMousePositionTrail(Coord2d c, int devId);

    void handleLKEvent(LKEvent* evt);

private:
    double      m_yfov; /** field of view (in radians) */
    Coord3d     m_cameraPos;
    LKLayer*    m_root;
	vector<int> m_glIntersectLayers;
    LKLayerList m_layersMouseIn[N_MOUSE_DEVICES];
	GLuint      m_viewportTexture;
	GLuint		m_frameBuffer;
	GLuint		m_depthBuffer;
	LKAnimator* m_animator;
};

#endif