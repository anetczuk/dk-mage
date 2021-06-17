/*
 * CaveMode.cpp
 *
 */

#include "dkmage/mode/CaveMode.h"

#include "dkmage/Draw.h"
#include "dkmage/generator/Dungeon.h"
#include "dkmage/BaseLevelGenerator.h"

#include "adiktedpp/Script.h"

#include <random>


namespace dkmage {
    namespace mode {

        /**
         *
         */
        class CaveGenImplementaton: public BaseLevelGenerator {
        public:

            void generate( const std::size_t seed ) override {
                LOG() << "generating level";
                srand( seed );
                generateLevel();
            }

            void generateLevel() {
                level.generateRandomMap( 9 );

                dkmage::generator::Dungeon dungeon;
                dungeon.limitNorth = 1;
                dungeon.limitSouth = 0;
                dungeon.fortify( true );

                dungeon.generate( 1, 5 );
//                dungeon.addRandomRoom( adiktedpp::SlabType::ST_TREASURE, 3, false, 1 );
                dungeon.moveToBottomEdge( 0 );

//                LOG() << "dungeon:\n" << dungeon.print();

                drawDungeon( level, dungeon );

                {
                    dkmage::generator::Room* first = dungeon.room(0);
                    const Point firstCenter = first->position().center();
                    const Rect bbox = dungeon.boundingBox();

                    /// add neutral portal
                    const Point portalCenter( bbox.max.x + 16, firstCenter.y );
                    const Rect portalRect( portalCenter, 3, 3 );
                    level.setSlab( portalRect, adiktedpp::SlabType::ST_PORTAL );
            //        level.setSlabRoom( portalRect, adiktedpp::SlabType::ST_PORTAL, adiktedpp::PlayerType::PT_UNSET );

                    /// add gold ore
                    const Point veinCenter( bbox.min.x - 20, firstCenter.y - 1 );
                    const Rect veinRect( veinCenter, 9, 5 );
                    const Point gemCenter( veinCenter.x, veinCenter.y + 3 );
                    const Rect gemRect( gemCenter, 3, 3 );
                    level.setSlab( gemRect, adiktedpp::SlabType::ST_ROCK );
                    level.setSlab( gemCenter, adiktedpp::SlabType::ST_GEMS );
                    level.setVein( veinRect, adiktedpp::SlabType::ST_GOLD, 30 );

                    /// add other
                    Point pos = firstCenter + Point(0, 2);
                    level.setSlab( pos, adiktedpp::SlabType::ST_CLAIMED, adiktedpp::PlayerType::PT_0 );
                    level.setItem( pos, 4, adiktedpp::SubTypeItem::STI_SPREVMAP );
                    level.setCreature( firstCenter.x, firstCenter.y-2, 3, adiktedpp::SubTypeCreature::STC_IMP, 4 );
                    level.setCreature( firstCenter.x, firstCenter.y-2, 5, adiktedpp::SubTypeCreature::STC_IMP, 4 );
//                    level.setCreature( firstCenter.x, firstCenter.y+2, 4, adiktedpp::SubTypeCreature::STC_SKELETON, 4 );
//                    level.setCreature( firstCenter.x+2, firstCenter.y, 4, adiktedpp::SubTypeCreature::STC_WARLOCK, 1 );

            //        /// fill treasure with gold
            //        std::vector< dkmage::generator::Room* > treasureRooms = dungeon.findRoom( adiktedpp::SlabType::ST_TREASURE );
            //        if ( treasureRooms.empty() == false ) {
            //            dkmage::generator::Room* treasure = treasureRooms.front();
            //            const Rect& roomRect = treasure->position();
            //            level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
            //        }
                }


            //    dkmage::generator::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_GOOD );
                dkmage::generator::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_1 );
                enemyDungeon.limitNorth = 0;
                enemyDungeon.limitSouth = 1;
                enemyDungeon.fortify( true );

//                enemyDungeon.generate( 6, 5 );
                enemyDungeon.generate( 12, 5 );
                enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_PORTAL, 3 );
                enemyDungeon.moveToTopEdge( 0 );

            //    LOG() << "enemy dungeon:\n" << enemyDungeon.print();

                drawDungeon( level, enemyDungeon );

