/*
 * MazeMode.cpp
 *
 */

#include "dkmage/mode/MazeMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Maze.h"
#include "dkmage/spatial/Dungeon.h"
#include "dkmage/ParameterDefaults.h"

#include "utils/ProbabilityMass.h"
#include "utils/Container.h"
#include "utils/Rand.h"

#include <cmath>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        enum class MazeRoom {
            MR_DIAGONAL,
            MR_DIAMOND,
            MR_CORNERS,
            MR_HERO
        };

        /// ================================================

        bool MazeMode::generate() {
            Level& level = map.level;

            level.generateRandomMap( 9 );

            spatial::Maze maze;
            maze.corridorSize = 3;
            maze.generate( 21, 6 );
//                maze.corridorSize = 1;
//                maze.generate( 41, 12 );

            maze.centerizeOnMap();

            drawMaze( level, maze );

            generateMazeItems( maze );

            LOG_INFO() << "preparing dungeons";
            preparePlayerDungeon();

            prepareEnemyDungeon();

            LOG_INFO() << "preparing script";
            prepareScript();

            return true;
        }

        void MazeMode::generateMazeItems( spatial::Maze& maze ) {
            Level& level = map.level;

            const std::size_t corrNum = maze.corridorsNum();
            std::set< std::size_t > indexSet;
            for ( std::size_t i=0; i<corrNum; ++i ) {
                indexSet.insert( i );
            }

//                {
//                    const std::size_t furthestIndex = maze.getFurthestIndex();
//                    if ( indexSet.erase( furthestIndex ) > 0 ) {
//                        const Rect furthest = maze.nodeRect( furthestIndex );
//                        drawSpecial( level, furthest, SubTypeItem::STI_SPMULTPLY );
//                    }
//                }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 20, 5 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_REVMAP );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 0, 0 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_INCLEV );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 0, 5 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_INCLEV );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 20, 0 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_INCLEV );
                }
            }

            {
                const std::set< std::size_t > deadEndList = maze.getDeadEnds();
                for ( std::size_t item: deadEndList ) {
                    if ( indexSet.erase( item ) > 0 ) {
                        const Rect furthest = maze.nodeRect( item );
                        drawGoldChests( level, furthest );
                    }
                }
            }

            std::set< Trap > damageTraps = Traps();
            damageTraps.erase( Trap::T_LAVA );                          /// remove lava traps, because bridges are disabled

            ProbabilityMass< Trap > trapProbability;
            trapProbability.set( damageTraps, 10.0 );
            trapProbability.set( Trap::T_LIGHTNING, 5.0 );
            trapProbability.normalize();

            ProbabilityMass< MazeRoom > roomProbability;
            roomProbability.set( MazeRoom::MR_DIAGONAL, 1.0 );
            roomProbability.set( MazeRoom::MR_DIAMOND, 1.0 );
            roomProbability.set( MazeRoom::MR_CORNERS, 1.0 );
            roomProbability.set( MazeRoom::MR_HERO, 1.0 );
            roomProbability.normalize();

            const int maxDistance = maze.maxDistance();

            const std::size_t trapsChambers = maze.corridorsNum() * 0.3;
            LOG_INFO() << "generating traps chambers: " << trapsChambers;
            for ( std::size_t i=0; i<trapsChambers; ++i ) {
                const std::size_t setIndex = rng_randi( indexSet.size() );
                const std::size_t cIndex = get_item( indexSet, setIndex, true );
                const std::size_t nx = cIndex % maze.nodesX();
                const std::size_t ny = cIndex / maze.nodesX();
                const Rect nodeRect = maze.nodeRect( nx, ny );
                const Point nodeCenter = nodeRect.center();
                const int nodeDistance = maze.nodeDistance( nx, ny );

                const Trap trapType = trapProbability.getRandom();

                if ( maze.corridorSize == 1 ) {
                    level.setSlab( nodeCenter, Slab::S_PATH );
                    level.setTrap( nodeCenter, 4, trapType );
                    continue ;
                }
                if ( maze.corridorSize == 3 ) {
                    const MazeRoom roomType = roomProbability.getRandom();

                    switch( roomType ) {
                    case MazeRoom::MR_DIAGONAL: {
                        drawTrap3x3X( level, nodeCenter, trapType, Slab::S_PATH );
                        break ;
                    }
                    case MazeRoom::MR_DIAMOND: {
                        drawTrap3x3Diamond( level, nodeCenter, trapType, Slab::S_PATH );
                        break ;
                    }
                    case MazeRoom::MR_CORNERS: {
                        drawTrap3x3Corners( level, nodeCenter, trapType, Slab::S_PATH );
                        break ;
                    }
                    case MazeRoom::MR_HERO: {
                        const double distanceFactor = ((double) nodeDistance) / maxDistance * 2.0;
                        randHeroTrap( maze, nodeCenter, distanceFactor );
                        break ;
                    }
                    }
                }
            }
        }

        void MazeMode::randHeroTrap( spatial::Maze& maze, const Point& nodeCenter, const double distanceFactor ) {
            Level& level = map.level;

//                level.setVein( chamber, SlabType::ST_PATH, cSize );
            const Rect chamber( nodeCenter, maze.corridorSize, maze.corridorSize );
            level.setSlab( chamber, Slab::S_PATH );

            int creatureLevel = std::min( (int)(distanceFactor * 9 + 1), 9 );          /// in range [1, 9]
            creatureLevel = std::max( creatureLevel, 3 );                              /// in range [3, 9]

            drawHeroTrap( level, nodeCenter, creatureLevel );
        }

        void MazeMode::preparePlayerDungeon() {
            spatial::EvilDungeon dungeon;
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                dungeon.limitNorth = 0;
                dungeon.limitSouth = 0;
                dungeon.generate( 4, 5 );
            } else {
                dungeon.generate( 1, 5 );
            }

            dungeon.moveToBottomEdge( 4 );

            {
                /// randomize dungeon position
                Rect randRect( 25, 11 );
                randRect.centerize();
                randRect.moveBottomTo( 0 );
                const std::size_t randomPosIndex = rng_randi( randRect.area() );
                const Point center = randRect.pointByIndex( randomPosIndex );
                dungeon.move( center );
            }

            /// add gold vein
            generateGoldSlabs( PN_DEFAULT_GOLD_SLABS_NUMBER_MAZE, PN_DEFAULT_GEM_SLABS_NUMBER_MAZE, PN_DEFAULT_GEM_FACES_NUMBER_MAZE, PN_DEFAULT_GEM_TRAPS_NUMBER_MAZE, 4, 25 );

            Level& level = map.level;

            /// dungeon have to be drawn before placing items inside it's rooms
            dungeon.draw( level );

            const spatial::EvilRoom* heart = dungeon.room( 0 );
            const Point firstCenter = heart->position().center();

            {
                /// add neutral portal
                const Rect mapRect = raw::RawLevel::mapRect();
                Rect randRect( 25, 11 );
                randRect.moveRightTo( mapRect.max.x - 7 );
                randRect.moveBottomTo( mapRect.max.y - 3 );
                const std::size_t randomPosIndex = rng_randi( randRect.area() );
                const Point portalCenter = randRect.pointByIndex( randomPosIndex );
                const Rect portalRect( portalCenter, 3, 3 );
                level.setRoom( portalRect, Room::R_PORTAL );
            }

            /// add other
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                const Point revPos = heart->edgePoint( spatial::Direction::D_SOUTH );
                level.setItem( revPos, Item::I_SPECIAL_REVMAP );

                const adiktedpp::Player player = dungeon.owner();
                level.setFortified( revPos + Point(0, 1), player );
                level.setSlab( revPos + Point(0, 2), Slab::S_EARTH );
                const Point monstersPos = revPos + Point(0, 3);
                level.setSlab( monstersPos, Slab::S_PATH );
                level.setCreatureAuto( monstersPos, Creature::C_MISTRESS, 20, 10 );
            }

            const adiktedpp::Player player = dungeon.owner();
            level.setCreature( firstCenter.x-1, firstCenter.y-2, 4, Creature::C_WARLOCK, 1, 1, player );
            level.setCreature( firstCenter.x+1, firstCenter.y-2, 4, Creature::C_SKELETON, 3, 1, player );
            level.setCreatureAuto( firstCenter.x, firstCenter.y+2, Creature::C_IMP, 8, 1, player );

