/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"
#include "dkmage/spatial/Fortress.h"

#include "adiktedpp/LakeGenerator.h"
#include "adiktedpp/AreaDetector.h"

#include "utils/ProbabilityMass.h"
#include "utils/Set.h"

#include <cmath>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        /**
         *
         */
        class Fortress {
        public:

            Level& level;
            ParametersMap& parameters;
            spatial::FortressDungeon dungeon;


            Fortress( Level& level, ParametersMap& parameters ): level(level), parameters(parameters), dungeon( Player::P_GOOD ) {
                dungeon.limitNorth = 4;
                dungeon.limitSouth = 4;
                dungeon.fortify( true );
            }

            bool generate();

            void draw() const;

            std::vector< const spatial::FortressRoom* > prepareCorridors( const std::vector< const spatial::FortressRoom* >& startRooms, const std::size_t roomsNum, const bool allowBranches );

//            std::vector< const spatial::FortressRoom* > prepareRoom( const spatial::FortressRoomType roomType, const spatial::FortressRoom* startItem, const bool allowBranches );

            bool prepareBridge( const spatial::FortressRoom& entranceRoom );

            /// find shortest bridge in four directions
            std::vector< Point > findBridge( const Point startPoint );

            std::vector< Point > findBridge( const Point startPoint, const Point bridgeDirection );

        };


        bool Fortress::generate() {
            const spatial::FortressRoom* heart = dungeon.addRandomRoom( spatial::FortressRoomType::FR_DUNGEON_HEART, 5 );
            std::vector< const spatial::FortressRoom* > branchStart;
            branchStart.push_back( heart );
//            for ( std::size_t i=0; i<3; ++i ) {
//                branchStart.push_back( heart );
//            }

            const std::size_t corridorLength = rng_randi( 3 ) + 3;
            std::vector< const spatial::FortressRoom* > mainCorridor = prepareCorridors( branchStart, corridorLength, false );
            if ( mainCorridor.size() == 1 ) {
                const spatial::FortressRoom* lastRoom = mainCorridor.back();
                if ( lastRoom->restrictedDirections().empty() == false ) {
                    /// restricted room -- add unrestricted one
                    std::vector< const spatial::FortressRoom* > next = dungeon.addRandomRoom( spatial::FortressRoomType::FR_EMPTY, *lastRoom, false );
                    if ( next.size() != 1 ) {
                        LOG() << "unable to create main junction room";
                        return false;
                    }
                    mainCorridor = next;
                }
                const spatial::FortressRoom* corridorExit = mainCorridor.back();
                for ( std::size_t i=0; i<2; ++i ) {
                    mainCorridor.push_back( corridorExit );
                }
            }

            const std::size_t branchLength = rng_randi( 3 ) + 4;
            const std::vector< const spatial::FortressRoom* > entrances = prepareCorridors( mainCorridor, branchLength, true );
            if ( entrances.size() < 2 ) {
                LOG() << "unable create at least two branch corridors";
                if ( parameters.isSet( ParameterName::PN_STOP_ON_FAIL ) ) {
                    /// draw for debug purpose
                    dungeon.moveToTopEdge( 8 );
                    draw();
                }
                return false;
            }

            dungeon.moveToTopEdge( 8 );

            {
                /// generate lake
                Rect lakeLimit = raw::RawLevel::mapRect( 4 );
                lakeLimit.max.y -= 18;                                      /// make space for evil dungeon
                const Rect dungeonBBox = dungeon.boundingBox();
                if ( lakeLimit.hasInside( dungeonBBox ) == false ) {
                    LOG() << "fortress too big";
                    return false;
                }
                const std::set< Point > outline = dungeon.outline();
                if ( lakeLimit.hasInside( outline ) == false ) {
                    LOG() << "fortress too big";
                    return false;
                }

                LakeGenerator lavaLake;
                lavaLake.available = outline;
                const std::size_t lakeSize = outline.size() * 1.6;
                lavaLake.generate( lakeLimit, lakeSize );
                LakeGenerator::grow( lavaLake.added, 1, 0 );

                /// draw lake
                level.setSlab( lavaLake.added, Slab::S_LAVA );
                level.setSlab( outline, Slab::S_LAVA );

                /// draw 'EARTH' shore
                std::set< Point > lakeEdge = LakeGenerator::edge( lavaLake.added );
                level.setSlab( lakeEdge, Slab::S_EARTH );

                /// fill islands
                AreaDetector areaDetector;
                areaDetector.set( lavaLake.added, 1 );                   /// mark lava
                areaDetector.fill( 0, 0, 1 );                            /// fill outside as lava

                const std::vector< AreaDetector::AreaInfo > islands = areaDetector.fill( 0 );
                LOG() << "islands found: " << islands.size();

                for ( const AreaDetector::AreaInfo& item: islands ) {
                    const std::vector< utils::Point > points = item.cellsAsPoints();
                    level.setSlab( points, Slab::S_LAVA );
                }
            }

            /// dungeon have to be drawn before placing items inside it's rooms
            draw();

            /// prepare entrances
            std::size_t addedBridges = 0;
            for ( const spatial::FortressRoom* entrance: entrances ) {
                if ( prepareBridge( *entrance ) ) {
                    ++addedBridges;
                }
            }

            if ( addedBridges < 2 ) {
                LOG() << "unable create at least two bridges";
                return false;
            }

            const Point firstCenter = dungeon.roomCenter( 0 );

            /// add guards
            const Player owner = dungeon.owner();
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-2, Creature::C_SAMURAI, 7, 9, owner );
            level.setCreatureAuto( firstCenter.x-1, firstCenter.y-2, Creature::C_THIEF, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x,   firstCenter.y-2, Creature::C_FAIRY, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x+1, firstCenter.y-2, Creature::C_GIANT, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-2, Creature::C_BARBARIAN, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-1, Creature::C_KNIGHT, 7, 9, owner );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-1, Creature::C_MONK, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y, Creature::C_WIZARD, 9, 9, owner );

            /// fill treasure with gold
            std::vector< spatial::FortressRoom* > treasureRooms = dungeon.findRoom( spatial::FortressRoomType::FR_TREASURE );
            for ( const spatial::FortressRoom* treasure: treasureRooms ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, Item::I_GOLD_HOARD3 );
            }

            return true;
        }

        void Fortress::draw() const {
            dungeon.draw( level );
        }

        std::vector< const spatial::FortressRoom* > Fortress::prepareCorridors( const std::vector< const spatial::FortressRoom* >& startRooms, const std::size_t roomsNum, const bool allowBranches ) {
            std::vector< const spatial::FortressRoom* > roomQueue = startRooms;

            ProbabilityMass< spatial::FortressRoomType > roomProbability;
            roomProbability.set( spatial::FortressRoomType::FR_TREASURE, 0.25 );
            roomProbability.set( spatial::FortressRoomType::FR_CORRIDOR, 1.8 );
            roomProbability.set( spatial::FortressRoomType::FR_BRANCH, 0.3 );
            roomProbability.set( spatial::FortressRoomType::FR_BOULDER_CORRIDOR, 2.0 );
            roomProbability.set( spatial::FortressRoomType::FR_EMPTY, 1.0 );
            roomProbability.normalize();

            /// create branches
            for ( std::size_t i=0; i<roomsNum; ++i ) {
                std::vector< const spatial::FortressRoom* > nextRooms;
                const std::size_t qSize = roomQueue.size();
                for ( std::size_t x=0; x<qSize; ++x ) {
                    const spatial::FortressRoomType roomType = roomProbability.getRandom();
                    const spatial::FortressRoom* item = roomQueue[ x ];
                    std::vector< const spatial::FortressRoom* > next = dungeon.addRandomRoom( roomType, *item, allowBranches );
//                    std::vector< const spatial::FortressRoom* > next = prepareRoom( roomType, item, allowBranches );
                    const std::size_t nextSize = next.size();
                    if ( nextSize < 1 ) {
                        LOG() << "unable to generate chamber level " << i;
//                        LOG() << "unable to generate chamber level " << i << " " << roomType;
                        continue ;
                    }
                    nextRooms.insert( nextRooms.end(), next.begin(), next.end() );
                }
                roomQueue = nextRooms;
            }

            if ( allowBranches == false ) {
                return roomQueue;
            }

            /// create branch exit
            std::vector< const spatial::FortressRoom* > exitRooms;
            const std::size_t qSize = roomQueue.size();
            for ( std::size_t x=0; x<qSize; ++x ) {
                const spatial::FortressRoom* item = roomQueue[ x ];
                std::vector< const spatial::FortressRoom* > next = dungeon.addRandomRoom( spatial::FortressRoomType::FR_EXIT, *item, false );
                if ( next.empty() ) {
                    LOG() << "unable to generate branch exit";
//                    return std::vector< const spatial::FortressRoom* >();
                    continue ;
                }
                exitRooms.insert( exitRooms.end(), next.begin(), next.end() );
            }

            return exitRooms;
        }

