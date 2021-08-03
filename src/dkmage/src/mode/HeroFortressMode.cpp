/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/LakeGenerator.h"
#include "adiktedpp/script/Script.h"

#include "utils/Rand.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        void HeroFortressMode::generateLevel() {
            LOG() << "preparing map";
            level.generateRandomMap( 9 );

            generateCaves( 28 );

            preparePlayerDungeon();

            prepareEnemyDungeon();

            /// =========== scripting ===========

            LOG() << "preparing script";
            script::Script script( level );

            script.addLine( "SET_GENERATE_SPEED( 500 )" );
            script.addLine( "MAX_CREATURES( PLAYER0, 10 )" );
            script.addLine( "START_MONEY( PLAYER0,  20000 )" );                 /// does not show in treasure

            script.addLine( "" );
            script.addLine( "" );
            const std::set< PlayerType > availablePlayers = { PlayerType::PT_0 };

            script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setEvilAvailable( PlayerType::PT_0 );
            script.set( availableCreatures );

            script.addLine( "" );
            script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( PlayerType::PT_ALL, Room::R_BRIDGE, script::AvailableMode::AM_DISABLED );
            script.set( availableRooms );

            script.addLine( "" );
            script.setDoorsAvailable( PlayerType::PT_ALL, 0 );

            script.addLine( "" );
//            script.setTrapsAvailable( PlayerType::PT_ALL, 0 );
            script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( PlayerType::PT_ALL, true );
            availableTraps.setStateFlag( PlayerType::PT_ALL, Trap::T_LAVA, false );
            script.set( availableTraps );

            script.addLine( "" );
//            script.setMagicStandard( PlayerType::PT_ALL );
            script::MagicAvailableState availableMagic( availablePlayers );
            availableMagic.setStandard( PlayerType::PT_ALL );
            availableMagic.setStateMode( PlayerType::PT_ALL, Spell::S_POWER_DESTROY_WALLS, script::AvailableMode::AM_DISABLED );
            availableMagic.setStateMode( PlayerType::PT_ALL, Spell::S_POWER_ARMAGEDDON, script::AvailableMode::AM_DISABLED );
            script.set( availableMagic );

            script.addLine( "" );
            script.addLine( "" );
            script.addLine( "REM --- main script ---" );
            script.addLine( "" );
            script.setWinConditionStandard( PlayerType::PT_0 );

            script.rebuild();

            /// ========================================================

            const bool valid = level.verifyMap();
            if ( valid == false ) {
                LOG() << "detected invalid map -- restarting generation";
//                    storePreview( "level.bmp" );
                generateLevel();
                return ;
            }
//            if ( level.countClaimAreas() > 0 ) {
//                LOG() << "map problem found: unclaimable areas";
//                generateLevel();
//                return ;
//            }
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
                const int itemIndex = rand() % indexSet.size();
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
            dkmage::spatial::Dungeon dungeon;
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            dungeon.generate( 1, 5 );
            dungeon.moveToBottomEdge( 4 );