//                    /// fill treasure with gold
//                    spatial::DungeonRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        void MazeMode::prepareEnemyDungeon() {
            spatial::EvilDungeon enemyDungeon( Player::P_P1 );
            enemyDungeon.limitNorth = 0;
            enemyDungeon.limitSouth = 2;
            enemyDungeon.fortify( true );

            const std::size_t roomsNum = parameters.getSizeT( ParameterName::PN_ENEMY_KEEPER_ROOMS_NUMBER, PN_DEFAULT_ENEMY_KEEPER_ROOMS_NUMBER_MAZE ) + 1;
            enemyDungeon.generate( roomsNum, 5 );

            enemyDungeon.moveToTopEdge( 4 );

        //    LOG_INFO() << "enemy dungeon:\n" << enemyDungeon.print();

            Level& level = map.level;

            /// dungeon have to be drawn before placing items inside it's rooms
            enemyDungeon.draw( level );

            const Point firstCenter = enemyDungeon.roomCenter( 0 );
            const Rect bbox = enemyDungeon.boundingBox();
            const Player owner = enemyDungeon.owner();

            /// add portal
            const Point portalCenter( bbox.min.x - 8, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.digLine( firstCenter, portalCenter, owner, true );
            level.setRoom( portalRect, Room::R_PORTAL, owner, true );

            /// add gold ore
            const Point veinCenter( bbox.max.x + 12, 4 );
            level.digLine( firstCenter, veinCenter );
            const Rect veinRect( veinCenter, 9, 5 );
            drawGoldVein( level, veinRect, 3 );

            /// add other
            level.setCreatureAuto( firstCenter.x, firstCenter.y+2, Creature::C_IMP, 8 );
//                    level.setCreature( firstCenter.x, firstCenter.y-2, 4, SubTypeCreature::STC_SKELETON, 1 );

            /// fill treasure with gold
            const std::vector< spatial::EvilRoom* > treasures = enemyDungeon.findRoom( Room::R_TREASURE );
            for ( const spatial::EvilRoom* treasure: treasures ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, Item::I_GOLD_HOARD3 );
            }
        }

        void MazeMode::prepareScript() {
            adiktedpp::script::Script& script = map.script;
            script::ScriptCommand& initSection = script.initSection();

            script.setFXLevel();

            initSection.addEmptyLine();
            initSection.SET_GENERATE_SPEED( 500 );

            const std::size_t aiAttitude = parameters.getSizeT( ParameterName::PN_ENEMY_KEEPER_ATTITUDE, PN_DEFAULT_ENEMY_KEEPER_ATTITUDE_MAZE );
            initSection.COMPUTER_PLAYER( Player::P_P1, aiAttitude );

            std::size_t initialGold = parameters.getSizeT( ParameterName::PN_INIT_GOLD_AMOUNT, PN_DEFAULT_INIT_GOLD_AMOUNT );
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                initialGold += 200000;
            }
            initSection.START_MONEY( Player::P_P0, initialGold );                /// does not show in treasure
            initSection.START_MONEY( Player::P_P1, 250000 );                     /// does not show in treasure

            const std::size_t maxCreatures   = parameters.getSizeT( ParameterName::PN_CREATURES_LIMIT, PN_DEFAULT_CREATURES_LIMIT_MAZE );
            const std::size_t aiMaxCreatures = parameters.getSizeT( ParameterName::PN_ENEMY_KEEPER_CREATURES_LIMIT, PN_DEFAULT_ENEMY_KEEPER_CREATURES_LIMIT_MAZE );
            initSection.MAX_CREATURES( Player::P_P0, maxCreatures );
            initSection.MAX_CREATURES( Player::P_P1, aiMaxCreatures );

            initSection.addEmptyLine();
            script.setHeroCreaturesPool( 20 );

            const std::set< Player > availablePlayers = { Player::P_P0, Player::P_P0 };

            script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setHeroAvailable( Player::P_P0 );

            script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( Player::P_ALL, Room::R_BRIDGE, script::RoomAvailableMode::RAM_DISABLED );

            /// necromancer mode
            availableCreatures.setEvilAvailable( Player::P_P0, false );
            availableRooms.setStateMode( Player::P_P0, Room::R_TORTURE, script::RoomAvailableMode::RAM_DISABLED );

            script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( Player::P_ALL, true );
            availableTraps.setStateFlag( Player::P_ALL, Trap::T_LAVA, false );

            initSection.addEmptyLine();
            script.set( availableCreatures );

            initSection.addEmptyLine();
            script.set( availableRooms );

            initSection.addEmptyLine();
            script.setDoorsAvailable( Player::P_ALL, 0 );

            initSection.addEmptyLine();
            script.set( availableTraps );

            initSection.addEmptyLine();
//            script.setMagicStandard( Player::P_ALL );
            script::MagicAvailableState availableMagic( availablePlayers );
            availableMagic.setStandard( Player::P_ALL );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_DESTROY_WALLS, script::MagicAvailableMode::AM_DISABLED );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_ARMAGEDDON, script::MagicAvailableMode::AM_DISABLED );
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                availableMagic.setStateMode( Player::P_P0, Spell::S_POWER_CALL_TO_ARMS, script::MagicAvailableMode::AM_AVAILABLE );
                availableMagic.setStateMode( Player::P_P0, Spell::S_POWER_DESTROY_WALLS, script::MagicAvailableMode::AM_AVAILABLE );
//                magicAvailableState.setAllAvailable( Player::P_P0, script::AvailableMode::AM_ENABLED );
            }
            script.set( availableMagic );

//            script.addLineInit( "" );
//            script.concealWholeMap( Player::P_P0 );

//            script.addLineInit( "" );
//            script.setImpRotting( false );

            /// end game conditions
            script.setWinConditionStandard( Player::P_P0 );
            script.setLoseConditionStandard( Player::P_P0 );
        }

    } /* namespace mode */
} /* namespace dkmage */
