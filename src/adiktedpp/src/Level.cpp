/*
 *
 */

#include "adiktedpp/Level.h"

#include "adiktedpp/LakeGenerator.h"

#include "utils/Log.h"


extern "C" {
    #include "libadikted/adikted.h"
}


using namespace utils;


namespace adiktedpp {

    static raw::SlabType convertToSlab( const Slab slab ) {
        switch( slab ) {
        case Slab::S_ROCK:   return raw::SlabType::ST_ROCK;
        case Slab::S_GOLD:   return raw::SlabType::ST_GOLD;
        case Slab::S_GEMS:   return raw::SlabType::ST_GEMS;
        case Slab::S_EARTH:  return raw::SlabType::ST_EARTH;
        case Slab::S_PATH:   return raw::SlabType::ST_PATH;
        case Slab::S_LAVA:   return raw::SlabType::ST_LAVA;
        case Slab::S_WATER:  return raw::SlabType::ST_WATER;
        }
        LOG() << "invalid slab: " << (int)slab;
        std::stringstream stream;
        stream << FILE_NAME << ": invalid slab: " << (int)slab;
        throw std::invalid_argument( stream.str() );
    }

    static Slab convertToSlab( const raw::SlabType slab ) {
        switch( slab ) {
        case raw::SlabType::ST_ROCK:  return Slab::S_ROCK;
        case raw::SlabType::ST_GOLD:  return Slab::S_GOLD;
        case raw::SlabType::ST_EARTH: return Slab::S_EARTH;
        case raw::SlabType::ST_PATH:  return Slab::S_PATH;
        case raw::SlabType::ST_LAVA:  return Slab::S_LAVA;
        case raw::SlabType::ST_WATER: return Slab::S_WATER;
        case raw::SlabType::ST_GEMS:  return Slab::S_GEMS;

        case raw::SlabType::ST_PORTAL:      return Slab::S_PATH;
        case raw::SlabType::ST_TREASURE:    return Slab::S_PATH;
        case raw::SlabType::ST_LIBRARY:     return Slab::S_PATH;
        case raw::SlabType::ST_PRISONCASE:  return Slab::S_PATH;
        case raw::SlabType::ST_TORTURE:     return Slab::S_PATH;
        case raw::SlabType::ST_TRAINING:    return Slab::S_PATH;
        case raw::SlabType::ST_DUNGHEART:   return Slab::S_PATH;
        case raw::SlabType::ST_WORKSHOP:    return Slab::S_PATH;
        case raw::SlabType::ST_SCAVENGER:   return Slab::S_PATH;
        case raw::SlabType::ST_TEMPLE:      return Slab::S_PATH;
        case raw::SlabType::ST_GRAVEYARD:   return Slab::S_PATH;
        case raw::SlabType::ST_HATCHERY:    return Slab::S_PATH;
        case raw::SlabType::ST_LAIR:        return Slab::S_PATH;
        case raw::SlabType::ST_BARRACKS:    return Slab::S_PATH;
        case raw::SlabType::ST_BRIDGE:      return Slab::S_PATH;
        case raw::SlabType::ST_GUARDPOST:   return Slab::S_PATH;

        default: {
            LOG() << "invalid slab type: " << slab;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid slab type: " << slab;
            throw std::invalid_argument( stream.str() );
        }
        }
    }

    /// ===========================================================

