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


        /// =====================================================================


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

            const std::size_t dx2 = graph.dimmensionX();
            const std::size_t dy2 = graph.dimmensionY();

            for ( std::size_t y=0; y<dy2; ++y ) {
                for ( std::size_t x=0; x<dx2; ++x ) {
                    const std::size_t gIndex = (y + 1) * xDimm + (x + 1);
                    grid[ gIndex ] = graph.state( x, y );
                }
            }

            const std::size_t enterNodeIndex = graph.dimmX / 2;
            {
                const std::size_t centerState = enterNodeIndex * 2 + 1;
                grid[ centerState ] = true;
            }
            {
                const std::size_t centerState = (yDimm - 1) * xDimm + enterNodeIndex * 2 + 1;
                grid[ centerState ] = true;
            }
        }

    } /* namespace generator */
} /* namespace dkmage */
