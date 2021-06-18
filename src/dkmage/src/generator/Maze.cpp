/*
 * Maze.cpp
 *
 */

#include "dkmage/generator/Maze.h"


namespace dkmage {
    namespace generator {

        bool MazeGraph::state( const std::size_t x, const std::size_t y ) {
            if ( dimmX < 1 ) {
                return false;
            }
            if ( dimmY < 1 ) {
                return false;
            }
            const std::size_t dx2 = dimmensionX();
            if ( x >= dx2 ) {
                return false;
            }
            const std::size_t dy2 = dimmensionY();
            if ( y >= dy2 ) {
                return false;
            }

            if ( x % 2 == 0 ) {
                /// x -- node
                const std::size_t xNode = x / 2;
                const std::size_t yNode = y / 2;
                const std::size_t nIndex = yNode * dimmX + xNode;
                MazeNode* node = nodes[ nIndex ];
                if ( y % 2 == 0 ) {
                    /// y -- node
                    return node->open;
                }
                /// y -- edge
                MazeEdge* edge = graph.getEdge( *node, Direction::D_NORTH );
                return edge->open;
            }

            /// x -- edge
            if ( y % 2 == 0 ) {
                /// y -- node
                const std::size_t xNode = x / 2;
                const std::size_t yNode = y / 2;
                const std::size_t nIndex = yNode * dimmX + xNode;
                MazeNode* node = nodes[ nIndex ];
                MazeEdge* edge = graph.getEdge( *node, Direction::D_EAST );
                return edge->open;
            } else {
                /// y -- edge -- on diagonal -- always false
                return false;
            }
        }

        std::string MazeGraph::print() {
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

        void MazeGraph::generateGraph() {
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

        /// returns directions that are not open
        MazeNode* MazeGraph::openNext( const MazeNode& node ) {
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


        /// =====================================================================


        bool Maze::state( const std::size_t x, const std::size_t y ) {
            const std::size_t xDimm = dimmensionX();
            if ( x >= xDimm ) {
                return false;
            }
            const std::size_t yDimm = dimmensionY();
            if ( y >= yDimm ) {
                return false;
            }
            const std::size_t cy = yDimm - y - 1;
            const std::size_t index = cy * xDimm + x;
            if ( index >= grid.size() ) {
                return false;
            }
            return grid[ index ];
        }

        void Maze::generate( const std::size_t baseDimmension ) {
            graph.resize( baseDimmension );
            graph.generate();
            prepareGrid();
        }

        void Maze::generate( const std::size_t dimmX, const std::size_t dimmY ) {
            graph.resize( dimmX, dimmY );
            graph.generate();
            prepareGrid();
        }

        void Maze::prepareGrid() {
            const std::size_t xDimm = dimmensionX();
            const std::size_t yDimm = dimmensionY();
            grid.resize( xDimm * yDimm, false );

            const std::size_t graphDimmX = graph.dimmensionX();
            const std::size_t graphDimmY = graph.dimmensionY();

            for ( std::size_t y=0; y<graphDimmY; ++y ) {
                for ( std::size_t x=0; x<graphDimmX; ++x ) {
                    if ( graph.state( x, y ) == false ) {
                        continue ;
                    }
                    openPassage( x, y );
                }
            }

            /// open entrances
            const std::size_t enterIndex = xDimm / 2;
            grid[ enterIndex ] = true;
            {
                const std::size_t centerState = (yDimm - 1) * xDimm + enterIndex;
                grid[ centerState ] = true;
            }
        }

        void Maze::openPassage( const std::size_t sX, const std::size_t sY ) {
            const std::size_t gx = sX / 2 * (corridorSize + 1);
            const std::size_t gy = sY / 2 * (corridorSize + 1);
            if ( sX % 2 == 0 ) {
                if ( sY % 2 == 0 ) {
                    /// corridor -- node
                    open( gx, gy, corridorSize, corridorSize );
                } else {
                    /// vertical -- north edge
                    open( gx, gy + corridorSize, corridorSize, 1 );
                }
            } else {
                if ( sY % 2 == 0 ) {
                    /// horizontal -- east edge
                    open( gx + corridorSize, gy, 1, corridorSize );
                } else {
                    /// diagonal -- do nothing
                }
            }
        }

        void Maze::open( const std::size_t gx, const std::size_t gy, const std::size_t xRange, const std::size_t yRange ) {
            const std::size_t xDimm = dimmensionX();
            for ( std::size_t dy=0; dy<yRange; ++dy) {
                for ( std::size_t dx=0; dx<xRange; ++dx) {
                    const std::size_t cx = gx + dx;
                    const std::size_t cy = gy + dy;
                    const std::size_t gIndex = (cy + 1) * xDimm + (cx + 1);
//                    if ( gIndex >= grid.size() ) {
//                        continue ;
//                    }
                    grid[ gIndex ] = true;
                }
            }
        }

    } /* namespace generator */
} /* namespace dkmage */