//        std::vector< const spatial::FortressRoom* > Fortress::prepareRoom( const spatial::FortressRoomType roomType, const spatial::FortressRoom* startItem, const bool allowBranches ) {
////            static const std::size_t ROOM_MAX_SIZE = 4;
//            static ProbabilityMass<std::size_t> roomSize357;
//            if ( roomSize357.empty() ) {
//                roomSize357.set( 3, 1.0 );
//                roomSize357.set( 5, 1.0 );
//                roomSize357.set( 7, 0.25 );
//                roomSize357.normalize();
//            }
//
//            const std::size_t corridorLength = rng_randi( 5 ) + 1;
//
//            switch( roomType ) {
//            case spatial::FortressRoomType::FR_DUNGEON_HEART: {
////                const std::size_t rSize = roomSize357.getRandom();
////                const spatial::FortressRoom* next = dungeon.addRandomRoom( roomType, rSize, *startItem, true, corridorLength );
////                if ( next == nullptr ) {
////                    return {};
////                }
////                return {next};
//                return {nullptr};
//            }
//            case spatial::FortressRoomType::FR_TREASURE: {
//                const std::size_t rSize = roomSize357.getRandom();
//                const spatial::FortressRoom* next = dungeon.addRandomRoom( roomType, rSize, *startItem, true, corridorLength );
//                if ( next == nullptr ) {
//                    return {};
//                }
//                return {next};
//            }
//            case spatial::FortressRoomType::FR_CORRIDOR: {
//                std::size_t rSizeX = roomSize357.getRandom();
//                std::size_t rSizeY = 1;
//                if ( rng_randb() ) {
//                    std::swap( rSizeX, rSizeY );
//                }
//                const spatial::FortressRoom* next = dungeon.addRandomRoom( roomType, rSizeX, rSizeY, *startItem, true, corridorLength );
//                if ( next == nullptr ) {
//                    return {};
//                }
//                if ( allowBranches ) {
//                    if ( rng_randb( 0.3 ) ) {
//                        /// give a chance to generate branch
//                        return {next, next};
//                    }
//                }
//                return {next};
//            }
//            case spatial::FortressRoomType::FR_BRANCH: {
//                const spatial::FortressRoom* next = dungeon.addRandomRoom( roomType, 1, *startItem, true, corridorLength );
//                if ( next == nullptr ) {
//                    return {};
//                }
//                if ( allowBranches ) {
//                    return {next, next};            /// yes, added twice to make a branch
//                }
//                return {next};
//            }
//            case spatial::FortressRoomType::FR_BOULDER_CORRIDOR: {
//
//                const std::vector< spatial::Direction > availableDirs = dungeon.freeDirections( *startItem );
//
//                const spatial::FortressRoom* next = dungeon.addRandomRoom( roomType, 1, *startItem, true, corridorLength );
//                if ( next == nullptr ) {
//                    return {};
//                }
//                if ( allowBranches ) {
//                    return {next, next};            /// yes, added twice to make a branch
//                }
//                return {next};
//            }
//            case spatial::FortressRoomType::FR_EMPTY: {
//                const std::size_t rSize = roomSize357.getRandom();
//                const spatial::FortressRoom* next = dungeon.addRandomRoom( roomType, rSize, *startItem, true, corridorLength );
//                if ( next == nullptr ) {
//                    return {};
//                }
//                return {next};
//            }
//            }
//
//            LOG() << "unhandled fortress room: " << (int)roomType;
//            return {};
//        }

        bool Fortress::prepareBridge( const spatial::FortressRoom& entranceRoom ) {
            const std::vector< spatial::Direction > directions = dungeon.linkDirections( entranceRoom );
            const spatial::Direction corridorDirection = directions[ 0 ];
            const spatial::Direction entranceDirection = spatial::opposite( corridorDirection );
            const Point entrancePoint = entranceRoom.edgePoint( entranceDirection, 1 );

            const Point initialDirection = movePoint( Point(0, 0), entranceDirection, 1 );
            const Point bridgeStart = entrancePoint + initialDirection;
            const std::vector< Point > bridge = findBridge( bridgeStart );
            if ( bridge.size() < 2 ) {
                LOG() << "unable find bridge shore";
                return false;
            }

            level.setDoor( entrancePoint, Door::D_IRON, true );

            const Point bridgeDirection = bridge[1] - bridge[0];
            const Point bridgeOrtho = bridgeDirection.swapped();
            std::set< Point > neighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
            neighbourDirections.erase( -bridgeDirection );

            std::size_t i = 0;
            for ( const Point bridgePoint: bridge ) {
                ++i;

                /// add bridge keepers
                level.setSlab( bridgePoint, Slab::S_PATH );
                const int fairyLevel = rng_randi( 4 );
                level.setCreature( bridgePoint, 0, Creature::C_FAIRY, 2, 3 + fairyLevel, Player::P_GOOD );
                level.setCreature( bridgePoint, 2, Creature::C_FAIRY, 2, 3 + fairyLevel, Player::P_GOOD );
                if ( i == 1 ) {
                    level.setCreature( bridgePoint, 1, Creature::C_KNIGHT, 1, 7, Player::P_GOOD );
                }

                /// add turrets
                if ( i % 2 == 1 ) {
                    bool added = false;
                    const Point rightGuardPosition = bridgePoint + bridgeOrtho * 2;
                    if ( level.isSlab( rightGuardPosition, Slab::S_LAVA ) ) {
//                            const Rect guardPost( rightGuardPosition, 3, 3 );
//                            level.setSlab( guardPost, Slab::S_LAVA );
                        level.setSlab( rightGuardPosition, Slab::S_PATH );
                        level.setCreature( rightGuardPosition, 0, Creature::C_MONK, 1, 9, Player::P_GOOD );
                        level.setCreature( rightGuardPosition, 1, Creature::C_WIZARD, 1, 9, Player::P_GOOD );
                        level.setCreature( rightGuardPosition, 2, Creature::C_ARCHER, 1, 9, Player::P_GOOD );
                        added = true;
                    }
                    const Point leftGuardPosition = bridgePoint - bridgeOrtho * 2;
                    if ( level.isSlab( leftGuardPosition, Slab::S_LAVA ) ) {
//                            const Rect guardPost( leftGuardPosition, 3, 3 );
//                            level.setSlab( guardPost, Slab::S_LAVA );
                        level.setSlab( leftGuardPosition, Slab::S_PATH );
                        level.setCreature( leftGuardPosition, 0, Creature::C_MONK, 1, 9, Player::P_GOOD );
                        level.setCreature( leftGuardPosition, 1, Creature::C_WIZARD, 1, 9, Player::P_GOOD );
                        level.setCreature( leftGuardPosition, 2, Creature::C_ARCHER, 1, 9, Player::P_GOOD );
                        added = true;
                    }
                    if ( added == false ) {
                        ++i;
                    }
                }
            }

            return true;
        }

        std::vector< Point > Fortress::findBridge( const Point startPoint ) {
            static const std::vector< Point > directions = { Point( 1, 0 ), Point( -1, 0 ), Point( 0, 1 ), Point( 0, -1 ) };

            std::vector< Point > ret;

            for ( const Point dir: directions ) {
                Point nextPoint = startPoint + dir;
                if ( level.isSlab( nextPoint, Slab::S_LAVA ) == false ) {
                    continue ;
                }
                std::vector< Point > bridge = findBridge( startPoint, dir );
                if ( bridge.empty() ) {
                    continue ;
                }

                if ( ret.empty() ) {
                    ret = bridge;
                    continue ;
                }
                if ( ret.size() > bridge.size() ) {
                    ret = bridge;
                }
            }

            return ret;
        }

        std::vector< Point > Fortress::findBridge( const Point startPoint, const Point bridgeDirection ) {
            std::vector< Point > ret;

            std::set< Point > heighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
            heighbourDirections.erase( -bridgeDirection );

            Point bridgePoint = startPoint;
            while( true ) {
                if ( level.isSlab( bridgePoint, Slab::S_EARTH ) ) {
                    /// shore found
                    return ret;
                }
                if ( level.isSlab( bridgePoint, Slab::S_LAVA ) == false ) {
                    /// found other slab -- bridge didn't reach lake's shore
                    return std::vector< Point >();
                }

                ret.push_back( bridgePoint );

                /// search for shore
                bool shoreFound = false;
                for ( const Point item: heighbourDirections ) {
                    if ( level.isSlab( bridgePoint + item, Slab::S_EARTH ) ) {
                        /// lake's shore found
                        shoreFound = true;
                        break;
                    }
                }
                if ( shoreFound ) {
                    break;
                }

                bridgePoint += bridgeDirection;
            }

            return ret;
        }


        /// ===========================================================


        bool HeroFortressMode::generate() {
            level.generateRandomMap( 9 );

//            generateCaves( 28 );

            Fortress enemyFortress( level, parameters );
            if ( enemyFortress.generate() == false ) {
                return false;
            };

            preparePlayerDungeon();

            LOG() << "preparing script";
            prepareScript();

            return true;
        }

        bool HeroFortressMode::check() {
//            if ( level.countClaimAreas() > 0 ) {
//                LOG() << "map problem found: unclaimable areas";
//                return false;
//            }

            return true;
        }

        void HeroFortressMode::generateCaves( const std::size_t cavesNum ) {
            const Rect mapRect = raw::RawLevel::mapRect();
            const Point mapCenter = mapRect.center();
            const Rect region( mapCenter + Point(0, 8), 70, 33 );

            const int regionArea = region.area();
            if ( regionArea < 0 ) {
                LOG() << "invalid region area: " << regionArea;
                return ;
            }

            std::set< std::size_t > indexSet;
            for ( int i=0; i<regionArea; ++i ) {
                indexSet.insert( i );
            }

            const std::size_t trapsNum = std::min( (std::size_t)regionArea, cavesNum );

            for ( std::size_t i=0; i<trapsNum; ++i ) {
                const int itemIndex = rng_randi( indexSet.size() );
                const int regionIndex = getSetItem( indexSet, itemIndex, true );
                const int rX = regionIndex % region.width();
                const int rY = regionIndex / region.width();

                const Point caveCenter = region.min + Point( rX, rY );
                const Slab centerStab = level.getSlab( caveCenter );
                if ( centerStab == Slab::S_ROCK ) {
                    continue ;
                }

                const Rect chamber( caveCenter, 5, 5 );
                level.setCave( chamber, Slab::S_PATH, 12 );

                const int centerDistance = std::abs(mapCenter.y - caveCenter.y);
                const double centerFactor = ( 1.0 - 2.0 * centerDistance / region.height() );        /// in range [0.5, 1.0]
                const int creatureLevel = centerFactor * 7 + 3;
                drawHeroTrap( level, caveCenter, creatureLevel );
            }
        }

        void HeroFortressMode::preparePlayerDungeon() {
            spatial::EvilDungeon dungeon;
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            dungeon.generate( 1, 5 );
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                ///dungeon.limitNorth = 0;
                ///dungeon.limitSouth = 0;
                ///dungeon.generate( 4, 5 );

                spatial::EvilRoom* heart = dungeon.room( 0 );
                dungeon.addRoom( Room::R_TREASURE, 5, *heart, spatial::Direction::D_WEST );
                spatial::EvilRoom* hatchery = dungeon.addRoom( Room::R_HATCHERY, 5, *heart, spatial::Direction::D_EAST );
                dungeon.addRoom( Room::R_LAIR, 5, *hatchery, spatial::Direction::D_EAST );
            }

            dungeon.moveToBottomEdge( 4 );

            {
                /// randomize dungeon position
                Rect randRect( 21, 9 );
                randRect.centerize();
                randRect.moveBottomTo( 0 );
                const std::size_t randomPosIndex = rng_randi( randRect.area() );
                const Point center = randRect.pointByIndex( randomPosIndex );
                dungeon.move( center );
            }

            {
                /// add gold vein
                std::size_t goldSlabsNum = parameters.getSizeT( ParameterName::PN_GOLD_SLABS_NUMBER, 40 );
                LOG() << "gold slabs number: " << goldSlabsNum;

                std::size_t gemsNum = parameters.getSizeT( ParameterName::PN_GEM_FACES_NUMBER, 2 );
                gemsNum = std::min( gemsNum, (std::size_t)4 );
                LOG() << "gems number: " << gemsNum;

                generateLeftGoldVein( goldSlabsNum, gemsNum );
            }

            /// dungeon have to be drawn before placing items inside it's rooms
            dungeon.draw( level );

            const spatial::EvilRoom* heart = dungeon.room( 0 );
            const Point firstCenter = heart->position().center();

            {
                /// add neutral portal
                const Rect mapRect = raw::RawLevel::mapRect();
                Rect randRect( 21, 11 );
                randRect.moveRightTo( mapRect.max.x - 7 );
                randRect.moveBottomTo( mapRect.max.y - 3 );
                const std::size_t randomPosIndex = rng_randi( randRect.area() );
                const Point portalCenter = randRect.pointByIndex( randomPosIndex );
                const Rect portalRect( portalCenter, 3, 3 );
                level.setRoom( portalRect, Room::R_PORTAL );
            }

            /// add other
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                const Point revPos = heart->edgePoint( spatial::Direction::D_SOUTH );
                level.setItem( revPos, 4, Item::I_SPECIAL_REVMAP );

                const adiktedpp::Player player = dungeon.owner();
                level.setFortified( revPos + Point(0, 1), player );
                level.setSlab( revPos + Point(0, 2), Slab::S_EARTH );
                const Point monstersPos = revPos + Point(0, 3);
                level.setSlab( monstersPos, Slab::S_PATH );
                level.setCreatureAuto( monstersPos, Creature::C_MISTRESS, 20, 9 );
            }

            level.setCreatureAuto( firstCenter.x, firstCenter.y-2, Creature::C_IMP, 8 );