    static raw::SubTypeItem convertToRaw( const Item item ) {
        switch( item ) {
        case Item::I_HEROGATE:      return raw::SubTypeItem::STI_HEROGATE;

        case Item::I_GOLDL:         return raw::SubTypeItem::STI_GOLDL;
        case Item::I_GOLD:          return raw::SubTypeItem::STI_GOLD;
        case Item::I_GOLDCHEST:     return raw::SubTypeItem::STI_GOLDCHEST;
        case Item::I_GOLD_HOARD1:   return raw::SubTypeItem::STI_GLDHOARD1;
        case Item::I_GOLD_HOARD2:   return raw::SubTypeItem::STI_GLDHOARD2;
        case Item::I_GOLD_HOARD3:   return raw::SubTypeItem::STI_GLDHOARD3;
        case Item::I_GOLD_HOARD4:   return raw::SubTypeItem::STI_GLDHOARD4;
        case Item::I_GOLD_HOARD5:   return raw::SubTypeItem::STI_GLDHOARD5;

        case Item::I_SPECIAL_REVMAP:    return raw::SubTypeItem::STI_SPREVMAP;
        case Item::I_SPECIAL_RESURCT:   return raw::SubTypeItem::STI_SPRESURCT;
        case Item::I_SPECIAL_TRANSFR:   return raw::SubTypeItem::STI_SPTRANSFR;
        case Item::I_SPECIAL_STEALHR:   return raw::SubTypeItem::STI_SPSTEALHR;
        case Item::I_SPECIAL_MULTPLY:   return raw::SubTypeItem::STI_SPMULTPLY;
        case Item::I_SPECIAL_INCLEV:    return raw::SubTypeItem::STI_SPINCLEV;
        case Item::I_SPECIAL_MKSAFE:    return raw::SubTypeItem::STI_SPMKSAFE;
        case Item::I_SPECIAL_HIDNWRL:   return raw::SubTypeItem::STI_SPHIDNWRL;

        default: {
            LOG() << "invalid item: " << (int)item;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid item: " << (int)item;
            throw std::invalid_argument( stream.str() );
        }
        }
    }

    /// ===========================================================

    static raw::SlabType convertToSlab( const Room room ) {
        switch( room ) {
        case Room::R_CLAIMED:       return raw::SlabType::ST_CLAIMED;
        case Room::R_DUNGEON_HEART: return raw::SlabType::ST_DUNGHEART;
        case Room::R_PORTAL:        return raw::SlabType::ST_PORTAL;
        case Room::R_TREASURE:      return raw::SlabType::ST_TREASURE;
        case Room::R_LAIR:          return raw::SlabType::ST_LAIR;
        case Room::R_HATCHERY:      return raw::SlabType::ST_HATCHERY;
        case Room::R_TRAINING:      return raw::SlabType::ST_TRAINING;
        case Room::R_LIBRARY:       return raw::SlabType::ST_LIBRARY;
        case Room::R_BRIDGE:        return raw::SlabType::ST_BRIDGE;
        case Room::R_WORKSHOP:      return raw::SlabType::ST_WORKSHOP;
        case Room::R_GUARD_POST:    return raw::SlabType::ST_GUARDPOST;
        case Room::R_PRISON:        return raw::SlabType::ST_PRISONCASE;
        case Room::R_TORTURE:       return raw::SlabType::ST_TORTURE;
        case Room::R_BARRACKS:      return raw::SlabType::ST_BARRACKS;
        case Room::R_TEMPLE:        return raw::SlabType::ST_TEMPLE;
        case Room::R_GRAVEYARD:     return raw::SlabType::ST_GRAVEYARD;
        case Room::R_SCAVENGER:     return raw::SlabType::ST_SCAVENGER;
        }
        LOG() << "invalid room: " << room;
        std::stringstream stream;
        stream << FILE_NAME << ": invalid room: " << room;
        throw std::invalid_argument( stream.str() );
    }

    static raw::SubTypeTrap convertToRaw( const Trap trap ) {
        switch( trap ) {
        case Trap::T_BOULDER:        return raw::SubTypeTrap::STT_BOULDER;
        case Trap::T_ALARM:          return raw::SubTypeTrap::STT_ALARM;
        case Trap::T_POISON_GAS:     return raw::SubTypeTrap::STT_GAS;
        case Trap::T_LIGHTNING:      return raw::SubTypeTrap::STT_LIGHTNG;
        case Trap::T_WORD_OF_POWER:  return raw::SubTypeTrap::STT_WORDPWR;
        case Trap::T_LAVA:           return raw::SubTypeTrap::STT_LAVA;
        }
        LOG() << "invalid trap: " << (int)trap;
        std::stringstream stream;
        stream << FILE_NAME << ": invalid trap: " << (int)trap;
        throw std::invalid_argument( stream.str() );
    }

