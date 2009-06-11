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
#ifndef LKUtil_h
#define LKUtil_h

#include <algorithm>
//#include "mmlib/manymouse.h"
#include <vector>


struct LKEvent;


/** creates a vector of LKEvents by polling input from
 *  the ManyMouse library */
std::vector<LKEvent*> BuildLKEventsFromManyMouseEvents(void);


/** returns true if the iterable source of type S contains the
 *  element t of type T, else false  */
template <class S, class T>
bool contains(S& source, T& t)
{
	if (source.empty())
		return false;
	typename S::iterator found = std::find(source.begin(), source.end(), t);
    return (found != source.end());
}


#endif