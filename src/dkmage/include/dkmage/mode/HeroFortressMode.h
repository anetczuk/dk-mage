/*
 * HeroFortressMode.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_

#include "dkmage/BaseLevelGenerator.h"

#include "dkmage/spatial/Fortress.h"

#include "utils/ProbabilityMass.h"


namespace dkmage {
    namespace mode {

        /**
         *
         */
        class Fortress {
        public:

            adiktedpp::GameMap& gameMap;
            adiktedpp::Level& level;                    //TODO: remove (unnecessary)
            ParametersMap& parameters;
            spatial::FortressDungeon fortress;

            utils::ProbabilityMass< spatial::FortressRoomType > roomProbability;


            Fortress( adiktedpp::GameMap& gameMap, ParametersMap& parameters );

            bool generate();

            std::vector< const spatial::FortressRoom* > prepareCorridors( const std::vector< const spatial::FortressRoom* >& startRooms, const std::size_t roomsNum, const bool allowBranches );

            void prepareExitRooms( const std::vector< const spatial::FortressRoom* >& startRooms );

            void prepareSecondaryPass();

            void cutBlindCorridors();

            void cutInvalidExits();

            bool generateLake( const utils::Rect& lakeLimit );

    //            std::vector< const spatial::FortressRoom* > prepareRoom( const spatial::FortressRoomType roomType, const spatial::FortressRoom* startItem, const bool allowBranches );

            bool prepareBridges( const std::vector< const spatial::FortressRoom* >& exitRooms );

            void prepareCorridorTraps();

            /// find shortest bridge in four directions
            utils::PointList findBridge( const utils::Point startPoint );

            utils::PointList findBridge( const utils::Point startPoint, const utils::Point bridgeDirection );

            bool isBridgePossible( const utils::Point startPoint ) const;

            bool findFortified( const utils::Point startPoint, const utils::Point bridgeDirection ) const;

        };


        /// ========================================================================================


        /**
         *
         */
        class HeroFortressMode: public BaseLevelGenerator {
        public:


        protected:

            bool generate() override;

            bool check() override;

            void generateCaves( const std::size_t cavesNum );

            void preparePlayerDungeon();

            void prepareScript();

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_ */