    static raw::SubTypeDoor convertToRaw( const Door door ) {
        switch( door ) {
        case Door::D_WOOD:   return raw::SubTypeDoor::STD_WOOD;
        case Door::D_BRACED: return raw::SubTypeDoor::STD_BRACED;
        case Door::D_IRON:   return raw::SubTypeDoor::STD_IRON;
        case Door::D_MAGIC:  return raw::SubTypeDoor::STD_MAGIC;
        }
        LOG() << "invalid door: " << (int)door;
        std::stringstream stream;
        stream << FILE_NAME << ": invalid door: " << (int)door;
        throw std::invalid_argument( stream.str() );
    }

    static raw::SubTypeCreature convertToRaw( const Creature creature ) {
        switch( creature ) {
        case Creature::C_TUNNELLER:   return raw::SubTypeCreature::STC_TUNELER;
        case Creature::C_FAIRY:       return raw::SubTypeCreature::STC_FAIRY;
        case Creature::C_ARCHER:      return raw::SubTypeCreature::STC_ARCHER;
        case Creature::C_PRIESTESS:   return raw::SubTypeCreature::STC_WITCH;
        case Creature::C_WIZARD:      return raw::SubTypeCreature::STC_WIZRD;
        case Creature::C_THIEF:       return raw::SubTypeCreature::STC_THEFT;
        case Creature::C_MONK:        return raw::SubTypeCreature::STC_MONK;
        case Creature::C_DWARF:       return raw::SubTypeCreature::STC_DWAFT;
        case Creature::C_BARBARIAN:   return raw::SubTypeCreature::STC_BARBARIN;
        case Creature::C_SAMURAI:     return raw::SubTypeCreature::STC_SMURI;
        case Creature::C_GIANT:       return raw::SubTypeCreature::STC_GIANT;
        case Creature::C_KNIGHT:      return raw::SubTypeCreature::STC_KNIGHT;
        case Creature::C_AVATAR:      return raw::SubTypeCreature::STC_AVATAR;

        case Creature::C_IMP:            return raw::SubTypeCreature::STC_IMP;
        case Creature::C_FLY:            return raw::SubTypeCreature::STC_FLY;
        case Creature::C_GHOST:          return raw::SubTypeCreature::STC_GHOST;
        case Creature::C_WARLOCK:        return raw::SubTypeCreature::STC_WARLOCK;
        case Creature::C_BEETLE:         return raw::SubTypeCreature::STC_BEETLE;
        case Creature::C_SPIDER:         return raw::SubTypeCreature::STC_SPIDER;
        case Creature::C_TROLL:          return raw::SubTypeCreature::STC_TROLL;
        case Creature::C_DEMONSPAWN:     return raw::SubTypeCreature::STC_SPAWN;
        case Creature::C_TENTACLE:       return raw::SubTypeCreature::STC_TENTCL;
        case Creature::C_SKELETON:       return raw::SubTypeCreature::STC_SKELETON;
        case Creature::C_HELL_HOUND:     return raw::SubTypeCreature::STC_HOUND;
        case Creature::C_MISTRESS:       return raw::SubTypeCreature::STC_MISTRESS;
        case Creature::C_ORC:            return raw::SubTypeCreature::STC_ORC;
        case Creature::C_BILE_DEMON:     return raw::SubTypeCreature::STC_BILEDEMN;
        case Creature::C_DRAGON:         return raw::SubTypeCreature::STC_DRAGON;
        case Creature::C_VAMPIRE:        return raw::SubTypeCreature::STC_VAMPIRE;
        case Creature::C_HORNY:          return raw::SubTypeCreature::STC_HORNY;
        }

        LOG() << "invalid creature: " << (int)creature;
        std::stringstream stream;
        stream << FILE_NAME << ": invalid creature: " << (int)creature;
        throw std::invalid_argument( stream.str() );
    }

    /// =======================================================================

    static raw::PlayerType convertToRaw( const Player player ) {
        switch( player ) {
        case Player::P_P0:    return raw::PlayerType::PT_0;
        case Player::P_P1:    return raw::PlayerType::PT_1;
        case Player::P_P2:    return raw::PlayerType::PT_2;
        case Player::P_P3:    return raw::PlayerType::PT_3;
        case Player::P_GOOD:  return raw::PlayerType::PT_GOOD;
        case Player::P_UNSET: return raw::PlayerType::PT_UNSET;
        case Player::P_ALL:   return raw::PlayerType::PT_ALL;
        }
        LOG() << "invalid player: " << (int)player;
        std::stringstream stream;
        stream << FILE_NAME << ": invalid player: " << (int)player;
        throw std::invalid_argument( stream.str() );
    }


