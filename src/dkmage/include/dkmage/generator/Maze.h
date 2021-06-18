/*
 * Maze.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_GENERATOR_MAZE_H_
#define DKMAGE_INCLUDE_DKMAGE_GENERATOR_MAZE_H_

#include "dkmage/generator/DungeonGraph.h"

#include <stack>


namespace dkmage {
    namespace generator {

        /**
         *
         */
        class MazeNode {
        public:
            bool open;
            bool visited;

            MazeNode(): open(false), visited(false) {
            }

            void setOpen() {
                open = true;
                visited = true;
            }

            void setClosed() {
                open = false;
                visited = true;
            }

        };

        /**
         *
         */
        class MazeEdge {
        public:
            bool open;
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

            void generate() {
                nodes.clear();
                nodes.resize( dimmX * dimmY );

                if ( dimmX < 1 ) {
                    return ;
                }
                if ( dimmY < 1 ) {
                    return ;
                }

                generateGraph();

                std::stack< MazeNode* > list;
                {
                    const std::size_t center = dimmX / 2;
                    MazeNode* currNode = nodes[ center ];
                    list.push( currNode );
                }
                while ( list.empty() == false ) {
                    MazeNode* currNode = list.top();
                    currNode->setOpen();
                    MazeNode* nextNode = openNext( *currNode );
                    if ( nextNode == nullptr ) {
                        /// no unvisited neighbour
                        list.pop();
                        continue ;
                    }
                    list.push( nextNode );
                }
            }

            std::string print();


        protected:

            void generateGraph();

            /// returns directions that are not open
            MazeNode* openNext( const MazeNode& node );

        };


        /**
         *
         */
        class Maze {

            MazeGraph graph;
            std::vector< bool > grid;


        public:

            std::size_t corridorSize;


            Maze(): corridorSize(1) {
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

            bool state( const std::size_t x, const std::size_t y );

            void generate( const std::size_t baseDimmension );

            void generate( const std::size_t dimmX, const std::size_t dimmY );


        protected:

            void prepareGrid();

            void openPassage( const std::size_t sX, const std::size_t sY );

            void open( const std::size_t gx, const std::size_t gy, const std::size_t xRange, const std::size_t yRange );

        };

    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_MAZE_H_ */
