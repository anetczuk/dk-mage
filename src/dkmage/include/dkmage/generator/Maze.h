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
            std::vector< bool > grid;


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

            std::size_t dimmensionX() const {
                if ( dimmX < 1 ) {
                    return 0;
                }
                return dimmX * 2 + 1;
            }

            std::size_t dimmensionY() const {
                if ( dimmY < 1 ) {
                    return 0;
                }
                return dimmY * 2 + 1;
            }

            bool state( const std::size_t x, const std::size_t y ) {
                const std::size_t xDimm = dimmensionX();
                if ( x >= xDimm ) {
                    return false;
                }
                const std::size_t yDimm = dimmensionY();
                if ( y >= yDimm ) {
                    return false;
                }
                const std::size_t cy = yDimm - y - 1;
                return grid[ cy * xDimm + x ];
            }

            MazeNode* node( const std::size_t index ) {
                return nodes[ index ];
            }

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
                    MazeNode* currNode = node( center );
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

                prepareGrid();
            }

            /// returns directions that are not open
            MazeNode* openNext( const MazeNode& node ) {
                std::vector< MazeNode* > closed;
                std::vector< Direction > dirs = graph.availableDirections( node );
                for ( const Direction dir: dirs ) {
                    MazeNode* target = graph.getItem( node, dir );
                    if ( target->visited ) {
                        continue ;
                    }
                    closed.push_back( target );
                }
                const std::size_t cSize = closed.size();
                if ( cSize < 1 ) {
                    return nullptr;
                }
                const std::size_t nextIndex = rand() % cSize;
                MazeNode* nextNode = closed[ nextIndex ];
                nextNode->setOpen();
                MazeEdge* edge = graph.getEdge( node, *nextNode );
                edge->open = true;
                return nextNode;
            }

            std::string print();


        protected:

            void generateGraph();

            void prepareGrid();

            std::size_t innerDimmX() const {
                if ( dimmX < 1 ) {
                    return 0;
                }
                return dimmX * 2 - 1;
            }

            std::size_t innerDimmY() const {
                if ( dimmY < 1 ) {
                    return 0;
                }
                return dimmY * 2 - 1;
            }

            bool innerState( const std::size_t x, const std::size_t y );

        };


        /**
         *
         */
        class Maze {

            MazeGraph graph;


        public:

            Maze() {
            }

            std::size_t dimmensionX() const {
                return graph.dimmensionX();
            }

            std::size_t dimmensionY() const {
                return graph.dimmensionY();
            }

            bool state( const std::size_t x, const std::size_t y ) {
                return graph.state( x, y );
            }

            void generate( const std::size_t baseDimmension );

            void generate( const std::size_t dimmX, const std::size_t dimmY );

        };

    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_MAZE_H_ */