    /// =============================================================================================================


    bool Level::verifyMap( const bool silent, const bool skipScriptVerification ) {
        if ( rawLevel.verifyMap( silent, skipScriptVerification ) == false ) {
            return false;
        }
        const std::size_t creaturesNum = rawLevel.countAllCreatures();
        if ( creaturesNum > 255 ) {
            LOG() << "limit of 255 creatures exceeded, creatures number: " << creaturesNum;
            return false;
        }
        return true;
    }

    std::size_t Level::countSolid( const utils::Point& point, const std::size_t radius ) const {
        Rect rect( point );
        rect.grow( radius );
        return rawLevel.countSolid( rect );
    }

    std::size_t Level::countSolid( const utils::Rect& rect ) const {
        return rawLevel.countSolid( rect );
    }

    Slab Level::getSlab( const std::size_t x, const std::size_t y ) {
        const raw::SlabType slab = rawLevel.getSlab( x, y );
        return convertToSlab( slab );
    }

    Slab Level::getSlab( const Point& point ) {
        return getSlab( point.x, point.y );
    }

    bool Level::isSlab( const utils::Point& point, const Slab type ) const {
        const raw::SlabType rawType = convertToSlab( type );
        return rawLevel.isSlab( point, rawType );
    }

    bool Level::isFortified( const utils::Point& point ) const {
        return rawLevel.isFortified( point );
    }

    bool Level::isOwner( const std::size_t x, const std::size_t y, const Player owner ) const {
        const raw::PlayerType playerType = convertToRaw( owner );
        const raw::PlayerType currOwner = rawLevel.getOwner( x, y );
        return ( playerType == currOwner );
    }

    bool Level::isOwner( const utils::Point& point, const Player owner ) const {
        const raw::PlayerType playerType = convertToRaw( owner );
        const raw::PlayerType currOwner = rawLevel.getOwner( point.x, point.y );
        return ( playerType == currOwner );
    }

    void Level::setSlab( const std::size_t x, const std::size_t y, const Slab type ) {
        const raw::SlabType rawType = convertToSlab( type );
        rawLevel.setSlab( x, y, rawType );
    }

    void Level::setSlab( const Point& point, const Slab type ) {
        const raw::SlabType rawType = convertToSlab( type );
        rawLevel.setSlab( point, rawType );
    }

    void Level::setSlab( const Rect& rect, const Slab type ) {
        const raw::SlabType rawType = convertToSlab( type );
        rawLevel.setSlab( rect, rawType );
    }

    void Level::setSlab( const std::set< Point >& positions, const Slab type ) {
        const raw::SlabType rawType = convertToSlab( type );
        rawLevel.setSlab( positions, rawType );
    }

    void Level::setSlab( const PointList& positions, const Slab type ) {
        const raw::SlabType rawType = convertToSlab( type );
        rawLevel.setSlab( positions, rawType );
    }

    void Level::setClaimed( const Point& point, const Player owner ) {
        const raw::PlayerType playerType = convertToRaw( owner );
        rawLevel.setSlab( point, raw::SlabType::ST_CLAIMED, playerType );
    }

    void Level::setClaimed( const Rect& rect, const Player owner ) {
        const raw::PlayerType playerType = convertToRaw( owner );
        rawLevel.setSlab( rect, raw::SlabType::ST_CLAIMED, playerType );
    }

    void Level::setFortified( const Point& point, const Player owner ) {
        const raw::PlayerType playerType = convertToRaw( owner );
        rawLevel.setSlab( point.x, point.y, raw::SlabType::ST_WALLDRAPE, playerType );          /// fortified wall
    }

    void Level::setFortified( const utils::Rect& rect, const Player owner ) {
        const raw::PlayerType playerType = convertToRaw( owner );
        const PointSet points = outline( rect, 1 );
        for ( const Point item: points ) {
            rawLevel.setSlab( item.x, item.y, raw::SlabType::ST_WALLDRAPE, playerType );
        }
    }

