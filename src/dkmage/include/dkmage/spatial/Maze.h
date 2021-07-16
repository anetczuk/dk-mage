/*
 * Maze.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_MAZE_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_MAZE_H_

#include "dkmage/spatial/Spatialtem.h"
#include "dkmage/spatial/DungeonGraph.h"

#include "utils/Rect.h"

#include <stack>
#include <set>


namespace dkmage {
    namespace spatial {

        /**
         *
         */
        class MazeNode {
        public:

            bool visited;                   /// is node processed by maze generator?
            int distanceToEntry;

            MazeNode(): visited(false), distanceToEntry(-1) {
            }

            void setVisited() {
                visited = true;
            }

        };

        /**
         *
         */
        class MazeEdge {
        public:

            bool open;                          /// is there passage between chambers (wall removed)?


            MazeEdge(): open(false) {
            }
        };

        inline std::ostream& operator<<( std::ostream& os, const MazeEdge& data ) {
            os << data.open;
            return os;
        }


        /**
         * Implementation based on:
         * https://en.wikipedia.org/wiki/Maze_generation_algorithm
         */
        class MazeGraph {
        public:

            std::size_t dimmX;                              /// number of nodes in 'graph' object
            std::size_t dimmY;                              /// number of nodes in 'graph' object
            DungeonGraph< MazeNode, MazeEdge > graph;
            std::vector< MazeNode* > nodes;


            MazeGraph(): dimmX( 0 ), dimmY( 0 ) {
            }

            MazeGraph( const std::size_t dimmension ): dimmX( dimmension ), dimmY( dimmension ) {
                nodes.resize( dimmX * dimmY );
            }

            void resize( const std::size_t dimmension ) {
                dimmX = dimmension;
                dimmY = dimmension;
                nodes.resize( dimmX * dimmY );
            }

            void resize( const std::size_t dimmensionX, const std::size_t dimmensionY ) {
                dimmX = dimmensionX;
                dimmY = dimmensionY;
                nodes.resize( dimmX * dimmY );
            }

            /// counts nodes and edges
            std::size_t dimmensionX() const {
                if ( dimmX < 1 ) {
                    return 0;
                }
                return dimmX * 2 - 1;
            }

            /// counts nodes and edges
            std::size_t dimmensionY() const {
                if ( dimmY < 1 ) {
                    return 0;
                }
                return dimmY * 2 - 1;
            }

            bool state( const std::size_t x, const std::size_t y );

            /// generate maze graph (nodes and links)
            void generate();

            void calculateDistances( const std::size_t nx, const std::size_t ny );

            std::set< std::size_t > getDeadEnds();

            std::size_t getFurthest();

            std::size_t getFurthest( const std::size_t nx, const std::size_t ny );

            std::size_t maxDistance() const;

            int nodeDistance( const std::size_t nx, const std::size_t ny ) const;

            std::string print();


        protected:

            /// prepare nodes grid and linking edges
            void generateGraph();

            MazeNode* openNext( const MazeNode& node );

            std::vector< Direction > openDirections( const MazeNode& node );

        };


        /**
         *
         */
        class Maze: public Spatialtem {
        public:

            enum class PathType {
                PT_ROCK,
                PT_EARTH,
                PT_LAVA
            };


        private:

            MazeGraph graph;
            std::vector< Maze::PathType > grid;
            utils::Point minPoint;


        public:

            std::size_t corridorSize;


            Maze(): corridorSize(1) {
            }

            std::size_t corridorsNum() const {
                return graph.dimmX * graph.dimmY;
            }

            std::size_t nodesX() const {
                return graph.dimmX;
            }

            std::size_t nodesY() const {
                return graph.dimmY;
            }

            std::size_t dimmensionX() const {
                if ( graph.dimmX < 1 ) {
                    return 0;
                }
                return (corridorSize + 1) * graph.dimmX + 1;
            }

            std::size_t dimmensionY() const {
                if ( graph.dimmY < 1 ) {
                    return 0;
                }
                return (corridorSize + 1) * graph.dimmY + 1;
            }

            /// size of maze
            utils::Rect boundingBox() const override;

            void move( const int offsetX, const int offsetY ) override;

            PathType state( const std::size_t x, const std::size_t y ) const;

            void generate( const std::size_t baseDimmension );

            void generate( const std::size_t dimmX, const std::size_t dimmY );

            utils::Rect nodeRect( const std::size_t nIndex ) const;

            utils::Rect nodeRect( const std::size_t nx, const std::size_t ny ) const;

            std::set< std::size_t > getDeadEnds();

            std::size_t getFurthestIndex();

            utils::Rect getFurthest();

            std::size_t getFurthestIndex( const std::size_t nx, const std::size_t ny );

            utils::Rect getFurthest( const std::size_t nx, const std::size_t ny );

            int maxDistance() const;

            int nodeDistance( const std::size_t nx, const std::size_t ny ) const;


        protected:

            void prepareGrid();

            void openPassage( const std::size_t sX, const std::size_t sY );

            void setWall( const std::size_t sX, const std::size_t sY, const PathType value );

            void open( const std::size_t gx, const std::size_t gy, const std::size_t xRange, const std::size_t yRange );

            void setGridValue( const std::size_t gx, const std::size_t gy, const std::size_t xRange, const std::size_t yRange, const PathType value );

        };

    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_MAZE_H_ */
