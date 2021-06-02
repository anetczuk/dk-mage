/*
 * DungeonGraph.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEONGRAPH_H_
#define DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEONGRAPH_H_

#include "utils/Log.h"

#include <lemon/list_graph.h>

#include <sstream>


namespace dkmage {
    namespace generator {

        /**
         *
         */
        enum class Direction {
            D_NORTH,
            D_SOUTH,
            D_EAST,
            D_WEST
        };

        inline std::ostream& operator<<( std::ostream& os, const Direction& data ) {
            switch( data ) {
            case Direction::D_NORTH: {
                os << "D_NORTH";
                return os;
            }
            case Direction::D_SOUTH: {
                os << "D_SOUTH";
                return os;
            }
            case Direction::D_EAST: {
                os << "D_EAST";
                return os;
            }
            case Direction::D_WEST: {
                os << "D_WEST";
                return os;
            }
            }
            os << "UNKNOWN[" << (int) data << "]";
            return os;
        }

        const std::vector<Direction>& DirectionValues();

        inline Direction opposite( const Direction direction ) {
            switch( direction ) {
            case Direction::D_NORTH: return Direction::D_SOUTH;
            case Direction::D_SOUTH: return Direction::D_NORTH;
            case Direction::D_EAST:  return Direction::D_WEST;
            case Direction::D_WEST:  return Direction::D_EAST;
            }
            return Direction::D_NORTH;
        }


        /// ==================================================================


        /**
         * Implementation of rule of five.
         */
        template <typename T>
        struct GraphData {
            std::unique_ptr<T> data;

            GraphData(): data( new T() ) {
            }

            /// copy constructor
            GraphData( const GraphData& dataPtr ): GraphData() {
                /// swap have to be performed in case of proper execution
                GraphData* dataCast = const_cast<GraphData*>( &dataPtr );
                data.swap( dataCast->data );

    //                T* dataRaw = get();
    //                *dataRaw = dataPtr.ref();
            }

            /// move constructor
            GraphData( GraphData&& dataPtr ) noexcept: GraphData() {
                data.swap( dataPtr.data );
            }

            ~GraphData() {
            }

            /// copy assignment
            GraphData& operator=( const GraphData& dataPtr ) {
                *this = GraphData( dataPtr );
                return *this;
            }

            /// move assignment
            GraphData& operator=( GraphData&& dataPtr ) noexcept {
                *this = GraphData( dataPtr );
                return *this;
            }

            bool operator==( const T* ptr ) const {
                return ( get() == ptr );
            }

            T& operator*() {
                return *data;
            }

            void reset( T* ptr ) {
                data.reset( ptr );
            }

            const T& ref() const {
                return *data.get();
            }

            T& ref() {
                return *data.get();
            }

            const T* get() const {
                return data.get();
            }

            T* get() {
                return data.get();
            }

        };


        /**
         *
         */
        template <typename TItem>
        class DungeonGraph {
        public:

            typedef GraphData<TItem> DataPtr;

            lemon::ListDigraph graph;
            lemon::ListDigraph::ArcMap< Direction > edgeDirection;
            lemon::ListDigraph::NodeMap< DataPtr > items;

            lemon::ListDigraph::Node first;


            DungeonGraph(): graph(), edgeDirection( graph ), items( graph ) {
                first = graph.addNode();
            }

            std::size_t size() const {
                return (std::size_t) lemon::countNodes( graph );
            }

            TItem& firstItem() {
                return getItem( first );
            }

            TItem* itemByindex( const std::size_t index ) {
                lemon::ListDigraph::Node node = graph.nodeFromId( index );
                if ( graph.valid( node ) == false ) {
                    return nullptr;
                }
                TItem& item = getItem( node );
                return &item;
            }

            std::vector< TItem* > itemsList() {
                std::vector< TItem* > ret;
                ret.reserve( size() );
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    TItem& nItem = getItem( n );
                    ret.push_back( &nItem );
                }
                return ret;
            }

            std::size_t itemId( const TItem& item ) {
                lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    return (std::size_t) -1;
                }
                return graph.id( itemNode );
            }

            std::vector< Direction > freeDirections( const TItem& item ) {
                const lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    return std::vector< Direction >();
                }
                std::vector< Direction > ret;
                const std::vector<Direction>& directions = DirectionValues();
                for ( const Direction dir: directions ) {
                    if ( isFree( itemNode, dir ) ) {
                        ret.push_back( dir );
                    }

                }
                return ret;
            }

            bool isFree( const lemon::ListDigraph::Node& itemNode, const Direction direction ) {
                lemon::ListDigraph::Arc edge = findEdge( itemNode, direction );
                return ( edge == lemon::INVALID );
            }

            TItem* getItem( const TItem& from, const Direction direction ) {
                const lemon::ListDigraph::Node itemNode = findNode( from );
                if ( graph.valid( itemNode ) == false ) {
                    return nullptr;
                }
                lemon::ListDigraph::Node targetNode = getNode( itemNode, direction );
                if ( graph.valid( targetNode ) == false ) {
                    return nullptr;
                }
                TItem& targetItem = getItem( targetNode );
                return &targetItem;
            }

            TItem* addItem( const TItem& from, const Direction direction ) {
                const lemon::ListDigraph::Node itemNode = findNode( from );
                if ( graph.valid( itemNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return nullptr;
                }
                lemon::ListDigraph::Node targetNode = createNode( itemNode, direction );
                TItem& targetItem = getItem( targetNode );
                return &targetItem;
            }

            std::string print() {
                std::stringstream stream;
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    TItem& nItem = getItem( n );
                    stream << graph.id( n ) << " " << &nItem << "\n";
                }
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    TItem& nItem = getItem( n );
                    for (lemon::ListDigraph::OutArcIt e(graph, n); e != lemon::INVALID; ++e) {
                        const lemon::ListDigraph::Node tNode = targetNode( e );
                        TItem& tItem = getItem( tNode );
                        stream << graph.id( n ) << " " << &nItem << " -> " << edgeDirection[ e ] << " " << graph.id( tNode ) << " " << &tItem << "\n";
                    }
                }
                return stream.str();
            }


        protected:

            lemon::ListDigraph::Node findNode( const TItem& item ) {
                const TItem* itemPtr = &item;
                for (lemon::ListDigraph::NodeIt n( graph ); n != lemon::INVALID; ++n) {
                    const TItem* currItem = getItemPtr( n );
                    if ( currItem == itemPtr ) {
                        return n;
                    }
                }
                return lemon::ListDigraph::Node();            /// invalid node
            }

            lemon::ListDigraph::Node getNode( const lemon::ListDigraph::Node& from, const Direction direction ) {
                lemon::ListDigraph::Arc edge = findEdge( from, direction );
                if ( edge == lemon::INVALID ) {
                    return lemon::INVALID;
                }
                /// edge exists
                return targetNode( edge );
            }

            /// creates new node if needed
            lemon::ListDigraph::Node createNode( const lemon::ListDigraph::Node& from, const Direction direction ) {
                lemon::ListDigraph::Arc edge = findEdge( from, direction );
                if ( edge != lemon::INVALID ) {
                    /// edge exists
                    return targetNode( edge );
                }

                /// no edge -- add new item
                const lemon::ListDigraph::Node newNode = graph.addNode();
                createEdge( from, newNode, direction );
                return newNode;
            }

            lemon::ListDigraph::Node targetNode( const lemon::ListDigraph::Arc& edge ) {
                return graph.target( edge );
            }

            void createEdge( const lemon::ListDigraph::Node& from, const lemon::ListDigraph::Node& to, const Direction direction ) {
                lemon::ListDigraph::Arc fromEdge = graph.addArc( from, to );
                edgeDirection[ fromEdge ] = direction;
                lemon::ListDigraph::Arc toEdge = graph.addArc( to, from );
                edgeDirection[ toEdge ] = opposite( direction );
            }

            lemon::ListDigraph::Arc findEdge( const lemon::ListDigraph::Node& from, const Direction direction ) {
                /// check node edges
                for (lemon::ListDigraph::OutArcIt e(graph, from); e != lemon::INVALID; ++e) {
                    if ( edgeDirection[ e ] == direction ) {
                        return e;
                    }
                }
                return lemon::ListDigraph::Arc();
            }

            TItem& getItem( const lemon::ListDigraph::Node& node ) {
                DataPtr& roadPtr = items[ node ];
                if ( roadPtr == nullptr ) {
                    roadPtr.reset( new TItem() );
                }
                return *roadPtr;
            }

            TItem* getItemPtr( const lemon::ListDigraph::Node& node ) {
                DataPtr& roadPtr = items[ node ];
                if ( roadPtr == nullptr ) {
                    roadPtr.reset( new TItem() );
                }
                return roadPtr.get();
            }

        };

    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEONGRAPH_H_ */
