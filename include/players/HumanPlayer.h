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

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <players/Player.h>

#include <Definitions.h>

#include <SDL.h>

// forward declarations
class UnitBase;
class StructureBase;
class ObjectBase;

class HumanPlayer : public Player
{
public:

    /// This enum has the same int values as ItemID_enum for structures (exceptions are the first and the last entry)
    enum class TutorialHint {
        HarvestSpice = 0,
        ThisIsABarrack = 1,
        ThisIsAConstructionYard = 2,
        ThisIsAGunTurret = 3,
        ThisIsAHeavyFactory = 4,
        ThisIsAHighTechFactory = 5,
        ThisIsHouseIX = 6,
        ThisIsALightFactory = 7,
        ThisIsAPalace = 8,
        ThisIsARadar = 9,
        ThisIsARefinery = 10,
        ThisIsARepairYard = 11,
        ThisIsARocketTurret = 12,
        ThisIsASilo = 13,
        ThisIsASlab1 = 14,
        ThisIsASlab4 = 15,
        ThisIsAStarPort = 16,
        ThisIsAWall = 17,
        ThisIsAWindTrap = 18,
        ThisIsAWOR = 19,
        NotEnoughConrete = 20,
    };

    void init();
    virtual ~HumanPlayer();
    virtual void save(OutputStream& stream) const;

    virtual void update();

    Uint32 getAlreadyShownTutorialHints() const {
        return alreadyShownTutorialHints;
    }

    /**
        The player just started to produce an item.
        \param  itemID  the item that is produced
    */
    virtual void onProduceItem(Uint32 itemID);

    /**
        The player just placed a structure.
        \param  pStructure  the structure that was placed (nullptr for Slab)
    */
    virtual void onPlaceStructure(const StructureBase* pStructure);

    /**
        A unit of this player was deployed.
        \param  pUnit  the unit that was deployed
    */
    virtual void onUnitDeployed(const UnitBase* pUnit);


    /**
        The set of selected units or structures has changed.
        \param  selectedObjectIDs   the new set of selected objects
    */
    virtual void onSelectionChanged(const std::set<Uint32>& selectedObjectIDs);

    /**
        Returns one of the 9 saved units lists
        \param  groupListIndex   which list should be returned
        \return the n-th list.
    */
    inline std::set<Uint32>& getGroupList(int groupListIndex) { return selectedLists[groupListIndex]; };

    /**
        Sets one of the 9 saved units lists
        \param  groupListIndex     which list should be set
        \param  newGroupList        the new list to set
    */
    void setGroupList(int groupListIndex, const std::set<Uint32>& newGroupList);

    static Player* create(House* associatedHouse, const std::string& playername) {
        return new HumanPlayer(associatedHouse, playername);
    }

    static Player* load(InputStream& stream, House* associatedHouse) {
        return new HumanPlayer(stream, associatedHouse);
    }

public:
    Uint32 nextExpectedCommandsCycle;                       ///< The next cycle we expect commands for (using for network games)

    std::set<Uint32> selectedLists[NUMSELECTEDLISTS];       ///< Sets of all the different groups on key 1 to 9

    Uint32 alreadyShownTutorialHints;                       ///< Contains flags for each tutorial hint (see enum TutorialHint)

private:
    HumanPlayer(House* associatedHouse, const std::string& playername);
    HumanPlayer(InputStream& stream, House* associatedHouse);

    void triggerStructureTutorialHint(Uint32 itemID);

    bool hasConcreteOfSize(const Coord& concreteSize) const;
    bool hasConcreteAtPositionOfSize(const Coord& pos, const Coord& concreteSize) const;
};



#endif // HUMANPLAYER_H
