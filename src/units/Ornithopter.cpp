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

#include <units/Ornithopter.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <Map.h>
#include <House.h>
#include <Game.h>
#include <SoundPlayer.h>

#define ORNITHOPTER_FRAMETIME 3

Ornithopter::Ornithopter(House* newOwner) : AirUnit(newOwner) {

    Ornithopter::init();

    setHealth(getMaxHealth());
}

Ornithopter::Ornithopter(InputStream& stream) : AirUnit(stream) {
    Ornithopter::init();
}

void Ornithopter::init() {
    itemID = Unit_Ornithopter;
    owner->incrementUnits(itemID);

    graphicID = ObjPic_Ornithopter;
    graphic = pGFXManager->getObjPic(graphicID,getOwner()->getHouseID());
    shadowGraphic = pGFXManager->getObjPic(ObjPic_OrnithopterShadow,getOwner()->getHouseID());

    numImagesX = NUM_ANGLES;
    numImagesY = 3;

    numWeapons = 1;
    bulletType = Bullet_SmallRocket;

    currentMaxSpeed = currentGame->objectData.data[itemID][originalHouseID].maxspeed;
}

Ornithopter::~Ornithopter() = default;

void Ornithopter::checkPos() {
    AirUnit::checkPos();

    if(!target) {
        if(destination.isValid()) {
            if(blockDistance(location, destination) <= 2) {
                destination.invalidate();
            }
        } else {
            if(blockDistance(location, guardPoint) > 17) {
                setDestination(guardPoint);
            }
        }
    }

    drawnFrame = ((currentGame->getGameCycleCount() + getObjectID())/ORNITHOPTER_FRAMETIME) % numImagesY;
}

bool Ornithopter::canAttack(const ObjectBase* object) const {
    if ((object != nullptr) && !object->isAFlyingUnit()
        && ((object->getOwner()->getTeam() != owner->getTeam()) || object->getItemID() == Unit_Sandworm)
        && object->isVisible(getOwner()->getTeam()))
        return true;
    else
        return false;
}

void Ornithopter::destroy() {
    // place wreck
    if(currentGameMap->tileExists(location)) {
        Tile* pTile = currentGameMap->getTile(location);
        pTile->assignDeadUnit(DeadUnit_Ornithopter, owner->getHouseID(), Coord(lround(realX), lround(realY)));
    }

    AirUnit::destroy();
}

void Ornithopter::playAttackSound() {
    soundPlayer->playSoundAt(Sound_Rocket,location);
}

bool Ornithopter::canPass(int xPos, int yPos) const {
    return (currentGameMap->tileExists(xPos, yPos) && (!currentGameMap->getTile(xPos, yPos)->hasAnAirUnit()));
}

