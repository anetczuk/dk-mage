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

            adiktedpp::Level& level;
            ParametersMap& parameters;
            spatial::FortressDungeon fortress;

            utils::ProbabilityMass< spatial::FortressRoomType > roomProbability;


            Fortress( adiktedpp::Level& level, ParametersMap& parameters );

            bool generate();

            std::vector< const spatial::FortressRoom* > prepareCorridors( const std::vector< const spatial::FortressRoom* >& startRooms, const std::size_t roomsNum, const bool allowBranches );

            std::vector< const spatial::FortressRoom* > prepareExitRooms( const std::vector< const spatial::FortressRoom* >& startRooms );

            void prepareSecondaryPass();

            void cutBlindCorridors();

            bool generateLake( const utils::Rect& lakeLimit );

    //            std::vector< const spatial::FortressRoom* > prepareRoom( const spatial::FortressRoomType roomType, const spatial::FortressRoom* startItem, const bool allowBranches );

            bool prepareBridges( const std::vector< const spatial::FortressRoom* >& entranceRooms );

            /// find shortest bridge in four directions
            utils::PointList findBridge( const utils::Point startPoint );

            utils::PointList findBridge( const utils::Point startPoint, const utils::Point bridgeDirection );

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