    void Level::setRoom( const Rect& position, const Room room ) {
        const raw::SlabType rawType = convertToSlab( room );
        rawLevel.setSlab( position, rawType );
    }

    void Level::setRoom( const Rect& position, const Room room, const Player owner, const bool fortify ) {
        const raw::SlabType rawType = convertToSlab( room );
        const raw::PlayerType playerType = convertToRaw( owner );
        rawLevel.setRoom( position, rawType, playerType, fortify );
    }

    void Level::setRoom( const utils::Point& position, const Room room, const Player owner, const bool fortify ) {
        const raw::SlabType rawType = convertToSlab( room );
        const raw::PlayerType playerType = convertToRaw( owner );
        const Rect rect( position, 1, 1 );
        rawLevel.setRoom( rect, rawType, playerType, fortify );
    }

    std::size_t Level::countItems( const utils::Point& point ) const {
        return rawLevel.countItems( point );
    }

    std::size_t Level::countItems( const utils::Rect& rect ) const {
        return rawLevel.countItems( rect );
    }

    void Level::setTrap( const std::size_t x, const std::size_t y, const Trap trap ) {
        const raw::SubTypeTrap rawTrap = convertToRaw( trap );
        rawLevel.setTrap( x, y, 4, rawTrap );
    }

    void Level::setTrap( const Point& point, const Trap trap ) {
        const raw::SubTypeTrap rawTrap = convertToRaw( trap );
        rawLevel.setTrap( point, 4, rawTrap );
    }

    void Level::setTrap( const Point& point, const std::size_t subIndex, const Trap trap ) {
        const raw::SubTypeTrap rawTrap = convertToRaw( trap );
        rawLevel.setTrap( point, subIndex, rawTrap );
    }

    bool Level::canPlaceDoor( const utils::Point& point ) const {
        return rawLevel.canPlaceDoor( point );
    }

    void Level::setDoor( const std::size_t x, const std::size_t y, const Door door, const bool locked ) {
        const raw::SubTypeDoor rawDoor = convertToRaw( door );
        rawLevel.setDoor( x, y, rawDoor, locked );
    }

    void Level::setDoor( const utils::Point& point, const Door door, const bool locked ) {
        const raw::SubTypeDoor rawDoor = convertToRaw( door );
        rawLevel.setDoor( point, rawDoor, locked );
    }

    void Level::setCreature( const std::size_t x, const std::size_t y, const std::size_t subIndex, const Creature creature, const std::size_t number, const std::size_t expLevel, const Player owner ) {
        const raw::SubTypeCreature creatureType = convertToRaw( creature );
        const raw::PlayerType playerType = convertToRaw( owner );
        rawLevel.setCreature( x, y, subIndex, creatureType, number, expLevel, playerType );
    }

    void Level::setCreature( const Point& point, const std::size_t subIndex, const Creature creature, const std::size_t number, const std::size_t expLevel, const Player owner ) {
        const raw::SubTypeCreature creatureType = convertToRaw( creature );
        const raw::PlayerType playerType = convertToRaw( owner );
        rawLevel.setCreature( point, subIndex, creatureType, number, expLevel, playerType );
    }

    void Level::setCreatureAuto( const std::size_t x, const std::size_t y, const Creature creature, const std::size_t number, const std::size_t expLevel, const Player owner ) {
        if ( x >= raw::RawLevel::MAP_SIZE_X || y >= raw::RawLevel::MAP_SIZE_Y ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return ;
        }

        const raw::SubTypeCreature creatureType = convertToRaw( creature );
        const raw::PlayerType playerType        = convertToRaw( owner );

        std::size_t creaturesNum = number;
        std::size_t sub = 0;
        while ( sub < 9 && creaturesNum > 0 ) {
            const std::size_t insertNum = min( (std::size_t) 5, creaturesNum );
            creaturesNum -= insertNum;

            rawLevel.setCreature( x, y, sub, creatureType, insertNum, expLevel, playerType );
            ++sub;
        }
    }

