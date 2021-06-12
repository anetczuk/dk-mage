/*
 * Cave.cpp
 *
 */

#include "dkmage/mode/Cave.h"

#include "utils/Log.h"



//#include "dkmage/generator/Dungeon.h"
//
//#include "adiktedpp/Level.h"
//#include "adiktedpp/Script.h"
//#include "adiktedpp/Messages.h"
//
//#include <random>
//#include <time.h>                                   /// time
//
//
//inline std::string getKeeperDataPath() {
//    #if defined(KEEPER_DATA_PATH)
//        return std::string(KEEPER_DATA_PATH);
//    #else
//        return "";
//    #endif
//}
//
//inline std::string getKeeperLevelsPath() {
//    #if defined(KEEPER_LEVELS_PATH)
//        return std::string(KEEPER_LEVELS_PATH);
//    #else
//        return "";
//    #endif
//}
//
//
//void fortifyRoom( adiktedpp::Level& level, const Rect& position, const adiktedpp::PlayerType owner ) {
//    for ( int x = position.min.x-1; x<= position.max.x+1; ++x ) {
//        for ( int y = position.min.y-1; y<= position.max.y+1; ++y ) {
//            const adiktedpp::SlabType currSlab = level.getSlab( x, y );
//            if ( adiktedpp::isEarth( currSlab ) ) {
//                level.setSlab( x, y, adiktedpp::SlabType::ST_WALLDRAPE, owner );
//            }
//        }
//    }
//}
//
//void fortifyRoom( adiktedpp::Level& level, const Point& position, const adiktedpp::PlayerType owner ) {
//    for ( int x = position.x-1; x<= position.x+1; ++x ) {
//        for ( int y = position.y-1; y<= position.y+1; ++y ) {
//            const adiktedpp::SlabType currSlab = level.getSlab( x, y );
//            if ( adiktedpp::isEarth( currSlab ) ) {
//                level.setSlab( x, y, adiktedpp::SlabType::ST_WALLDRAPE, owner );
//            }
//        }
//    }
//}
//
//void digLine( adiktedpp::Level& level, const Point& from, const Point& to, const adiktedpp::PlayerType owner, const bool fortify ) {
//    const std::vector<Point> points = line( from, to );
//    for ( const Point& item: points ) {
//        const adiktedpp::SlabType currSlab = level.getSlab( item );
//        if ( adiktedpp::isEarth( currSlab ) || adiktedpp::isWall( currSlab ) || (currSlab == adiktedpp::SlabType::ST_ROCK) ) {
//            level.setSlab( item, adiktedpp::SlabType::ST_CLAIMED, owner );
//            if ( fortify ) {
//                fortifyRoom( level, item, owner );
//            }
//        }
//    }
//}
//
//void drawRoom( adiktedpp::Level& level, const Rect& position,
//               const adiktedpp::SlabType room, const adiktedpp::PlayerType owner, const bool fortify )
//{
//    if ( room != adiktedpp::SlabType::ST_DUNGHEART ) {
//        level.setSlabRoom( position, room, owner );
//        if ( fortify ) {
//            fortifyRoom( level, position, owner );
//        }
//        return ;
//    }
//    if ( position.width() > 3 && position.height() > 3 ) {
//        level.setSlabRoom( position, adiktedpp::SlabType::ST_CLAIMED, owner );
//        const Point center = position.center();
//        const Rect shrink(center, 3, 3);
//        level.setSlabRoom( shrink, room, owner );
//    } else {
//        level.setSlabRoom( position, room, owner );
//    }
//    if ( fortify ) {
//        fortifyRoom( level, position, owner );
//    }
//}
//
//void drawDungeon( adiktedpp::Level& level, dkmage::generator::Dungeon& dungeon ) {
//    const adiktedpp::PlayerType owner = dungeon.owner();
//    const bool fortify = dungeon.fortify();
//    std::vector< dkmage::generator::Room* > roomsList = dungeon.rooms();
//    for ( const dkmage::generator::Room* item: roomsList ) {
//        /// set room
//        const Rect& position = item->position();
//        const adiktedpp::SlabType itemType = item->type();
//        drawRoom( level, position, itemType, owner, fortify );
//
//        /// draw corridors
//        const Point& itemCenter = item->position().center();
//        std::vector< dkmage::generator::Room* > connectedList = dungeon.connectedRooms( *item );
//        for ( const dkmage::generator::Room* connected: connectedList ) {
//            const Point& connectedCenter = connected->position().center();
//            digLine( level, itemCenter, connectedCenter, owner, fortify );
//        }
//    }
//}
//
//
//void generateLevel() {
//    dkmage::generator::Dungeon dungeon;
//    dungeon.fortify( true );
//    dungeon.generate( 12, 5 );
//    dungeon.centerize( 42, 42 );
//    {
//        const Rect bbox = dungeon.boundingBox();
//        const int yoffset = 80 - bbox.max.y;
//        dungeon.move( 0, yoffset );
//    }
//
////    dkmage::generator::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_GOOD );
//    dkmage::generator::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_1 );
//    enemyDungeon.fortify( true );
//    enemyDungeon.generate( 6, 5 );
////    enemyDungeon.generate( 12, 5 );
//    enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_PORTAL, 3 );
//    enemyDungeon.centerize( 42, 42 );
//    {
//        const Rect bbox = enemyDungeon.boundingBox();
//        const int yoffset = 4 - bbox.min.y;
//        enemyDungeon.move( 0, yoffset );
//    }
//
//    LOG() << "dungeon:\n" << dungeon.print();
////    LOG() << "enemy dungeon:\n" << enemyDungeon.print();
//
//    adiktedpp::ScopeMessages messages( "cli.log.txt" );
//    adiktedpp::Level level;
//
//    const std::string& keeperData = getKeeperDataPath();
//    level.setDataPath( keeperData );
//
//    LOG() << "drawing map";
//
//    level.generateRandomMap();
//
//    drawDungeon( level, dungeon );
//    drawDungeon( level, enemyDungeon );
//
//    {
//        dkmage::generator::Room* first = dungeon.room(0);
//        const Point firstCenter = first->position().center();
//
//        /// add neutral portal
//        const Rect bbox = dungeon.boundingBox();
//        Point portalCenter = firstCenter;
//        portalCenter.x = bbox.max.x + 9;
//        const Rect portalRect( portalCenter, 3, 3 );
//        level.setSlab( portalRect, adiktedpp::SlabType::ST_PORTAL );
////        level.setSlabRoom( portalRect, adiktedpp::SlabType::ST_PORTAL, adiktedpp::PlayerType::PT_UNSET );
//
//        // add other
//        Point pos = firstCenter - Point(0, 3);
//        level.setSlab( pos, adiktedpp::SlabType::ST_CLAIMED, adiktedpp::PlayerType::PT_0 );
//        level.setItem( pos, 4, adiktedpp::SubTypeItem::STI_SPREVMAP );
//        level.setCreature( firstCenter.x, firstCenter.y-2, 3, adiktedpp::SubTypeCreature::STC_IMP, 4 );
//        level.setCreature( firstCenter.x, firstCenter.y-2, 5, adiktedpp::SubTypeCreature::STC_IMP, 4 );
//        level.setCreature( firstCenter.x, firstCenter.y+2, 4, adiktedpp::SubTypeCreature::STC_SKELETON, 4 );
//        level.setCreature( firstCenter.x+2, firstCenter.y, 4, adiktedpp::SubTypeCreature::STC_WARLOCK, 1 );
//
////        /// fill treasure with gold
////        std::vector< dkmage::generator::Room* > treasureRooms = dungeon.findRoom( adiktedpp::SlabType::ST_TREASURE );
////        if ( treasureRooms.empty() == false ) {
////            dkmage::generator::Room* treasure = treasureRooms.front();
////            const Rect& roomRect = treasure->position();
////            level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
////        }
//    }
//    {
//        dkmage::generator::Room* first = enemyDungeon.room(0);
//        const Point firstCenter = first->position().center();
//        level.setCreature( firstCenter.x, firstCenter.y+2, 3, adiktedpp::SubTypeCreature::STC_IMP, 2 );
//        level.setCreature( firstCenter.x, firstCenter.y+2, 5, adiktedpp::SubTypeCreature::STC_IMP, 2 );
//        level.setCreature( firstCenter.x, firstCenter.y-2, 4, adiktedpp::SubTypeCreature::STC_SKELETON, 1 );
//
////        /// fill treasure with gold
////        std::vector< dkmage::generator::Room* > treasureRooms = enemyDungeon.findRoom( adiktedpp::SlabType::ST_TREASURE );
////        if ( treasureRooms.empty() == false ) {
////            dkmage::generator::Room* treasure = treasureRooms.front();
////            const Rect& roomRect = treasure->position();
////            level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
////        }
//    }
//
////    LOG() << "items:\n" << level.printItems();
//
//    /// === scripting ===
//
//    adiktedpp::Script script( level );
//
//    script.addLine( "SET_GENERATE_SPEED( 500 )" );
//
//    script.addLine( "COMPUTER_PLAYER( PLAYER1, 0 )" );
//    script.addLine( "MAX_CREATURES( PLAYER0, 30 )" );
//    script.addLine( "MAX_CREATURES( PLAYER1, 50 )" );
//
//    script.addLine( "START_MONEY( PLAYER0, 50000 )" );               /// does not show in treasure
//    script.addLine( "START_MONEY( PLAYER1, 500000 )" );              /// does not show in treasure
//
//    script.addLine( "" );
//    script.setEvilCreaturesPool( 30 );
//    script.addLine( "" );
//    script.setEvilCreaturesAvailable( adiktedpp::PlayerType::PT_ALL );
//    script.addLine( "" );
//    script.setRoomsAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_ENABLED );
//    script.addLine( "" );
//    script.setDoorsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );
//    script.addLine( "" );
//    script.setTrapsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );
//    script.addLine( "" );
//    script.setMagicAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_ENABLED );
//    script.addLine( "" );
//    script.addLine( "MAGIC_AVAILABLE( ALL_PLAYERS, POWER_DESTROY_WALLS, 1, 1 )" );
//
//    script.addLine( "" );
//    script.addLine( "REM --- main script ---" );
//    script.addLine( "" );
//    script.addLine( "IF( PLAYER0, ALL_DUNGEONS_DESTROYED == 1 )" );
//    script.addLine( "    WIN_GAME" );
//    script.addLine( "ENDIF" );
//
//    script.rebuild();
//
//    /// =================
//
//    level.setRescale( 3 );
//    if ( level.generateBmp( "level.bmp" ) == false ) {
//        messages.readRecent();
//    }
//
//    const bool valid = level.verifyMap();
//    LOG() << "is map valid: " << valid;
//    if ( valid == false ) {
//        return ;
//    }
//
//    const std::string mapOutPath = getKeeperLevelsPath() + "/mAp00333";
//    const bool saved = level.saveMapByPath( mapOutPath );
////    const bool saved = level.saveMapById( 333 );
//    LOG() << "is map saved: " << saved;
//    if ( saved == false ) {
//        return ;
//    }
//}



namespace dkmage {
    namespace mode {

        struct ModeData {

        };


        Cave::Cave() {
        }

        void Cave::generate( const std::size_t seed ) {
            LOG() << "generating level with seed " << seed;
            srand( seed );
//            generateLevel();
        }

        void Cave::storeLevel( const std::string& levelPath ) {
            LOG() << "storing level in: " << levelPath;
        }

        void Cave::storePreview( const std::string& filePath ) {
            LOG() << "storing bmp in: " << filePath;
        }

    } /* namespace mode */
} /* namespace dkmage */
