/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/script/Script.h"

#include "utils/Rand.h"


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
            adiktedpp::script::Script script( level );

            script.addLine( "SET_GENERATE_SPEED( 500 )" );

            script.addLine( "MAX_CREATURES( PLAYER0, 10 )" );

            script.addLine( "START_MONEY( PLAYER0,  20000 )" );                 /// does not show in treasure

            script.addLine( "" );
            script.addLine( "" );
            const std::set< adiktedpp::PlayerType > availablePlayers = { adiktedpp::PlayerType::PT_0 };

            adiktedpp::script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setEvilAvailable( adiktedpp::PlayerType::PT_0 );
            script.set( availableCreatures );

            script.addLine( "" );
            adiktedpp::script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( adiktedpp::PlayerType::PT_ALL, adiktedpp::script::Room::R_BRIDGE, adiktedpp::script::AvailableMode::AM_DISABLED );
            script.set( availableRooms );

            script.addLine( "" );
            script.setDoorsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );

            script.addLine( "" );
//            script.setTrapsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );
            adiktedpp::script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( adiktedpp::PlayerType::PT_ALL, true );
            availableTraps.setStateFlag( adiktedpp::PlayerType::PT_ALL, adiktedpp::script::Trap::T_LAVA, false );
            script.set( availableTraps );

            script.addLine( "" );