    void Level::setCreatureAuto( const Point& point, const Creature creature, const std::size_t number, const std::size_t expLevel, const Player owner ) {
        setCreatureAuto( point.x, point.y, creature, number, expLevel, owner );
    }

    void Level::setItem( const Point& point, const Item item ) {
        const raw::SubTypeItem rawItem = convertToRaw( item );
        rawLevel.setItem( point, rawItem );
    }

    void Level::setItem( const Point& point, const std::size_t subIndex, const Item item ) {
        const raw::SubTypeItem rawItem = convertToRaw( item );
        rawLevel.setItem( point, subIndex, rawItem );
    }

    void Level::setItem( const Rect& rect, const std::size_t subIndex, const Item item ) {
        const raw::SubTypeItem rawItem = convertToRaw( item );
        rawLevel.setItem( rect, subIndex, rawItem );
    }

    /// ==============================================================================

    utils::Point Level::getActionPointPosition( const std::size_t accessPoint ) {
        return rawLevel.getActionPointPosition( accessPoint );
    }

    std::size_t Level::addActionPoint( const Point& point, const std::size_t radius ) {
        return rawLevel.addActionPoint( point.x, point.y, 4, radius );
    }

    utils::Point Level::getHeroGatePosition( const std::size_t gateNum ) {
        return rawLevel.getHeroGatePosition( gateNum );
    }

    /// ==============================================================================

    std::size_t Level::setCave( const Rect& boundingLimit, const Slab type, const std::size_t itemsNum ) {
        if ( itemsNum < 1 ) {
            return 0;
        }
        const int area = boundingLimit.area();
        if ( area < 0 ) {
            return 0;
        }
        if ( itemsNum >= (std::size_t) area ) {
            setSlab( boundingLimit, type );
            return (std::size_t) area;
        }

//        std::set< Point > available;
//        for ( int x=boundingLimit.min.x; x<=boundingLimit.max.x; ++x ) {
//            for ( int y=boundingLimit.min.y; y<=boundingLimit.max.y; ++y ) {
//                available.insert( Point(x,y) );
//            }
//        }
//        std::set< Point > added;
//        for ( std::size_t i=0; i<itemsNum; ++i ) {
//            const std::size_t vIndex = rng_randi() % available.size();
//            auto avpos = available.begin();
//            std::advance( avpos, vIndex );
//            added.insert( *avpos );
//            available.erase( avpos );
//        }
//        setSlab( added, type );
//        return added.size();

        LakeGenerator vein;
        vein.add( boundingLimit.center() );
        vein.generate( boundingLimit, itemsNum );

        setSlab( vein.added, type );
        return vein.added.size();
    }

    void Level::digLine( const Point& from, const Point& to, const Slab type ) {
        const PointList points = line( from, to );
        digCorridor( points, type );
    }

    void Level::digLine( const Point& from, const Point& to, const Player owner, const bool fortify ) {
        const PointList points = line( from, to );
        digClaimed( points, owner, fortify );
    }

    void Level::digCorridor( const utils::PointList& corridor, const Slab type ) {
        const raw::SlabType slabType = convertToSlab( type );
        for ( const Point& item: corridor ) {
            const raw::SlabType currSlab = rawLevel.getSlab( item );
            if ( raw::isEarth( currSlab ) ||
                 raw::isWall( currSlab ) ||
                 raw::isLiquid( currSlab ) ||
                 raw::isImpassable( currSlab ) )
            {
                rawLevel.setSlab( item, slabType );
            }
        }
    }

    void Level::digClaimed( const utils::PointList& corridor, const Player owner, const bool fortify ) {
        const raw::PlayerType playerType = convertToRaw( owner );
        for ( const Point& item: corridor ) {
            const raw::SlabType currSlab = rawLevel.getSlab( item );
            if ( raw::isEarth( currSlab ) ||
                 raw::isWall( currSlab ) ||
                 raw::isLiquid( currSlab ) ||
                 raw::isImpassable( currSlab ) )
            {
                rawLevel.setSlab( item, raw::SlabType::ST_CLAIMED, playerType );
                if ( fortify ) {
                    rawLevel.fortify( item, playerType );
                }
            }
        }
    }

} /* namespace adiktedpp */
