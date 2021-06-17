/*
 * Maze.cpp
 *
 */

#include "dkmage/generator/Maze.h"


namespace dkmage {
    namespace generator {

        void Maze::generate( const std::size_t baseDimmension ) {
            graph.resize( baseDimmension );
            graph.generate();
//            LOG() << "graph:\n" << graph.print();
        }

        void Maze::generate( const std::size_t dimmX, const std::size_t dimmY ) {
            graph.resize( dimmX, dimmY );
            graph.generate();
        }

    } /* namespace generator */
} /* namespace dkmage */
