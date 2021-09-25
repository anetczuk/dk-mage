/*
 * Maze.cpp
 *
 */

#include "dkmage/spatial/Maze.h"

#include "utils/Rand.h"


using namespace utils;


namespace dkmage {
    namespace spatial {

        bool MazeGraph::state( const std::size_t x, const std::size_t y ) {
            if ( dimmX < 1 ) {
                /// empty graph
                return false;
            }
            if ( dimmY < 1 ) {
                /// empty graph
                return false;
            }
            const std::size_t dx2 = dimmensionX();
            if ( x >= dx2 ) {
                /// coord outside graph
                return false;
            }
            const std::size_t dy2 = dimmensionY();
            if ( y >= dy2 ) {
                /// coord outside graph
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
                    return true;
                }
                /// y -- edge
                MazeEdge* edge = graph.getEdge( *node, Direction::D_SOUTH );
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
            }

            /// y -- edge -- on diagonal -- always false
            return false;
        }

        void MazeGraph::generate() {
            nodes.clear();
            nodes.resize( dimmX * dimmY );

            if ( dimmX < 1 ) {
                return ;
            }
            if ( dimmY < 1 ) {
                return ;
            }

            /// prepare nodes grid and linking edges
            generateGraph();

            /// open passages (remove walls)
            std::stack< MazeNode* > list;
            {
                const std::size_t center = dimmX / 2;
                MazeNode* currNode = nodes[ center ];
                list.push( currNode );
            }
            while ( list.empty() == false ) {
                MazeNode* currNode = list.top();
//                    if ( currNode->visited == false ) {
//                        /// randomize type
//                    }
                currNode->setVisited();
                MazeNode* nextNode = openNext( *currNode );
                if ( nextNode == nullptr ) {
                    /// no unvisited neighbour
                    list.pop();
                    continue ;
                }
                list.push( nextNode );
            }
        }

        void MazeGraph::calculateDistances( const std::size_t nx, const std::size_t ny ) {
            if ( nx >= dimmX ) {
                return ;
            }
            if ( ny >= dimmY ) {
                return ;
            }

            std::stack< MazeNode* > list;
            {
                const std::size_t start = ny * dimmX + nx;
                MazeNode* currNode = nodes[ start ];
                currNode->distanceToEntry = 0;
                list.push( currNode );
            }
            while ( list.empty() == false ) {
                MazeNode* currNode = list.top();
                list.pop();
                const int nextDistance = currNode->distanceToEntry + 1;

                std::vector< Direction > dirs = graph.linkDirections( *currNode );
                for ( const Direction dir: dirs ) {
                    MazeEdge* edge = graph.getEdge( *currNode, dir );
                    if ( edge->open == false ) {
                        continue ;
                    }
                    MazeNode* target = graph.getItem( *currNode, dir );
                    if ( target->distanceToEntry >= 0 && target->distanceToEntry <= nextDistance ) {
                        continue ;
                    }
                    target->distanceToEntry = nextDistance;
                    list.push( target );
                }
            }
        }

        std::set< std::size_t > MazeGraph::getDeadEnds() {
            std::set< std::size_t > ret;

            for ( std::size_t i=0; i<nodes.size(); ++i ) {
                MazeNode* item = nodes[i];
                std::vector< Direction > dirs = openDirections( *item );
                if ( dirs.size() != 1 ) {
                    continue ;
                }
                /// dead-end
                ret.insert( i );
            }

            return ret;
        }

        std::size_t MazeGraph::getFurthest() {
            int furthestDistance   = -1;
            std::size_t furthestIndex = (std::size_t) -1;

            for ( std::size_t i=0; i<nodes.size(); ++i ) {
                MazeNode* item = nodes[i];
                const int currDistance = item->distanceToEntry;
                if ( currDistance > furthestDistance ) {
                    furthestDistance = currDistance;
                    furthestIndex = i;
                }
            }

            return furthestIndex;
        }

        std::size_t MazeGraph::getFurthest( const std::size_t nx, const std::size_t ny ) {
            if ( nx >= dimmX ) {
                return -1;
            }
            if ( ny >= dimmY ) {
                return -1;
            }

            int furthestDistance   = -1;
            MazeNode* furthestNode = nullptr;

            std::stack< MazeNode* > list;
            {
                const std::size_t start = ny * dimmX + nx;
                MazeNode* currNode = nodes[ start ];
                list.push( currNode );
            }
            while ( list.empty() == false ) {
                MazeNode* currNode = list.top();
                list.pop();
                const int currDistance = currNode->distanceToEntry;
                if ( currDistance > furthestDistance ) {
                    furthestDistance = currDistance;
                    furthestNode = currNode;
                }

                std::vector< Direction > dirs = graph.linkDirections( *currNode );
                for ( const Direction dir: dirs ) {
                    MazeEdge* edge = graph.getEdge( *currNode, dir );
                    if ( edge->open == false ) {
                        continue ;
                    }
                    MazeNode* target = graph.getItem( *currNode, dir );
                    if ( target->distanceToEntry <= currDistance ) {
                        continue ;
                    }
                    list.push( target );
                }
            }

            auto it = std::find( nodes.begin(), nodes.end(), furthestNode );
            if ( it == nodes.end() ) {
                LOG_INFO() << "invalid state";
                return -1;
            }

            const int index = std::distance( nodes.begin(), it );
            return (std::size_t) index;
        }

