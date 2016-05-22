/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MMATH_H
#define MMATH_H

// forward declaration
class Coord;

#include <DataTypes.h>
#include <fixmath/FixPoint.h>
#include <cmath>

int getRandomInt(int min, int max);
int getRandomOf(int numParam, ...);

FixPoint destinationAngleRad(const Coord& p1, const Coord& p2);

inline FixPoint RadToDeg256(FixPoint angleRad) { return (angleRad << 7)/FixPt_PI; } // angleRad*256/(2*FixPt_PI)

inline FixPoint Deg256ToRad(FixPoint angle) { return (angle*FixPt_PI) >> 7; }   // angle*2*FixPt_PI/256;

/**
    Calculates the smaller angle between angle1 and angle2. The result is always positive,
    e.g. angleDiff(RIGHTUP, DOWN) = angleDiff(DOWN, RIGHTUP) = 5
    \param  angle1  the first angle
    \param  angle2  the second angle
    \return the angle between angle1 and angle2.
*/
inline int angleDiff(int angle1, int angle2) {
    int diff = abs(angle1 - angle2);
    return std::min(diff, NUM_ANGLES - diff);
}

inline int angleToDrawnAngle(FixPoint angle) { return lround(angle >> 5) & 0x7; }   //  lround(angle*NUM_ANGLES/256) % NUM_ANGLES;

inline int destinationDrawnAngle(const Coord& p1, const Coord& p2) {
    return angleToDrawnAngle(RadToDeg256(destinationAngleRad(p1, p2)));
}

FixPoint distanceFrom(const Coord& p1, const Coord& p2);
FixPoint distanceFrom(FixPoint x, FixPoint y, FixPoint to_x, FixPoint to_y);

FixPoint blockDistance(const Coord& p1, const Coord& p2);

// Retreat location for launcher
Coord retreatLocation(const Coord& p1, const Coord& p2);

int mirrorAngleHorizontal(int angle);
int mirrorAngleVertical(int angle);

/**
    This method converts between world coordinates and zoomed world coordinates.
    \param  x   a coordinate in world coordinates
    \return a coordinate which is converted by the current zoom level
*/
int world2zoomedWorld(int x);

/**
    This method converts between world coordinates and zoomed world coordinates.
    \param  x   a coordinate in world coordinates
    \return a coordinate which is converted by the current zoom level
*/
int world2zoomedWorld(float x);

/**
    This method converts between world coordinates and zoomed world coordinates.
    \param  coord   a coordinate in world coordinates
    \return a coordinate which is converted by the current zoom level
*/
Coord world2zoomedWorld(const Coord& coord);

/**
    This method converts between world coordinates and zoomed world coordinates.
    \param  x   a coordinate in world coordinates
    \return a coordinate which is converted by the current zoom level
*/
int zoomedWorld2world(int x);

/**
    This method converts between world coordinates and zoomed world coordinates.
    \param  coord   a coordinate in world coordinates
    \return a coordinate which is converted by the current zoom level
*/
Coord zoomedWorld2world(const Coord& coord);


#endif //MMATH_H
