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
            MazeNode(): open(false) {
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

            std::size_t dimmX;
            std::size_t dimmY;
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

            std::size_t dimmensionX() const {
                if ( dimmX < 1 ) {
                    return 0;
                }
                return dimmX * 2 - 1;
            }

            std::size_t dimmensionY() const {
                if ( dimmY < 1 ) {
                    return 0;
                }
                return dimmY * 2 - 1;
            }

            bool state( const std::size_t x, const std::size_t y ) {
                if ( dimmX < 1 ) {
                    return false;
                }
                if ( dimmY < 1 ) {
                    return false;
                }
                const std::size_t dx2 = dimmX * 2 - 1;
                if ( x >= dx2 ) {
                    return false;
                }
                const std::size_t dy2 = dimmY * 2 - 1;
                if ( y >= dy2 ) {
                    return false;
                }

                const std::size_t cx = dx2 - x - 1;
                const std::size_t cy = dy2 - y - 1;

                if ( cx % 2 == 0 ) {
                    /// x -- node
                    const std::size_t xNode = cx / 2;
                    const std::size_t yNode = cy / 2;
                    const std::size_t nIndex = yNode * dimmX + xNode;
                    MazeNode* node = nodes[ nIndex ];
                    if ( cy % 2 == 0 ) {
                        /// y -- node
                        return node->open;
                    }
                    /// y -- edge
                    MazeEdge* edge = graph.getEdge( *node, Direction::D_NORTH );
                    return edge->open;
                }

                /// x -- edge
                if ( cy % 2 == 0 ) {
                    /// y -- node
                    const std::size_t xNode = cx / 2;
                    const std::size_t yNode = cy / 2;
                    const std::size_t nIndex = yNode * dimmX + xNode;
                    MazeNode* node = nodes[ nIndex ];
                    MazeEdge* edge = graph.getEdge( *node, Direction::D_EAST );
                    return edge->open;
                } else {
                    /// y -- edge -- on diagonal -- always false
                    return false;
                }

            }

            MazeNode* node( const std::size_t index ) {
                return nodes[ index ];
            }

            /// returns directions that are not open
            MazeNode* openNext( const MazeNode& node ) {
                std::vector< MazeNode* > closed;
                std::vector< Direction > dirs = graph.availableDirections( node );
                for ( const Direction dir: dirs ) {
                    MazeNode* target = graph.getItem( node, dir );
                    if ( target->open ) {
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
                nextNode->open = true;
                MazeEdge* edge = graph.getEdge( node, *nextNode );
                edge->open = true;
                return nextNode;
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

                generateGrid();

                std::stack< MazeNode* > list;
                {
                    const std::size_t center = dimmX / 2;
                    MazeNode* currNode = node( center );
                    list.push( currNode );
                }
                while ( list.empty() == false ) {
                    MazeNode* currNode = list.top();
                    currNode->open = true;
                    MazeNode* nextNode = openNext( *currNode );
                    if ( nextNode == nullptr ) {
                        /// no unvisited neighbour
                        list.pop();
                        continue ;
                    }
                    list.push( nextNode );
                }
            }

            std::string print() {
                std::stringstream stream;

                const std::size_t xDimm = dimmensionX();
                const std::size_t yDimm = dimmensionY();
                for ( std::size_t y=0; y<yDimm; ++y ) {
                    for ( std::size_t x=0; x<xDimm; ++x ) {
                        const bool val = state( x, y );
                        stream << val << " ";
                    }
                    stream << "\n";
                }

    //                for ( std::size_t y=0; y<dimm; ++y ) {
    //                    /// nodes row
    //                    for ( std::size_t x=0; x<dimm; ++x ) {
    //                        MazeNode* node = nodes[ y * dimm + x ];
    //                        stream << node->open << " ";
    //                        MazeEdge* edge = graph.getEdge( *node, Direction::D_EAST );
    //                        if ( edge != nullptr ) {
    //                            stream << edge->open << " ";
    //                        }
    //                    }
    //                    stream << "\n";
    //                    if ( y >= (dimm-1) ) {
    //                        continue ;
    //                    }
    //                    /// edges row
    //                    for ( std::size_t x=0; x<dimm; ++x ) {
    //                        MazeNode* node = nodes[ y * dimm + x ];
    //                        MazeEdge* edge = graph.getEdge( *node, Direction::D_NORTH );
    //                        stream << edge->open << "   ";
    //                    }
    //                    stream << "\n";
    //                }

                return stream.str();
            }


        protected:

            void generateGrid() {
                {
                    /// create first row
                    MazeNode* prevNode = graph.addItem();
                    nodes[0] = prevNode;
                    for ( std::size_t x=1; x<dimmX; ++x ) {
                        prevNode = graph.addItem( *prevNode, Direction::D_EAST );
                        nodes[x] = prevNode;
                    }
                }
                for ( std::size_t y=1; y<dimmY; ++y ) {
                    MazeNode* prevNode = graph.addItem();
                    MazeNode* prevRow  = nodes[ (y-1) * dimmX + 0 ];
                    graph.addEdge( *prevNode, *prevRow, Direction::D_SOUTH );
                    nodes[ y * dimmX + 0 ] = prevNode;
                    for ( std::size_t x=1; x<dimmX; ++x ) {
                        prevNode = graph.addItem( *prevNode, Direction::D_EAST );
                        MazeNode* prevRow  = nodes[ (y-1) * dimmX + x ];
                        graph.addEdge( *prevNode, *prevRow, Direction::D_SOUTH );
                        nodes[ y * dimmX + x ] = prevNode;
                    }
                }
            }

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
