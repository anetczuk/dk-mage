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
        struct NodeDataWrapper {
            std::unique_ptr<T> data;

            NodeDataWrapper(): data( new T() ) {
            }

            /// copy constructor
            NodeDataWrapper( const NodeDataWrapper& dataPtr ): NodeDataWrapper() {
                /// swap have to be performed in case of proper execution
                NodeDataWrapper* dataCast = const_cast<NodeDataWrapper*>( &dataPtr );
                data.swap( dataCast->data );

    //                T* dataRaw = get();
    //                *dataRaw = dataPtr.ref();
            }

            /// move constructor
            NodeDataWrapper( NodeDataWrapper&& dataPtr ) noexcept: NodeDataWrapper() {
                data.swap( dataPtr.data );
            }

            ~NodeDataWrapper() {
            }

            /// copy assignment
            NodeDataWrapper& operator=( const NodeDataWrapper& dataPtr ) {
                *this = NodeDataWrapper( dataPtr );
                return *this;
            }

            /// move assignment
            NodeDataWrapper& operator=( NodeDataWrapper&& dataPtr ) noexcept {
                *this = NodeDataWrapper( dataPtr );
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
        class NoEdgeData {
        };


        /**
         *
         */
        template <typename T>
        class EdgeDataWrapper {
        public:

            Direction dir;
            std::shared_ptr< T > data;

            EdgeDataWrapper(): dir(), data() {
            }

            EdgeDataWrapper( const Direction direction, std::shared_ptr< T >& data ): dir( direction ), data( data ) {
            }

            bool operator==( const Direction direction ) const {
                return ( dir == direction );
            }

            bool valid() const {
                return (data.get() != nullptr);
            }

        };


        /// =================================================================================


        /**
         *
         */
        template <typename TNodeData, typename TEdgeData>
        class DungeonGraph {
        public:

            typedef NodeDataWrapper<TNodeData> GraphNode;
            typedef EdgeDataWrapper<TEdgeData> GraphEdge;

            lemon::ListDigraph graph;
            lemon::ListDigraph::NodeMap< GraphNode > nodesMap;
            lemon::ListDigraph::ArcMap< GraphEdge >  edgesMap;

            lemon::ListDigraph::Node root;


            DungeonGraph(): graph(), nodesMap( graph ), edgesMap( graph ), root( lemon::INVALID ) {
            }

            std::size_t size() const {
                return (std::size_t) lemon::countNodes( graph );
            }

            TNodeData* rootItem() {
                return itemByindex( 0 );
            }

            TNodeData* itemByindex( const std::size_t index ) {
                lemon::ListDigraph::Node node = graph.nodeFromId( index );
                if ( graph.valid( node ) == false ) {
                    return nullptr;
                }
                TNodeData& item = getItem( node );
                return &item;
            }

            std::vector< TNodeData* > itemsList() {
                std::vector< TNodeData* > ret;
                ret.reserve( size() );
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    TNodeData& nItem = getItem( n );
                    ret.push_back( &nItem );
                }
                return ret;
            }

            std::vector< TNodeData* > connectedItems( const TNodeData& item ) {
                const lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    return std::vector< TNodeData* >();
                }
                std::vector< TNodeData* > ret;
                const std::vector<Direction>& directions = DirectionValues();
                for ( const Direction dir: directions ) {
                    TNodeData* target = getItem( item, dir );
                    if ( target == nullptr ) {
                        continue ;
                    }
                    ret.push_back( target );
                }
                return ret;
            }

            std::size_t itemId( const TNodeData& item ) {
                lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    return (std::size_t) -1;
                }
                return graph.id( itemNode );
            }

            std::vector< Direction > freeDirections( const TNodeData& item ) {
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

            std::vector< Direction > availableDirections( const TNodeData& item ) {
                const lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    return std::vector< Direction >();
                }
                std::vector< Direction > ret;
                const std::vector<Direction>& directions = DirectionValues();
                for ( const Direction dir: directions ) {
                    if ( isFree( itemNode, dir ) == false ) {
                        ret.push_back( dir );
                    }

                }
                return ret;
            }

            bool isFree( const lemon::ListDigraph::Node& itemNode, const Direction direction ) {
                lemon::ListDigraph::Arc edge = findEdge( itemNode, direction );
                return ( edge == lemon::INVALID );
            }

            TNodeData* getItem( const TNodeData& from, const Direction direction ) {
                const lemon::ListDigraph::Node itemNode = findNode( from );
                if ( graph.valid( itemNode ) == false ) {
                    return nullptr;
                }
                lemon::ListDigraph::Node targetNode = getNode( itemNode, direction );
                if ( graph.valid( targetNode ) == false ) {
                    return nullptr;
                }
                TNodeData& targetItem = getItem( targetNode );
                return &targetItem;
            }

            TNodeData* addItem() {
                lemon::ListDigraph::Node targetNode = graph.addNode();
                if ( root == lemon::INVALID ) {
                    root = targetNode;
                }
                TNodeData& targetItem = getItem( targetNode );
                return &targetItem;
            }

            TNodeData* addItem( const TNodeData& from, const Direction direction, const bool addLink = true ) {
                const lemon::ListDigraph::Node itemNode = findNode( from );
                if ( graph.valid( itemNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return nullptr;
                }
                lemon::ListDigraph::Node targetNode = createNode( itemNode, direction, addLink );
                TNodeData& targetItem = getItem( targetNode );
                return &targetItem;
            }

            std::string print() {
                std::stringstream stream;
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    TNodeData& nItem = getItem( n );
                    stream << graph.id( n ) << " " << &nItem << "\n";
                }
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    TNodeData& nItem = getItem( n );
                    for (lemon::ListDigraph::OutArcIt e(graph, n); e != lemon::INVALID; ++e) {
                        const lemon::ListDigraph::Node tNode = targetNode( e );
                        TNodeData& tItem = getItem( tNode );
                        stream << graph.id( n ) << " " << &nItem << " -> " << edgesMap[ e ] << " " << graph.id( tNode ) << " " << &tItem << "\n";
                    }
                }
                return stream.str();
            }


        protected:

            lemon::ListDigraph::Node findNode( const TNodeData& item ) {
                const TNodeData* itemPtr = &item;
                for (lemon::ListDigraph::NodeIt n( graph ); n != lemon::INVALID; ++n) {
                    const TNodeData* currItem = getItemPtr( n );
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
            lemon::ListDigraph::Node createNode( const lemon::ListDigraph::Node& from, const Direction direction, const bool addLink = true ) {
                lemon::ListDigraph::Arc edge = findEdge( from, direction );
                if ( edge != lemon::INVALID ) {
                    /// edge exists
                    return targetNode( edge );
                }

                /// no edge -- add new item
                const lemon::ListDigraph::Node newNode = graph.addNode();
                if ( addLink ) {
                    createEdge( from, newNode, direction );
                }
                return newNode;
            }

            lemon::ListDigraph::Node targetNode( const lemon::ListDigraph::Arc& edge ) {
                return graph.target( edge );
            }

            void createEdge( const lemon::ListDigraph::Node& from, const lemon::ListDigraph::Node& to, const Direction direction ) {
                lemon::ListDigraph::Arc fromEdge = graph.addArc( from, to );
                std::shared_ptr< TEdgeData > edgeData( new TEdgeData() );
                edgesMap[ fromEdge ] = GraphEdge( direction, edgeData );
                lemon::ListDigraph::Arc toEdge = graph.addArc( to, from );
                edgesMap[ toEdge ] = GraphEdge( opposite( direction ), edgeData );
            }

            lemon::ListDigraph::Arc findEdge( const lemon::ListDigraph::Node& from, const Direction direction ) {
                /// check node edges
                for (lemon::ListDigraph::OutArcIt e(graph, from); e != lemon::INVALID; ++e) {
                    if ( edgesMap[ e ] == direction ) {
                        return e;
                    }
                }
                return lemon::ListDigraph::Arc();
            }

            TNodeData& getItem( const lemon::ListDigraph::Node& node ) {
                GraphNode& roadPtr = nodesMap[ node ];
                if ( roadPtr == nullptr ) {
                    roadPtr.reset( new TNodeData() );
                }
                return *roadPtr;
            }

            TNodeData* getItemPtr( const lemon::ListDigraph::Node& node ) {
                GraphNode& roadPtr = nodesMap[ node ];
                if ( roadPtr == nullptr ) {
                    roadPtr.reset( new TNodeData() );
                }
                return roadPtr.get();
            }

        };

    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEONGRAPH_H_ */