                {
                    dkmage::generator::Room* first = enemyDungeon.room(0);
                    const Point firstCenter = first->position().center();
                    const Rect bbox = enemyDungeon.boundingBox();

                    /// add gold ore
                    const Point veinCenter( bbox.max.x + 12, firstCenter.y + 1 );
                    const Rect veinRect( veinCenter, 9, 5 );
                    const Point gemCenter( veinCenter.x, veinCenter.y - 2 );        /// 3 faces
                    const Rect gemRect( gemCenter, 3, 3 );
                    level.setSlab( gemRect, adiktedpp::SlabType::ST_ROCK );
                    level.setVein( veinRect, adiktedpp::SlabType::ST_GOLD, 30 );
                    level.setSlab( gemCenter, adiktedpp::SlabType::ST_GEMS );

                    /// add other
                    level.setCreature( firstCenter.x, firstCenter.y+2, 3, adiktedpp::SubTypeCreature::STC_IMP, 4 );
                    level.setCreature( firstCenter.x, firstCenter.y+2, 5, adiktedpp::SubTypeCreature::STC_IMP, 4 );
//                    level.setCreature( firstCenter.x, firstCenter.y-2, 4, adiktedpp::SubTypeCreature::STC_SKELETON, 1 );

            //        /// fill treasure with gold
            //        std::vector< dkmage::generator::Room* > treasureRooms = enemyDungeon.findRoom( adiktedpp::SlabType::ST_TREASURE );
            //        if ( treasureRooms.empty() == false ) {
            //            dkmage::generator::Room* treasure = treasureRooms.front();
            //            const Rect& roomRect = treasure->position();
            //            level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
            //        }
                }

            //    LOG() << "items:\n" << level.printItems();

                /// === scripting ===

                adiktedpp::Script script( level );

                script.addLine( "SET_GENERATE_SPEED( 500 )" );

                script.addLine( "COMPUTER_PLAYER( PLAYER1, 0 )" );
                script.addLine( "MAX_CREATURES( PLAYER0, 30 )" );
                script.addLine( "MAX_CREATURES( PLAYER1, 50 )" );

                script.addLine( "START_MONEY( PLAYER0, 20000 )" );               /// does not show in treasure
                script.addLine( "START_MONEY( PLAYER1, 100000 )" );              /// does not show in treasure

                script.addLine( "" );
                script.setEvilCreaturesPool( 30 );
                script.addLine( "" );
                script.setEvilCreaturesAvailable( adiktedpp::PlayerType::PT_ALL );
//                script.setEvilCreaturesAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_POSSIBLE );
                script.addLine( "" );
                script.setRoomsAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_POSSIBLE );
                script.setRoomAvailable( adiktedpp::PlayerType::PT_0, adiktedpp::SlabType::ST_TREASURE, adiktedpp::AvailableMode::AM_ENABLED );
                script.setRoomAvailable( adiktedpp::PlayerType::PT_0, adiktedpp::SlabType::ST_LAIR, adiktedpp::AvailableMode::AM_ENABLED );
                script.setRoomAvailable( adiktedpp::PlayerType::PT_0, adiktedpp::SlabType::ST_HATCHERY, adiktedpp::AvailableMode::AM_ENABLED );
                script.setRoomAvailable( adiktedpp::PlayerType::PT_0, adiktedpp::SlabType::ST_LIBRARY, adiktedpp::AvailableMode::AM_ENABLED );
//                script.setRoomsAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_ENABLED );
                script.addLine( "" );
                script.setDoorsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );
                script.addLine( "" );
                script.setTrapsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );
                script.addLine( "" );
                script.setMagicAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_POSSIBLE );
                script.setMagicAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::SubTypeItem::STI_SPELLIMP, adiktedpp::AvailableMode::AM_ENABLED );
                script.setMagicAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::SubTypeItem::STI_SPELLSOE, adiktedpp::AvailableMode::AM_DISABLED );
//                script.setMagicAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_ENABLED );
                script.addLine( "" );
//                script.addLine( "MAGIC_AVAILABLE( ALL_PLAYERS, POWER_DESTROY_WALLS, 1, 1 )" );

                script.addLine( "" );
                script.addLine( "REM --- main script ---" );
                script.addLine( "" );
                script.addLine( "IF( PLAYER0, ALL_DUNGEONS_DESTROYED == 1 )" );
                script.addLine( "    WIN_GAME" );
                script.addLine( "ENDIF" );

                script.rebuild();

                /// =================

                const bool valid = level.verifyMap();
                LOG() << "is map valid: " << valid;
                if ( valid == false ) {
                    LOG() << "detected invalid map -- restarting generation";
                    generateLevel();
                    return ;
                }
            }

        };


        /// =======================================================


        CaveMode::CaveMode(): LevelGeneratorWrapper( new CaveGenImplementaton() ) {
        }

    } /* namespace mode */
} /* namespace dkmage */