//                    /// fill treasure with gold
//                    spatial::FortressRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        void HeroFortressMode::prepareScript() {
//            script.setFXLevel();

            script.addLineInit( "SET_GENERATE_SPEED( 500 )" );
            script.addLineInit( "MAX_CREATURES( PLAYER0, 25 )" );

            std::size_t initialGold = parameters.getSizeT( ParameterName::PN_INIT_GOLD_AMOUNT, 20000 );
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                initialGold += 200000;
            }
            script.setStartMoney( Player::P_P0, initialGold );                /// does not show in treasure

            script.addLineInit( "" );
            script.setEvilCreaturesPool( 10 );

            script.addLineInit( "" );
            const std::set< Player > availablePlayers = { Player::P_P0 };
            script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setEvilAvailable( Player::P_P0 );
            script.set( availableCreatures );

            script.addLineInit( "" );
            script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( Player::P_ALL, Room::R_BRIDGE, script::AvailableRoomMode::ARM_DISABLED );
            script.set( availableRooms );

            script.addLineInit( "" );
            script.setDoorsAvailable( Player::P_ALL, 0 );

            script.addLineInit( "" );
//            script.setTrapsAvailable( Player::P_ALL, 0 );
            script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( Player::P_ALL, true );
            availableTraps.setStateFlag( Player::P_ALL, Trap::T_LAVA, false );
            script.set( availableTraps );

            script.addLineInit( "" );
//            script.setMagicStandard( Player::P_ALL );
            script::MagicAvailableState availableMagic( availablePlayers );
            availableMagic.setStandard( Player::P_ALL );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_DESTROY_WALLS, script::AvailableMode::AM_DISABLED );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_ARMAGEDDON, script::AvailableMode::AM_DISABLED );
            script.set( availableMagic );

//            script.addLineInit( "" );
//            script.setImpRotting( false );

            /// end game conditions
            script.setWinConditionStandard( Player::P_P0 );
            script.setLoseConditionStandard( Player::P_P0 );
        }

    } /* namespace mode */
} /* namespace dkmage */