        std::size_t MazeGraph::maxDistance() const {
            int maxDist = -1;
            for ( const MazeNode* item: nodes ) {
                const int currDistance = item->distanceToEntry;
                maxDist = std::max( maxDist, currDistance );
            }
            return maxDist;
        }

        int MazeGraph::nodeDistance( const std::size_t nx, const std::size_t ny ) const {
            if ( nx >= dimmX ) {
                return -1;
            }
            if ( ny >= dimmY ) {
                return -1;
            }

            const std::size_t nIndex = ny * dimmX + nx;
            const MazeNode* node = nodes[ nIndex ];
            if ( node == nullptr ) {
                return -1;
            }
            return node->distanceToEntry;
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
                MazeNode* newNode = graph.addItem();
                MazeNode* prevRow  = nodes[ (y-1) * dimmX + 0 ];
                graph.addEdge( *prevRow, *newNode, Direction::D_SOUTH );
                nodes[ y * dimmX + 0 ] = newNode;
                for ( std::size_t x=1; x<dimmX; ++x ) {
                    newNode = graph.addItem( *newNode, Direction::D_EAST );
                    MazeNode* prevRow  = nodes[ (y-1) * dimmX + x ];
                    graph.addEdge( *prevRow, *newNode, Direction::D_SOUTH );
                    nodes[ y * dimmX + x ] = newNode;
                }
            }
        }

        MazeNode* MazeGraph::openNext( const MazeNode& node ) {
            std::vector< MazeNode* > closed;
            std::vector< Direction > dirs = graph.linkDirections( node );
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
            const std::size_t nextIndex = rng_randi( cSize );
            MazeNode* nextNode = closed[ nextIndex ];
            MazeEdge* edge = graph.getEdge( node, *nextNode );
            edge->open = true;
            return nextNode;
        }

        std::vector< Direction > MazeGraph::openDirections( const MazeNode& node ) {
            std::vector< Direction > open;
            std::vector< Direction > dirs = graph.linkDirections( node );
            for ( const Direction dir: dirs ) {
                const MazeEdge* edge = graph.getEdge( node, dir );
                if ( edge == nullptr ) {
                    continue ;
                }
                if ( edge->open == false ) {
                    continue ;
                }
                open.push_back( dir );
            }
            return open;
        }


        /// =====================================================================


        Rect Maze::boundingBox() const {
            const std::size_t rSize = graph.nodes.size();
            if ( rSize < 1 ) {
                return Rect( minPoint, 0, 0 );
            }

            Rect minMax = nodeRect( 0, 0 );
            for ( std::size_t y=0; y<graph.dimmY; ++y ) {
                for ( std::size_t x=0; x<graph.dimmX; ++x ) {
                    const Rect nRect = nodeRect( x, y );
                    minMax.expand( nRect );
                }
            }

            minMax.grow( 1 );           /// border of maze
            return minMax;
        }

        void Maze::move( const int offsetX, const int offsetY ) {
            minPoint += Point( offsetX, offsetY );
        }

        Maze::PathType Maze::state( const std::size_t x, const std::size_t y ) const {
            const std::size_t xDimm = dimmensionX();
            if ( x >= xDimm ) {
                return Maze::PathType::PT_ROCK;
            }
            const std::size_t yDimm = dimmensionY();
            if ( y >= yDimm ) {
                return Maze::PathType::PT_ROCK;
            }
            const std::size_t index = y * xDimm + x;
            if ( index >= grid.size() ) {
                return Maze::PathType::PT_ROCK;
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
            graph.calculateDistances( graph.dimmX / 2, 0 );
            graph.calculateDistances( graph.dimmX / 2, graph.dimmY - 1 );
        }

        Rect Maze::nodeRect( const std::size_t nIndex ) const {
            const std::size_t nx = nIndex % graph.dimmX;
            const std::size_t ny = nIndex / graph.dimmX;
            return nodeRect( nx, ny );
        }

        Rect Maze::nodeRect( const std::size_t nx, const std::size_t ny ) const {
            const std::size_t cx = nx * (corridorSize + 1) + 1;
            const std::size_t cy = ny * (corridorSize + 1) + 1;
            const Point pointMin( cx, cy );
            const Point pointMax = pointMin + Point( corridorSize - 1, corridorSize - 1 );
            Rect rect( pointMin, pointMax );
            rect.move( minPoint );
            return rect;
        }

        std::set< std::size_t > Maze::getDeadEnds() {
            return graph.getDeadEnds();
        }

        std::size_t Maze::getFurthestIndex() {
            return graph.getFurthest();
        }

        Rect Maze::getFurthest() {
            const std::size_t foundIndex = graph.getFurthest();
            if ( foundIndex == (std::size_t) -1 ) {
                /// not found
                return Rect();
            }
            const std::size_t fx = foundIndex % graph.dimmX;        /// node coords
            const std::size_t fy = foundIndex / graph.dimmX;        /// node coords
            return nodeRect( fx, fy );
        }

        std::size_t Maze::getFurthestIndex( const std::size_t nx, const std::size_t ny ) {
            return graph.getFurthest( nx, ny );
        }

        Rect Maze::getFurthest( const std::size_t nx, const std::size_t ny ) {
            const std::size_t foundIndex = graph.getFurthest( nx, ny );
            if ( foundIndex == (std::size_t) -1 ) {
                /// not found
                return Rect();
            }
            const std::size_t fx = foundIndex % graph.dimmX;        /// node coords
            const std::size_t fy = foundIndex / graph.dimmX;        /// node coords
            return nodeRect( fx, fy );
        }

        int Maze::maxDistance() const {
            return graph.maxDistance();
        }

        int Maze::nodeDistance( const std::size_t nx, const std::size_t ny ) const {
            return graph.nodeDistance( nx, ny );
        }

        void Maze::prepareGrid() {
            const std::size_t xDimm = dimmensionX();
            const std::size_t yDimm = dimmensionY();
            grid.resize( xDimm * yDimm, PathType::PT_ROCK );

            const std::size_t graphDimmX = graph.dimmensionX();
            const std::size_t graphDimmY = graph.dimmensionY();

            for ( std::size_t y=0; y<graphDimmY; ++y ) {
                for ( std::size_t x=0; x<graphDimmX; ++x ) {
                    if ( graph.state( x, y ) ) {
                        openPassage( x, y );
                        continue ;
                    }

                    /// generate lava or rock
                    const std::size_t lavaWall = rng_randi( 2 );
                    if ( lavaWall == 1 ) {
                        setWall( x, y, PathType::PT_LAVA );
                    }

                    // otherwise rock by default
                }
            }

            /// open entrances
            const std::size_t enterIndex = xDimm / 2;
            open( enterIndex - 1 - corridorSize/2, -1, corridorSize, 1 );
            open( enterIndex - 1 - corridorSize/2, yDimm - 2, corridorSize, 1 );
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

        void Maze::setWall( const std::size_t sX, const std::size_t sY, const PathType value ) {
            const std::size_t gx = sX / 2 * (corridorSize + 1);
            const std::size_t gy = sY / 2 * (corridorSize + 1);
            if ( sX % 2 == 0 ) {
                if ( sY % 2 == 0 ) {
                    /// corridor -- node -- do nothing
                } else {
                    /// vertical -- north edge
                    setGridValue( gx, gy + corridorSize, corridorSize, 1, value );
                }
            } else {
                if ( sY % 2 == 0 ) {
                    /// horizontal -- east edge
                    setGridValue( gx + corridorSize, gy, 1, corridorSize, value );
                } else {
                    /// diagonal -- do nothing
                }
            }
        }

        void Maze::open( const std::size_t gx, const std::size_t gy, const std::size_t xRange, const std::size_t yRange ) {
            setGridValue( gx, gy, xRange, yRange, PathType::PT_EARTH );
        }

        void Maze::setGridValue( const std::size_t gx, const std::size_t gy, const std::size_t xRange, const std::size_t yRange, const PathType value ) {
            const std::size_t xDimm = dimmensionX();
            for ( std::size_t dy=0; dy<yRange; ++dy) {
                for ( std::size_t dx=0; dx<xRange; ++dx) {
                    const std::size_t cx = gx + dx;
                    const std::size_t cy = gy + dy;
                    const std::size_t gIndex = (cy + 1) * xDimm + (cx + 1);
//                    if ( gIndex >= grid.size() ) {
//                        continue ;
//                    }
                    grid[ gIndex ] = value;
                }
            }
        }

    } /* namespace spatial */
} /* namespace dkmage */
