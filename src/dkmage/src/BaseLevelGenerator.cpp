/*
 * BaseLevelGenerator.cpp
 *
 */

#include "dkmage/BaseLevelGenerator.h"

#include "adiktedpp/Version.h"

#include "dkmage/Draw.h"
#include "dkmage/ParameterDefaults.h"

#include "utils/Rect.h"
#include "utils/Rand.h"

#include <cmath>
#include <fstream>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {

    void BaseLevelGenerator::generateLevel() {
        const bool succeed = generateAttempt();
        if ( succeed ) {
            return ;
        }
        if ( parameters.isSet( ParameterName::PN_STOP_ON_FAIL ) ) {
            /// do not attempt to generate
            LOG_WARN() << "detected invalid map -- stopping";
            return ;
        }
        LOG_WARN() << "detected invalid map -- restarting generation";
        generateLevel();
    }

    bool BaseLevelGenerator::generateAttempt() {
        LOG() << "generating map";

        /// reset/clear script
        map.script.clearData();
        Level& level = map.level;

        if ( generate() == false ) {
            LOG_WARN() << "could not generate map";
            return false;
        }

        std::vector< std::string > content = map.script.build();
        script::LevelScript dataLevel( level );
        dataLevel.rebuild( content );

        const bool extendedScript = map.script.isFXLevel();
        if ( extendedScript ) {
            LOG_WARN() << "detected FX script -- txt verification disabled";
        }

        if ( level.verifyMap( false, extendedScript ) == false ) {
            /// verification performed by adikted (for example if there is Dungeon Heart)
            LOG_WARN() << "generated level is invalid";
            return false;
        }

        LOG() << "creatures on map: " << level.getRawLevel().countAllCreatures();

        const std::size_t actionPointNum = level.getRawLevel().countActionPoints();
        const std::size_t actionPointsLimit = parameters.getSizeT( ParameterName::PN_ACTION_POINTS_LIMIT, PN_DEFAULT_ACTION_POINTS_LIMIT );
        LOG() << "action points number: " << actionPointNum << "/" << actionPointsLimit;
        if ( actionPointNum > actionPointsLimit ) {
            LOG_WARN() << "generated map is invalid -- too much action points";
            return false;
        }

        if ( checkScriptLimits() == false ) {
            /// custom checks depending on map mode
            LOG_WARN() << "generated map is invalid -- script limits";
            return false;
        }

        if ( check() == false ) {
            /// custom checks depending on map mode
            LOG_WARN() << "generated map is invalid -- custom checks";
            return false;
        }

        LOG() << "map generated successfully";
        return true;
    }

    bool BaseLevelGenerator::checkScriptLimits() const {
        const script::Script& script = map.script;

        const std::size_t tunnellers   = script.countTunnellerTriggers();
        const std::size_t parties      = script.countPartyTriggers();
        const std::size_t scriptValues = script.countScriptValues();
        const std::size_t ifConds      = script.countIfConditions();
        const std::size_t partyDefs    = script.countPartyDefinitions();

        const std::size_t tunnellersLimit = parameters.getSizeT( ParameterName::PN_SCRIPT_TUNNELLERS_LIMIT, PN_DEFAULT_SCRIPT_TUNNELLERS_LIMIT );
        const std::size_t partiesLimit    = parameters.getSizeT( ParameterName::PN_SCRIPT_PARTIES_LIMIT, PN_DEFAULT_SCRIPT_PARTIES_LIMIT );
        const std::size_t valuesLimit     = parameters.getSizeT( ParameterName::PN_SCRIPT_VALUES_LIMIT, PN_DEFAULT_SCRIPT_VALUES_LIMIT );
        const std::size_t ifCondsLimit    = parameters.getSizeT( ParameterName::PN_SCRIPT_IF_CONDS_LIMIT, PN_DEFAULT_SCRIPT_IF_CONDS_LIMIT );
        const std::size_t partyDefsLimit  = parameters.getSizeT( ParameterName::PN_SCRIPT_PARTY_DEFS_LIMIT, PN_DEFAULT_SCRIPT_PARTY_DEFS_LIMIT );

        LOG() << "used script resources: "
              << tunnellers           << "/" << tunnellersLimit << " tunneller triggers"
              << ", " << parties      << "/" << partiesLimit    << " party triggers"
              << ", " << scriptValues << "/" << valuesLimit     << " script values"
              << ", " << ifConds      << "/" << ifCondsLimit    << " IF conditions"
              << ", " << partyDefs    << "/" << partyDefsLimit  << " party definitions";

        if ( tunnellers > tunnellersLimit ) {
            return false;
        }
        if ( parties > partiesLimit ) {
            return false;
        }
        if ( scriptValues > valuesLimit ) {
            return false;
        }
        if ( ifConds > ifCondsLimit ) {
            return false;
        }
        if ( partyDefs > partyDefsLimit ) {
            return false;
        }

        return true;
    }

    void BaseLevelGenerator::writeIniFile() const {
        const Level& level = map.level;

        const std::string output = level.getRawLevel().outputFileName() + ".mage.ini";

        std::ofstream iniFile;
        iniFile.open( output );

        iniFile << "##\n";
        iniFile << "## map generated by dk-mage ver. " << VERSION_FULL_STRING << "\n";
        iniFile << "##\n";

        iniFile << "\n\n";
        iniFile << "[GENERAL]\n\n";

        const std::set< ParameterName >& localParameters = getLocalParameterNames();
        const std::set< std::string > localNames = getParameterName( localParameters );

        const ParametersMap::Data& rawParameters = parameters.rawData();
        auto iter  = rawParameters.begin();
        auto eiter = rawParameters.end();
        for ( ; iter != eiter; ++iter ) {
            if ( localNames.count( iter->first ) ) {
                /// skip local parameter
                continue ;
            }
            iniFile << iter->first << " = " << iter->second << "\n";
        }

        iniFile.close();
    }

    /// =============================================================================

    PointList findPassage( adiktedpp::Level& level, const Point startPoint, const Point bridgeDirection ) {
        PointList ret;

        std::set< Point > heighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
        heighbourDirections.erase( -bridgeDirection );

        Point bridgePoint = startPoint;
        while( true ) {
            if ( level.isSlab( bridgePoint, Slab::S_ROCK ) == false ) {
                /// passage found
                return ret;
            }

            ret.push_back( bridgePoint );

            /// search for passage
            bool passageFound = false;
            for ( const Point item: heighbourDirections ) {
                if ( level.isSlab( bridgePoint + item, Slab::S_ROCK ) == false ) {
                    /// passage found
                    passageFound = true;
                    break;
                }
            }
            if ( passageFound ) {
                break;
            }

            bridgePoint += bridgeDirection;
        }

        return ret;
    }

    void BaseLevelGenerator::generateGoldSlabs( const std::string& defaultGoldNum,
                                                const std::string& defaultGemSlabsNum, const std::string& defaultGemFacesNum, const std::string& defaultGemTrapsNum,
                                                const std::size_t gemRegionWidth, const std::size_t gemRegionHeight )
    {
        Level& level = map.level;
        script::Script& script = map.script;

        std::size_t goldSlabsNum = parameters.getSizeT( ParameterName::PN_GOLD_SLABS_NUMBER, defaultGoldNum );
        LOG() << "gold slabs number: " << goldSlabsNum;

        const std::size_t leftVeinGold = goldSlabsNum / 3;
        generateLeftGoldVein( leftVeinGold, 0 );

        const std::size_t rightVeinGold = goldSlabsNum - leftVeinGold;
        generateRightGoldVein( rightVeinGold, 0 );

        std::size_t gemSlabsNum = parameters.getSizeT( ParameterName::PN_GEM_SLABS_NUMBER, defaultGemSlabsNum );
        std::size_t gemFacesNum = parameters.getSizeT( ParameterName::PN_GEM_FACES_NUMBER, defaultGemFacesNum );
        std::size_t gemTrapsNum = parameters.getSizeT( ParameterName::PN_GEM_TRAPS_NUMBER, defaultGemTrapsNum );
        gemSlabsNum = std::min( gemSlabsNum, gemFacesNum );
        gemFacesNum = std::min( gemFacesNum, gemSlabsNum * 4 );

        LOG() << "gems slabs number: " << gemSlabsNum;
        LOG() << "gems faces number: " << gemFacesNum;
        LOG() << "gems traps number: " << gemTrapsNum;

        const SizeTSet gemGuardsNum  = parameters.getSizeTSet( ParameterName::PN_GEM_GUARD_NUMBER, PN_DEFAULT_GEM_GUARD_NUMBER );
        const SizeTSet gemGuardLevel = parameters.getSizeTSet( ParameterName::PN_GEM_GUARD_LEVEL, PN_DEFAULT_GEM_GUARD_LEVEL );

        const Rect mapRect = raw::RawLevel::mapRect();
        const Point mapCenter = mapRect.center();

        PointsField gemsField;
        {
            /// add left side
            Rect randPosArea( gemRegionWidth, gemRegionHeight );
            randPosArea.moveLeftTo( 2 );
            randPosArea.moveBottomTo( mapRect.max.y - 1 );
            gemsField.add( randPosArea );
        }
        {
            /// add right side
            Rect randPosArea( gemRegionWidth, gemRegionHeight );
            randPosArea.moveRightTo( mapRect.max.x - 1 );
            randPosArea.moveBottomTo( mapRect.max.y - 1 );
            gemsField.add( randPosArea );
        }

        const std::size_t slabFaces = gemFacesNum / gemSlabsNum;
        const std::size_t totalGemChambers = gemSlabsNum + gemTrapsNum;
        for ( std::size_t i=0; i<totalGemChambers; ++i ) {
            const std::size_t randomPosIndex = rng_randi( gemsField.size() );
            const Point gemCenter = gemsField.get( randomPosIndex );
            const bool leftSide = ( gemCenter.x < mapCenter.x );

            /// draw gem
            Rect gemRect( gemCenter, 5, 5 );
            level.setSlab( gemRect, Slab::S_ROCK );
            Point corridorDir;
            if ( leftSide ) {
                corridorDir = Point(  1, 0 );
                const PointList passage = findPassage( level, gemCenter, corridorDir );
                level.digCorridor( passage, Slab::S_EARTH );
            } else {
                corridorDir = Point( -1, 0 );
                const PointList passage = findPassage( level, gemCenter, corridorDir );
                level.digCorridor( passage, Slab::S_EARTH );
            }
            if ( i >= gemTrapsNum) {
                /// first add traps then actual gems

//                const std::size_t gemAP = level.addActionPoint( gemCenter, 1 );
//                script::ScriptCommand& actionSec = map.script.actionSection();
//                actionSec.REM( "gem reveal" );
//                actionSec.REM( std::to_string( gemAP ) + " -- gem position" );
//                actionSec.IF_ACTION_POINT( gemAP, Player::P_P0 );
//                actionSec.CHANGE_SLAB_TYPE( gemCenter, Slab::S_GEMS );
//                actionSec.ENDIF();

                script::ScriptCommand& mainSec = map.script.mainSection();
                const Point gemSide = gemCenter + corridorDir;
                mainSec.addEmptyLine();
                mainSec.REM( "- reveal hidden gem -" );
                mainSec.IF_SLAB_TYPE( gemSide, Slab::S_PATH );
                mainSec.CHANGE_SLAB_TYPE( gemCenter, Slab::S_GEMS );
                mainSec.ENDIF();

                std::size_t faces = slabFaces;
                if ( i <= gemFacesNum % gemSlabsNum ) {
                    ++faces;
                }
                drawGemFaces( level, gemCenter, faces, Slab::S_EARTH, corridorDir );
                level.setSlab( gemCenter, Slab::S_EARTH );
            }

            /// draw cavern with creatures
            Rect caveRect( gemCenter, 3, 5 );
            if ( leftSide ) {
                caveRect.move(  3, -1 );
            } else {
                caveRect.move( -3, -1 );
            }
            level.setCave( caveRect, Slab::S_PATH );
            const std::size_t guardNum   = gemGuardsNum.randomized();
            const std::size_t guardLevel = gemGuardLevel.randomized();
//            drawHeroTrap( level, caveRect.center(), guardLevel, guardNum );

            const std::size_t gemGuardAP = level.addActionPoint( caveRect.center(), 2 );
            drawHeroTrap( script, Player::P_GOOD, gemGuardAP, guardLevel, guardNum, "gem guard party" );

            gemRect.grow( 2 );
            gemsField.remove( gemRect );

            caveRect.grow( 2 );
            gemsField.remove( caveRect );
        }
    }

    void BaseLevelGenerator::generateLeftGoldVein( const std::size_t goldAmount, const std::size_t gemAmount ) {
        Level& level = map.level;
        const Rect mapRect = raw::RawLevel::mapRect();
        const std::size_t veinDimm = (std::size_t) sqrt( goldAmount ) * 1.5;
        Rect randPosArea( 21, 13 );
        randPosArea.moveLeftTo( 1 + veinDimm + 5 );
        randPosArea.moveBottomTo( mapRect.max.y - veinDimm );
        const std::size_t randomPosIndex = rng_randi( randPosArea.area() );
        const Point center = randPosArea.pointByIndex( randomPosIndex );
        const Rect veinRect( center, veinDimm, veinDimm );
        drawGoldVein( level, veinRect, goldAmount, gemAmount );
    }

    void BaseLevelGenerator::generateRightGoldVein( const std::size_t goldAmount, const std::size_t gemAmount ) {
        Level& level = map.level;
        const Rect mapRect = raw::RawLevel::mapRect();
        const std::size_t veinDimm = (std::size_t) sqrt( goldAmount ) * 1.5;
        Rect randPosArea( 25, 17 );
        randPosArea.moveRightTo( mapRect.max.x - veinDimm - 5 );
        randPosArea.moveBottomTo( mapRect.max.y - veinDimm );
        const std::size_t randomPosIndex = rng_randi( randPosArea.area() );
        const Point center = randPosArea.pointByIndex( randomPosIndex );
        const Rect veinRect( center, veinDimm, veinDimm );
        drawGoldVein( level, veinRect, goldAmount, gemAmount );
    }

} /* namespace dkmage */
