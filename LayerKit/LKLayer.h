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
#ifndef LKLayer_h
#define LKLayer_h

#include "math/Coord.h"
#include "LKKey.h"
#include <vector>
using std::vector;


class LKAnimator;

typedef enum EventType {
    DEV_BUTTON_DOWN          = 1,
    DEV_BUTTON_UP            = 2,
    DEV_LEFT_BUTTON_DOWN     = 4   | DEV_BUTTON_DOWN,
    DEV_LEFT_BUTTON_UP       = 8   | DEV_BUTTON_UP,
    DEV_RIGHT_BUTTON_DOWN    = 16  | DEV_BUTTON_DOWN,
    DEV_RIGHT_BUTTON_UP      = 32  | DEV_BUTTON_UP,
    DEV_BUTTON_DRAGGED       = 64,
    DEV_LEFT_BUTTON_DRAGGED  = 128 | DEV_BUTTON_DRAGGED,
    DEV_RIGHT_BUTTON_DRAGGED = 256 | DEV_BUTTON_DRAGGED,
    DEV_ENTERED              = 512,
    DEV_EXITED               = 1024,
    DEV_MOTION               = 2048,
	DEV_SCROLL_Y             = 4096,
	DEV_KEY                  = 8192
};

typedef enum {
	NONE_BUTTON,
    LEFT_BUTTON,
    RIGHT_BUTTON,
    MIDDLE_BUTTON
} ButtonID;


/** LKEvent represents an event caused by the system or the user that
 *  is passed to the event handling routines of LKLayer */
struct LKEvent {
    int       deviceID; /* the device type unique identifier for this event. Currently
						 * only applies to mouse-like devices */
    EventType type; 
    Coord2d   screenLocation; /** the location of the device in screen coordinates */
	Coord2d   devLocation; /** the location of the device in layer coordinates*/
	Coord2d   relativeMotion; /** the relative motion from the previous position */
    ButtonID  buttonID;
	int       scrollDelta;
	LKKey     key;
};


/** a definition that events should return to the caller if they want
 *  the event to be propergated down the event handler tree */
#define LK_CONTINUE_EVENT true


/** the main layer class of LayerKit. The layer class is responsible for drawing
 *  to the screen and handle user and system interactions (such as mouse clicks).
 *
 *  To implement features of LKLayer, its virtual member functions should be 
 *  overiden. For example, to draw a layer to screen, subclass LKLayer and overide
 *  the draw method. Similarly, to respond to mouse clicks, mouseDown() should
 *  be overidden.
 *
 *  To handle mouse clicks correctly, implementations of LKLayer should set the
 *  bounds of the layer (either in the constructor or via the setBounds() method).
 */
class LKLayer {
public:
    LKLayer(void);
	LKLayer(Coord3d position);
	LKLayer(Coord4d bounds);
	LKLayer(Coord3d position, Coord4d bounds);
	virtual ~LKLayer(void);

    int tag(void) const;
    LKLayer* layerWithTag(const int tag);

    LKLayer* superlayer(void) const;
    vector<LKLayer*> sublayers(void) const;    
    void addSublayer(LKLayer* layer);
    void removeFromSuperlayer(void);

    /** converts a point in local layer coordinates to a point
     * in the coordinate system of a layer */
	Coord2d convertPointToLayer(const Coord2d& p, LKLayer& alayer);
    
	/** converts a point in screen coordinates to a point in the
	 *  coordinate system of this layer */
	Coord2d convertPointToLayer(const Coord2d& p);

	/** converts a point from screen coordinates to a point in this
	 *  layer with at a specified z depth */
	Coord3d convertScreenToLayerWithZValue(const Coord2d aPoint, double zValue);

	Coord3d convertLayerToScreen(const Coord3d aPoint);

	Coord3d projectLayerToZValue(const Coord3d aPoint, double zValue);


    bool mouseInRect(const Coord2d& p, const Coord4d& rect);
    
    const Coord4d bounds(void) const;
    void setBounds(const Coord4d& bounds);
    void setBounds(const double& t, const double& u, const double& v, const double& w);
	bool autoComputeBounds(void);
	void setAutoComputeBounds(bool v);

    const Coord3d& position(void) const;
    void setPosition(const Coord3d& pos);
    void setPosition(const double& x, const double& y, const double& z);
    void setXPosition(const double x);
    void setYPosition(const double y);
    void setZPosition(const double z);
	void setRelativePosition(const double& dx, const double& dy, const double& dz);

	const Coord3d& positionOffset(void) const;
	void setPositionOffset(const Coord3d& pos);

    const Coord3d& rotation(void) const;
    /** sets the rotation to the specified values. This will reset the
     *  animator and stop any current animations. */
    void setRotation(const Coord3d& pos);
    void setRotation(const double& x, const double& y, const double& z);
    /** sets the rotation to the current rotation adjusted by the
     *  specified delta values. This will not reset the animator for
	 *  coordinate values of 0. */
    void setRelativeRotation(const double& dx, const double& dy, const double& dz);

    const Coord3d& scale(void) const;
    void setScaleS(const double s);
    void setScale(const Coord3d& scale);
    void setScale(const double& sx, const double& sy, const double& sz);

	const double opacity(void) const;
	void setOpacity(double v);

    /** calls the draw method of this and any sub layers */
	enum RenderStage {PRE_DRAW, DRAW, DRAW_TRANSPARENT, POST_DRAW};
    void display(long millisecondsPast, RenderStage renderStage=DRAW);
	void display(void);

    bool isHidden(void) const;
    void setIsHidden(const bool b);

    LKLayer* hitTest(Coord2d& c);

	/** converts a from VWorld coordinates to local layer coordinates */
	void convertFromVWorld(Coord2d& c);
	void convertFromVWorld(Coord3d& c);

    /** renders the layer. Assumes that OpenGL is already in the correct
     *  transformation matrix for this layer */
	virtual void draw(void);
	/** called after all LKlayers have had their draw method called. This
	 *  allows effects that must occur after drawing (such as transparency)
	 *  to be performed */
    virtual void postDraw(void);

    /** mouse has been clicked in the display. Users should override this
     * method to handle mouse click events. Return true for events to be
     * passed to the next intersecting sublayer */
    virtual bool mouseDown(LKEvent* event);
    virtual bool mouseDragged(LKEvent* event);
    virtual bool mouseUp(LKEvent* event);
    virtual bool mouseMoved(LKEvent* event);
    virtual bool mouseEntered(LKEvent* event);
    virtual bool mouseExited(LKEvent* event);
	virtual bool scrollWheel(LKEvent* event);
	virtual bool keydown(LKEvent* event);

    LKAnimator* animator(void);
    friend class LKAnimator;
	
	static const bool debugLayer(void);
	static void setDebugLayer(bool debug);

private:
    int      m_tag; /** the unique identifier for this layer */
    bool     m_isHidden; /** is the layer hidden from view */
    Coord4d  m_bounds;
	bool     m_autoComputeBounds;
    Coord3d  m_position;
	Coord3d  m_positionOffset;
    Coord3d  m_rotation;
    Coord3d  m_scale;
	double   m_opacity;
    LKLayer* m_superlayer;
	vector<LKLayer*> m_layers;
	LKAnimator* m_animator;

};



#endif