/*
 * SlabType.cpp
 *
 */

#include "adiktedpp/raw/SlabType.h"


namespace adiktedpp {
    namespace raw {

        const std::set< SlabType >& SlabRooms() {
            static std::set< SlabType > rooms;
            if ( rooms.empty() ) {
                rooms.insert( SlabType::ST_TREASURE );
                rooms.insert( SlabType::ST_LIBRARY );
                rooms.insert( SlabType::ST_PRISONCASE );
                rooms.insert( SlabType::ST_TORTURE );
                rooms.insert( SlabType::ST_TRAINING );
                rooms.insert( SlabType::ST_WORKSHOP );
                rooms.insert( SlabType::ST_SCAVENGER );
                rooms.insert( SlabType::ST_TEMPLE );
                rooms.insert( SlabType::ST_GRAVEYARD );
                rooms.insert( SlabType::ST_HATCHERY );
                rooms.insert( SlabType::ST_LAIR );
                rooms.insert( SlabType::ST_BARRACKS );
                rooms.insert( SlabType::ST_BRIDGE );
                rooms.insert( SlabType::ST_GUARDPOST );
            }
            return rooms;
        }

        const std::set< SlabType >& SlabDoors() {
            static std::set< SlabType > doors;
            if ( doors.empty() ) {
                doors.insert( SlabType::ST_DOORWOOD1 );
    //            doors.insert( SlabType::ST_DOORWOOD2 );
                doors.insert( SlabType::ST_DOORBRACE1 );
    //            doors.insert( SlabType::ST_DOORBRACE2 );
                doors.insert( SlabType::ST_DOORIRON1 );
    //            doors.insert( SlabType::ST_DOORIRON2 );
                doors.insert( SlabType::ST_DOORMAGIC1 );
    //            doors.insert( SlabType::ST_DOORMAGIC2 );
            }
            return doors;
        }

    }
}