//            script.setMagicStandard( adiktedpp::PlayerType::PT_ALL );
            adiktedpp::script::MagicAvailableState availableMagic( availablePlayers );
            availableMagic.setStandard( adiktedpp::PlayerType::PT_ALL );
            availableMagic.setStateMode( adiktedpp::PlayerType::PT_ALL, adiktedpp::script::Spell::S_POWER_DESTROY_WALLS, adiktedpp::script::AvailableMode::AM_DISABLED );
            availableMagic.setStateMode( adiktedpp::PlayerType::PT_ALL, adiktedpp::script::Spell::S_POWER_ARMAGEDDON, adiktedpp::script::AvailableMode::AM_DISABLED );
            script.set( availableMagic );

            script.addLine( "" );
            script.addLine( "" );
            script.addLine( "REM --- main script ---" );
            script.addLine( "" );
            script.setWinConditionStandard( adiktedpp::PlayerType::PT_0 );

            script.rebuild();

            /// ========================================================

            const bool valid = level.verifyMap();
            if ( valid == false ) {
                LOG() << "detected invalid map -- restarting generation";
//                    storePreview( "level.bmp" );
                generateLevel();
                return ;
            }
            if ( level.countClaimAreas() > 0 ) {
                LOG() << "map problem found: unclaimable areas";
                generateLevel();
                return ;
            }
        }

        void HeroFortressMode::generateCaves( const std::size_t cavesNum ) {
            const utils::Rect mapRect = adiktedpp::Level::mapRect();
            const utils::Point mapCenter = mapRect.center();
            const Rect region( mapCenter + utils::Point(0, 8), 70, 33 );

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
                const int regionIndex = utils::getSetItem( indexSet, itemIndex, true );
                const int rX = regionIndex % region.width();
                const int rY = regionIndex / region.width();

                const utils::Point caveCenter = region.min + utils::Point( rX, rY );
                const adiktedpp::SlabType centerStab = level.getSlab( caveCenter );
                if ( centerStab == adiktedpp::SlabType::ST_ROCK ) {
                    continue ;
                }

                const utils::Rect chamber( caveCenter, 5, 5 );
                level.setChamber( chamber, adiktedpp::SlabType::ST_PATH, 12 );

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
            level.setSlab( portalRect, adiktedpp::SlabType::ST_PORTAL );

            /// add gold vein
            const utils::Rect mapRect = adiktedpp::Level::mapRect();
            const int veinY = mapRect.max.y - 3;
            const Point veinCenter( bbox.min.x - 20, veinY );
            const Rect veinRect( veinCenter, 9, 5 );
            drawGoldVein( level, veinRect, 2 );

            /// add other
//            Point pos = firstCenter + Point(0, 2);
//            level.setItem( pos, 4, adiktedpp::SubTypeItem::STI_SPREVMAP );
            level.setCreatureAuto( firstCenter.x, firstCenter.y-2, adiktedpp::SubTypeCreature::STC_IMP, 8 );

//                    /// fill treasure with gold
//                    dkmage::spatial::Room* treasure = dungeon.findRoomFirst( adiktedpp::SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        void HeroFortressMode::prepareEnemyDungeon() {
            dkmage::spatial::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_GOOD );
            enemyDungeon.limitNorth = 0;
            enemyDungeon.limitSouth = 2;
            enemyDungeon.fortify( true );

            const dkmage::spatial::Room* heart = enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_DUNGHEART, 5 );
            enemyDungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 3, *heart, dkmage::spatial::Direction::D_NORTH, true, 1 );
            enemyDungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 3, *heart, dkmage::spatial::Direction::D_EAST, true, 1 );
            enemyDungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 3, *heart, dkmage::spatial::Direction::D_WEST, true, 1 );
            const dkmage::spatial::Room* southTr = enemyDungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 3, *heart, dkmage::spatial::Direction::D_SOUTH, true, 1 );
            const dkmage::spatial::Room* traps   = enemyDungeon.addRoom( adiktedpp::SlabType::ST_CLAIMED, 3, *southTr, dkmage::spatial::Direction::D_SOUTH, true, 3 );

            enemyDungeon.moveToTopEdge( 8 );

        //    LOG() << "enemy dungeon:\n" << enemyDungeon.print();

            Rect bbox = enemyDungeon.boundingBox();
            bbox.grow( 2 );
            level.setSlabOutline( bbox, adiktedpp::SlabType::ST_LAVA );

            Point bottom = bbox.center();
            bottom.y = bbox.max.y;
            level.setSlab( bottom, adiktedpp::SlabType::ST_EARTH );

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, enemyDungeon );

            const Point trapCenter = traps->position().center();
            level.setDoor( trapCenter.x, trapCenter.y - 2, adiktedpp::SubTypeDoor::STD_IRON, true );
            level.setDoor( trapCenter.x, trapCenter.y + 2, adiktedpp::SubTypeDoor::STD_IRON, true );
            drawTrap3x3Diamond( level, trapCenter, adiktedpp::SubTypeTrap::STT_BOULDER );
            drawTrap3x3Corners( level, trapCenter, adiktedpp::SubTypeTrap::STT_LIGHTNG );

            const Point firstCenter = enemyDungeon.roomCenter( 0 );

            /// add other
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-2, adiktedpp::SubTypeCreature::STC_SMURI, 7, 9 );
            level.setCreatureAuto( firstCenter.x-1, firstCenter.y-2, adiktedpp::SubTypeCreature::STC_THEFT, 9, 9 );
            level.setCreatureAuto( firstCenter.x,   firstCenter.y-2, adiktedpp::SubTypeCreature::STC_FAIRY, 9, 9 );
            level.setCreatureAuto( firstCenter.x+1, firstCenter.y-2, adiktedpp::SubTypeCreature::STC_GIANT, 9, 9 );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-2, adiktedpp::SubTypeCreature::STC_BARBARIN, 9, 9 );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-1, adiktedpp::SubTypeCreature::STC_KNIGHT, 7, 9 );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-1, adiktedpp::SubTypeCreature::STC_MONK, 9, 9 );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y, adiktedpp::SubTypeCreature::STC_WIZRD, 9, 9 );

            /// fill treasure with gold
            const std::vector< dkmage::spatial::Room* > treasures = enemyDungeon.findRoom( adiktedpp::SlabType::ST_TREASURE );
            for ( const auto* treasure: treasures ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
            }
        }

    } /* namespace mode */
} /* namespace dkmage */