//                LOG() << "dungeon:\n" << dungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, dungeon );

            const Point firstCenter = dungeon.roomCenter( 0 );
            const Rect bbox = dungeon.boundingBox();

            /// add neutral portal
            const Point portalCenter( bbox.max.x + 16, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.setRoom( portalRect, Room::R_PORTAL );

            /// add gold vein
            const Rect mapRect = raw::RawLevel::mapRect();
            const int veinY = mapRect.max.y - 3;
            const Point veinCenter( bbox.min.x - 20, veinY );
            const Rect veinRect( veinCenter, 9, 5 );
            drawGoldVein( level, veinRect, 2 );

            /// add other
            Point pos = firstCenter + Point(0, 2);
            level.setItem( pos, 4, Item::I_SPECIAL_REVMAP );
            level.setCreatureAuto( firstCenter.x, firstCenter.y-2, Creature::C_IMP, 8 );

//                    /// fill treasure with gold
//                    dkmage::spatial::Room* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        void HeroFortressMode::prepareEnemyDungeon() {
            dkmage::spatial::Dungeon enemyDungeon( PlayerType::PT_GOOD );
            enemyDungeon.limitNorth = 0;
            enemyDungeon.limitSouth = 2;
            enemyDungeon.fortify( true );

            const dkmage::spatial::Room* heart = enemyDungeon.addRandomRoom( Room::R_DUNGEON_HEART, 5 );
            enemyDungeon.addRoom( Room::R_TREASURE, 3, *heart, dkmage::spatial::Direction::D_NORTH, true, 1 );
            enemyDungeon.addRoom( Room::R_TREASURE, 3, *heart, dkmage::spatial::Direction::D_EAST, true, 1 );
            enemyDungeon.addRoom( Room::R_TREASURE, 3, *heart, dkmage::spatial::Direction::D_WEST, true, 1 );
            const dkmage::spatial::Room* southTr = enemyDungeon.addRoom( Room::R_TREASURE, 3, *heart, dkmage::spatial::Direction::D_SOUTH, true, 1 );
            const dkmage::spatial::Room* traps   = enemyDungeon.addRoom( Room::R_CLAIMED, 3, *southTr, dkmage::spatial::Direction::D_SOUTH, true, 3 );

            enemyDungeon.moveToTopEdge( 8 );

        //    LOG() << "enemy dungeon:\n" << enemyDungeon.print();

            const std::set< Point > outline = enemyDungeon.outline();
            level.setSlab( outline, Slab::S_LAVA );

            Rect bbox = enemyDungeon.boundingBox();
            bbox.grow( 4 );
            bbox.growWidth( 8 );

            LakeGenerator lavaLake;
            lavaLake.generateLake( bbox, 0.6 );
            LakeGenerator::grow( lavaLake.added, 1 );
            level.setSlab( lavaLake.added, Slab::S_LAVA );

            {
                /// make a bridge
                const Point trapsCenter = traps->position().center();
                const Point trapsExit   = trapsCenter + Point(0, 6);
                level.digLine( trapsCenter, trapsExit, Slab::S_EARTH );
            }

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, enemyDungeon );

            const Point trapCenter = traps->position().center();
            level.setDoor( trapCenter.x, trapCenter.y - 4, Door::D_STEEL, true );
            level.setTrap( trapCenter.x, trapCenter.y - 3, Trap::T_BOULDER );
            level.setDoor( trapCenter.x, trapCenter.y - 2, Door::D_STEEL, true );
            drawTrap3x3Diamond( level, trapCenter, Trap::T_BOULDER );
            drawTrap3x3Corners( level, trapCenter, Trap::T_LIGHTNING );
            level.setDoor( trapCenter.x, trapCenter.y + 2, Door::D_STEEL, true );

            const Point firstCenter = enemyDungeon.roomCenter( 0 );

            /// add other
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-2, Creature::C_SAMURAI, 7, 9 );
            level.setCreatureAuto( firstCenter.x-1, firstCenter.y-2, Creature::C_THIEF, 9, 9 );
            level.setCreatureAuto( firstCenter.x,   firstCenter.y-2, Creature::C_FAIRY, 9, 9 );
            level.setCreatureAuto( firstCenter.x+1, firstCenter.y-2, Creature::C_GIANT, 9, 9 );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-2, Creature::C_BARBARIAN, 9, 9 );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-1, Creature::C_KNIGHT, 7, 9 );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-1, Creature::C_MONK, 9, 9 );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y, Creature::C_WIZARD, 9, 9 );

            /// fill treasure with gold
            const std::vector< dkmage::spatial::Room* > treasures = enemyDungeon.findRoom( Room::R_TREASURE );
            for ( const dkmage::spatial::Room* treasure: treasures ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, Item::I_GOLD_HOARD3 );
            }
        }

    } /* namespace mode */
} /* namespace dkmage */
