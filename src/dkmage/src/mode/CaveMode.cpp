/*
 * CaveMode.cpp
 *
 */

#include "dkmage/mode/CaveMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"
#include "dkmage/ParameterDefaults.h"

#include "utils/Container.h"
#include "utils/Rand.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        bool CaveMode::generate() {
            Level& level = map.level;

            level.generateRandomMap( 9 );

            generateCaves();

            preparePlayerDungeon();

            prepareEnemyDungeon();

            LOG_INFO() << "preparing script";
            prepareScript();

            return true;
        }

        void CaveMode::generateCaves() {
            Level& level = map.level;

            const Rect mapRect = raw::RawLevel::mapRect();
            const Point mapCenter = mapRect.center();
            const Rect region( mapCenter, 70, 35 );

            const int regionArea = region.area();
            if ( regionArea < 0 ) {
                LOG_INFO() << "invalid region area: " << regionArea;
                return ;
            }

            std::set< std::size_t > indexSet;
            for ( int i=0; i<regionArea; ++i ) {
                indexSet.insert( i );
            }

            const std::size_t cavesNum = parameters.getSizeT( ParameterName::PN_CENTAL_CAVERNS_NUMBER, PN_DEFAULT_CENTAL_CAVERNS_NUMBER_CAVE );
            const std::size_t trapsNum = std::min( (std::size_t)regionArea, cavesNum );

            const SizeTSet creatureNum = parameters.getSizeTSet( ParameterName::PN_CAVERN_CREATURES_NUMBER, PN_DEFAULT_CAVERN_CREATURES_NUMBER_CAVE );
            const SizeTSet creatureExp = parameters.getSizeTSet( ParameterName::PN_CAVERN_CREATURES_LEVEL, PN_DEFAULT_CAVERN_CREATURES_LEVEL_CAVE );

            for ( std::size_t i=0; i<trapsNum; ++i ) {
                const int itemIndex = rng_randi( indexSet.size() );
                const int regionIndex = get_item( indexSet, itemIndex, true );
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
                const double centerFactor = ( 1.0 - (double) 2.0 * centerDistance / region.height() );        /// in range [0.0, 1.0]
                const std::size_t creatureLevel = creatureExp.valueByFactor( centerFactor );
                const std::size_t creaturesNum  = creatureNum.randomized();
                drawHeroTrap( level, caveCenter, creatureLevel, creaturesNum );
            }
        }

        void CaveMode::preparePlayerDungeon() {
            spatial::EvilDungeon dungeon( Player::P_P0 );
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

//            {
//                dungeon.addRandomRoom( Room::R_PRISON, 5, true );
//                dungeon.addRandomRoom( Room::R_GRAVEYARD, 5, true );
//                const Point monstersPos = Point(40, 60);
//                level.setSlab( monstersPos, Slab::S_PATH );
//                level.setCreatureAuto( monstersPos, Creature::C_FAIRY, 9, 0, Player::P_GOOD );
//            }

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
            generateGoldSlabs( PN_DEFAULT_GOLD_SLABS_NUMBER_CAVE, PN_DEFAULT_GEM_SLABS_NUMBER_CAVE, PN_DEFAULT_GEM_FACES_NUMBER_CAVE, PN_DEFAULT_GEM_TRAPS_NUMBER_CAVE, 4, 40 );

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

            const adiktedpp::Player owner = dungeon.owner();
            level.setCreatureAuto( firstCenter.x, firstCenter.y-2, Creature::C_IMP, 8, 1, owner );

//                   /// fill treasure with gold
//                   spatial::DungeonRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                   if ( treasure != nullptr ) {
//                       const Rect& roomRect = treasure->position();
//                       level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                   }
        }

        void CaveMode::prepareEnemyDungeon() {
            spatial::EvilDungeon enemyDungeon( Player::P_P1 );
            enemyDungeon.limitNorth = 0;
            enemyDungeon.limitSouth = 2;
            enemyDungeon.fortify( true );

            const std::size_t roomsNum = parameters.getSizeT( ParameterName::PN_ENEMY_KEEPER_ROOMS_NUMBER, PN_DEFAULT_ENEMY_KEEPER_ROOMS_NUMBER_CAVE ) + 1;
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

//                    LOG_INFO() << "items: " << level.printItems();
        }

        void CaveMode::prepareScript() {
            adiktedpp::script::Script& script = map.script;
            script::ScriptCommand& initSection = script.initSection();

            script.setFXLevel();

            initSection.addEmptyLine();
            initSection.SET_GENERATE_SPEED( 500 );

            const std::size_t aiAttitude = parameters.getSizeT( ParameterName::PN_ENEMY_KEEPER_ATTITUDE, PN_DEFAULT_ENEMY_KEEPER_ATTITUDE_CAVE );
            initSection.COMPUTER_PLAYER( Player::P_P1, aiAttitude );

            std::size_t initialGold = parameters.getSizeT( ParameterName::PN_INIT_GOLD_AMOUNT, PN_DEFAULT_INIT_GOLD_AMOUNT );
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                initialGold += 200000;
            }
            initSection.START_MONEY( Player::P_P0, initialGold );                /// does not show in treasure
            initSection.START_MONEY( Player::P_P1, 200000 );                     /// does not show in treasure

            const std::size_t maxCreatures   = parameters.getSizeT( ParameterName::PN_CREATURES_LIMIT, PN_DEFAULT_CREATURES_LIMIT_CAVE );
            const std::size_t aiMaxCreatures = parameters.getSizeT( ParameterName::PN_ENEMY_KEEPER_CREATURES_LIMIT, PN_DEFAULT_ENEMY_KEEPER_CREATURES_LIMIT_CAVE );
            initSection.MAX_CREATURES( Player::P_P0, maxCreatures );
            initSection.MAX_CREATURES( Player::P_P1, aiMaxCreatures );

            initSection.addEmptyLine();
            script.setEvilCreaturesPool( 30 );

            initSection.addEmptyLine();
            script.setEvilCreaturesAvailable( Player::P_ALL );

            initSection.addEmptyLine();
            script.setRoomsStandard();

            initSection.addEmptyLine();
            script.setDoorsAvailable( Player::P_ALL, 0 );

            initSection.addEmptyLine();
            script.setTrapsAvailable( Player::P_ALL, 0 );

            initSection.addEmptyLine();
//            script.setMagicStandard( Player::P_ALL );
            script::MagicAvailableState availableMagic;
            availableMagic.setStandard( Player::P_ALL );
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                availableMagic.setStateMode( Player::P_P0, Spell::S_POWER_CALL_TO_ARMS, script::MagicAvailableMode::AM_AVAILABLE );
                availableMagic.setStateMode( Player::P_P0, Spell::S_POWER_DESTROY_WALLS, script::MagicAvailableMode::AM_AVAILABLE );
//                availableMagic.setAllAvailable( Player::P_P0, script::AvailableMode::AM_ENABLED );
            }
            script.set( availableMagic );

//            script.addLineInit( "" );
//            script.concealWholeMap( Player::P_P0 );

//            script.addLineInit( "" );
//            script.setImpRotting( false );

//            /// action points
//            script.addLineAction( "IF_ACTION_POINT( 1, PLAYER0 )" );
//            script.addLineAction( "    DISPLAY_OBJECTIVE( 19, PLAYER0 )" );
//            script.addLineAction( "ENDIF" );

            /// end game conditions
            script.setWinConditionStandard( Player::P_P0 );
            script.setLoseConditionStandard( Player::P_P0 );
        }

    } /* namespace mode */
} /* namespace dkmage */
