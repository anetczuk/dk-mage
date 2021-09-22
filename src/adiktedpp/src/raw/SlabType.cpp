/*
 * SlabType.cpp
 *
 */

#include "adiktedpp/raw/SlabType.h"

#include "utils/Log.h"


namespace adiktedpp {
    namespace raw {

        std::string SlabTypeName( const SlabType data ) {
            switch( data ) {
            case SlabType::ST_ROCK:       { return "ST_ROCK"; }
            case SlabType::ST_GOLD:       { return "ST_GOLD"; }
            case SlabType::ST_EARTH:      { return "ST_EARTH"; }
            case SlabType::ST_TORCHDIRT:  { return "ST_TORCHDIRT"; }
            case SlabType::ST_WALLDRAPE:  { return "ST_WALLDRAPE"; }
            case SlabType::ST_WALLTORCH:  { return "ST_WALLTORCH"; }
            case SlabType::ST_WALLWTWINS: { return "ST_WALLWTWINS"; }
            case SlabType::ST_WALLWWOMAN: { return "ST_WALLWWOMAN"; }
            case SlabType::ST_WALLPAIRSHR:{ return "ST_WALLPAIRSHR"; }
            case SlabType::ST_PATH:       { return "ST_PATH"; }
            case SlabType::ST_CLAIMED:    { return "ST_CLAIMED"; }
            case SlabType::ST_LAVA:       { return "ST_LAVA"; }
            case SlabType::ST_WATER:      { return "ST_WATER"; }
            case SlabType::ST_PORTAL:     { return "ST_PORTAL"; }
            case SlabType::ST_TREASURE:   { return "ST_TREASURE"; }
            case SlabType::ST_LIBRARY:    { return "ST_LIBRARY"; }
            case SlabType::ST_PRISONCASE: { return "ST_PRISONCASE"; }
            case SlabType::ST_TORTURE:    { return "ST_TORTURE"; }
            case SlabType::ST_TRAINING:   { return "ST_TRAINING"; }
            case SlabType::ST_DUNGHEART:  { return "ST_DUNGHEART"; }
            case SlabType::ST_WORKSHOP:   { return "ST_WORKSHOP"; }
            case SlabType::ST_SCAVENGER:  { return "ST_SCAVENGER"; }
            case SlabType::ST_TEMPLE:     { return "ST_TEMPLE"; }
            case SlabType::ST_GRAVEYARD:  { return "ST_GRAVEYARD"; }
            case SlabType::ST_HATCHERY:   { return "ST_HATCHERY"; }
            case SlabType::ST_LAIR:       { return "ST_LAIR"; }
            case SlabType::ST_BARRACKS:   { return "ST_BARRACKS"; }
            case SlabType::ST_DOORWOOD1:  { return "ST_DOORWOOD1"; }
            case SlabType::ST_DOORWOOD2:  { return "ST_DOORWOOD2"; }
            case SlabType::ST_DOORBRACE1: { return "ST_DOORBRACE1"; }
            case SlabType::ST_DOORBRACE2: { return "ST_DOORBRACE2"; }
            case SlabType::ST_DOORIRON1:  { return "ST_DOORIRON1"; }
            case SlabType::ST_DOORIRON2:  { return "ST_DOORIRON2"; }
            case SlabType::ST_DOORMAGIC1: { return "ST_DOORMAGIC1"; }
            case SlabType::ST_DOORMAGIC2: { return "ST_DOORMAGIC2"; }
            case SlabType::ST_HOLYGROUND: { return "ST_HOLYGROUND"; }
            case SlabType::ST_BRIDGE:     { return "ST_BRIDGE"; }
            case SlabType::ST_GEMS:       { return "ST_GEMS"; }
            case SlabType::ST_GUARDPOST:  { return "ST_GUARDPOST"; }
            }
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument -- unknown value: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }

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
