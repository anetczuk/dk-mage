/*
 * CaveMode.cpp
 *
 */

#include "dkmage/mode/CaveMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/script/Script.h"

#include "utils/Set.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        void CaveMode::generateLevel() {
            level.generateRandomMap( 9 );

            generateCaves( 28 );

            preparePlayerDungeon();

            prepareEnemyDungeon();

            /// =========== scripting ===========
            LOG() << "preparing script";
            script::Script script( level );
            {
                const std::string type = parameters.getString( ParameterName::PN_TYPE, "" );
                const std::string seed = parameters.getString( ParameterName::PN_SEED, "" );
                script.storeParameters( type, seed );
            }

            script.addLine( "" );
            script.addLine( "SET_GENERATE_SPEED( 500 )" );

            script.addLine( "COMPUTER_PLAYER( PLAYER1, 0 )" );
            script.addLine( "MAX_CREATURES( PLAYER0, 30 )" );
            script.addLine( "MAX_CREATURES( PLAYER1, 50 )" );

            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                script.addLine( "START_MONEY( PLAYER0, 200000 )" );             /// does not show in treasure
            } else {
                script.addLine( "START_MONEY( PLAYER0,  20000 )" );             /// does not show in treasure
            }
            script.addLine( "START_MONEY( PLAYER1, 200000 )" );               /// does not show in treasure

            script.addLine( "" );
            script.setEvilCreaturesPool( 30 );

            script.addLine( "" );
            script.setEvilCreaturesAvailable( Player::P_ALL );

            script.addLine( "" );
            script.setRoomsStandard();

            script.addLine( "" );
            script.setDoorsAvailable( Player::P_ALL, 0 );

            script.addLine( "" );
            script.setTrapsAvailable( Player::P_ALL, 0 );

            script.addLine( "" );
            script.setMagicStandard( Player::P_ALL );

            script.addLine( "" );
            script.addLine( "" );
            script.addLine( "REM --- main script ---" );
            script.addLine( "" );
            script.setWinConditionStandard( Player::P_P0 );

            script.rebuild();

            /// ========================================================

            const bool valid = level.verifyMap();
            if ( valid == false ) {
                LOG() << "detected invalid map -- restarting generation";
                generateLevel();
                return ;
            }
        }

        void CaveMode::generateCaves( const std::size_t cavesNum ) {
            const Rect mapRect = raw::RawLevel::mapRect();
            const Point mapCenter = mapRect.center();
            const Rect region( mapCenter, 70, 35 );

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

        void CaveMode::preparePlayerDungeon() {
            spatial::Dungeon dungeon( Player::P_P0 );
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                dungeon.generate( 4, 5 );
            } else {
                dungeon.generate( 1, 5 );
            }

            dungeon.moveToBottomEdge( 4 );

//               LOG() << "dungeon:\n" << dungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, dungeon );

            const spatial::DungeonRoom* heart = dungeon.room( 0 );
            const Point firstCenter = heart->position().center();
            const Rect bbox = dungeon.boundingBox();

            /// add neutral portal
            const Point portalCenter( bbox.max.x + 16, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.setRoom( portalRect, Room::R_PORTAL );

            /// add gold vein
            const Rect mapRect = raw::RawLevel::mapRect();
            const int veinY = mapRect.max.y - 4;
            const Point leftVeinCenter( bbox.min.x - 20, veinY );
            const Rect leftVeinRect( leftVeinCenter, 9, 5 );

            std::size_t gemsNum = parameters.getSizeT( ParameterName::PN_GEM_FACES_NUMBER, 1 );
            gemsNum = std::min( gemsNum, (std::size_t)4 );
            LOG() << "gems number: " << gemsNum;

            drawGoldVein( level, leftVeinRect, gemsNum );

            const Point rightVeinCenter( std::min(bbox.max.x + 28, 78 ), veinY );
            const Rect rightVeinRect( rightVeinCenter, 9, 6 );
            drawGoldVein( level, rightVeinRect, 0 );

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
//                   level.setCreature( firstCenter.x, firstCenter.y+2, 4, SubTypeCreature::STC_SKELETON, 4 );
//                   level.setCreature( firstCenter.x+2, firstCenter.y, 4, SubTypeCreature::STC_WARLOCK, 1 );

//                   /// fill treasure with gold
//                   spatial::DungeonRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                   if ( treasure != nullptr ) {
//                       const Rect& roomRect = treasure->position();
//                       level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                   }
        }

        void CaveMode::prepareEnemyDungeon() {
        //    spatial::Dungeon enemyDungeon( Player::PT_GOOD );
            spatial::Dungeon enemyDungeon( Player::P_P1 );
            enemyDungeon.limitNorth = 0;
            enemyDungeon.limitSouth = 2;
            enemyDungeon.fortify( true );

//                enemyDungeon.generate( 6, 5 );
            enemyDungeon.generate( 12, 5 );
//                enemyDungeon.addRandomRoom( SlabType::ST_PORTAL, 3 );
            enemyDungeon.moveToTopEdge( 4 );

        //    LOG() << "enemy dungeon:\n" << enemyDungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, enemyDungeon );

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
            const std::vector< spatial::DungeonRoom* > treasures = enemyDungeon.findRoom( Room::R_TREASURE );
            for ( const spatial::DungeonRoom* treasure: treasures ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, Item::I_GOLD_HOARD3 );
            }

//                    LOG() << "items: " << level.printItems();
        }

    } /* namespace mode */
} /* namespace dkmage */
