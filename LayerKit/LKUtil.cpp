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
#include "LKUtil.h"

#include "math/Coord.h"
#include <iostream>
#include "LKLayer.h"
#include "platform/gl.h"
#include <string.h>
#include <stdio.h>
using std::cout;
using std::endl;


/** assume a theoretical limit of 24 mouse devices */
#define LKUTIL_MAX_N_MICE 24

/** create a static array to store the LKEvents corresponding to MM events */
static LKEvent mmLKEvents[LKUTIL_MAX_N_MICE]; 

std::vector<LKEvent*> BuildLKEventsFromManyMouseEvents(void)
{
	std::vector<LKEvent*> out;
	return out;
}


/*std::vector<LKEvent*> BuildLKEventsFromManyMouseEvents(void)
{
	bool isDeviceActive[LKUTIL_MAX_N_MICE];
	memset(isDeviceActive, 0, sizeof(isDeviceActive));
	ButtonID button;

	// reset certain state information in the LKEvents
	for (int i = 0; i < LKUTIL_MAX_N_MICE; i++){
		mmLKEvents[i].deviceID = i;
		mmLKEvents[i].type     = (EventType)0;
	}

	// process all the many mouse events, building up the list of
	// active LKEvents
	ManyMouseEvent mme;
	while (ManyMouse_PollEvent(&mme)){

		isDeviceActive[mme.device] = true;
		LKEvent& lkevt = mmLKEvents[mme.device];

		switch (mme.type){
		case MANYMOUSE_EVENT_RELMOTION:
		case MANYMOUSE_EVENT_ABSMOTION:
		
			// handle both relative and absolute mouse movements
			// by converting both into relative movements
			double x, y;
			if (mme.type == MANYMOUSE_EVENT_RELMOTION){
				x = mme.value;
				y = mme.value;
			} else {
				x = ((mme.value - 0) / 64831.0) * 1024  - lkevt.screenLocation.x;
				y = ((mme.value - 650) / 65207.0) * 600 - lkevt.screenLocation.y;
			}
			
			// handle button drag states
			if (lkevt.buttonID == NONE_BUTTON)
				lkevt.type = DEV_MOTION;
			else if (lkevt.buttonID == LEFT_BUTTON)
				lkevt.type = DEV_LEFT_BUTTON_DRAGGED;
			else if (lkevt.buttonID == LEFT_BUTTON)
				lkevt.type = DEV_RIGHT_BUTTON_DRAGGED;
			
			// update mouse position
			if (mme.item == 0){
				lkevt.screenLocation.x += x;
				lkevt.relativeMotion.x  = x;
			} else {
				lkevt.screenLocation.y += y;
				lkevt.relativeMotion.y  = y;
			}
			break;

		case MANYMOUSE_EVENT_BUTTON:
			
			button = mme.item == 0 ? LEFT_BUTTON : RIGHT_BUTTON;
			
			if (mme.value){

				// this is a mouse down event if we have the last observed
				// mouse button is not the same as the current observation
				if (lkevt.buttonID != button){
					if (mme.item == 0)
						lkevt.type = DEV_LEFT_BUTTON_DOWN;
					else if (mme.item == 1)
						lkevt.type = DEV_RIGHT_BUTTON_DOWN;
					lkevt.buttonID = button;
				}
				// else this is a mouse dragged event. We need to change
				// the event type accordingly
				else {
					if (button == LEFT_BUTTON)
						lkevt.type = DEV_LEFT_BUTTON_DRAGGED;
					else if (button == RIGHT_BUTTON)
						lkevt.type = DEV_RIGHT_BUTTON_DRAGGED;
				}

			} else {
				// handle mouse up events
				lkevt.buttonID = NONE_BUTTON;
				if (mme.item == 0)
					lkevt.type = DEV_LEFT_BUTTON_UP;
				else if (mme.item == 1)
					lkevt.type = DEV_RIGHT_BUTTON_UP;
			}

			break;
		case MANYMOUSE_EVENT_SCROLL:
			const char *wheel;
            const char *direction;
            if (mme.item == 0){
                wheel = "vertical";
                direction = ((mme.value > 0) ? "up" : "down");
            } else {
                wheel = "horizontal";
                direction = ((mme.value > 0) ? "right" : "left");
            }
            printf("Mouse #%u wheel %s %s\n", mme.device,
                    wheel, direction);
			break;
		case MANYMOUSE_EVENT_DISCONNECT:
            printf("Mouse #%u disconnect\n", mme.device);
		default:
			printf("Mouse #%u unhandled event type %d\n", mme.device,
                    mme.type);
		}

	}

	// output all the active mice
	std::vector<LKEvent*> out;
	for (int i = 0; i < LKUTIL_MAX_N_MICE; i++)
		if (isDeviceActive[i])
			out.push_back(&mmLKEvents[i]);

	return out;
}*/