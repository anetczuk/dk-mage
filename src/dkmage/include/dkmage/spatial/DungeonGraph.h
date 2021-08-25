/*
 * DungeonGraph.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEONGRAPH_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEONGRAPH_H_

#include "utils/Rect.h"
#include "utils/Log.h"

#include <lemon/list_graph.h>

#include <set>
#include <map>
#include <sstream>


namespace dkmage {
    namespace spatial {

        /**
         *
         */
        enum class Direction {
            D_NORTH,
            D_SOUTH,
            D_EAST,
            D_WEST
        };

        inline std::ostream& operator<<( std::ostream& os, const Direction data ) {
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

        utils::Point edgePoint( const utils::Rect& rect, const Direction direction, const std::size_t delta );


        /// ==================================================================


        /**
         *
         */
        enum class Axis {
            A_HORIZONTAL,
            A_VERTICAL
        };

        inline std::ostream& operator<<( std::ostream& os, const Axis data ) {
            switch( data ) {
            case Axis::A_HORIZONTAL: {
                os << "A_HORIZONTAL";
                return os;
            }
            case Axis::A_VERTICAL: {
                os << "A_VERTICAL";
                return os;
            }
            }
            os << "UNKNOWN[" << (int) data << "]";
            return os;
        }

        inline Axis get_axis( const Direction direction ) {
            switch( direction ) {
            case Direction::D_NORTH: return Axis::A_VERTICAL;
            case Direction::D_SOUTH: return Axis::A_VERTICAL;
            case Direction::D_EAST:  return Axis::A_HORIZONTAL;
            case Direction::D_WEST:  return Axis::A_HORIZONTAL;
            }
            return Axis::A_HORIZONTAL;
        }


        /// ==================================================================


        /**
         * Implementation of rule of five.
         */
        template <typename T>
        struct NodeDataWrapper {
            std::unique_ptr<T> data;

            NodeDataWrapper(): data( nullptr ) {
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

        inline std::ostream& operator<<( std::ostream& os, const NoEdgeData& /*data*/ ) {
            /// do nothing
            return os;
        }


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

        template <typename T>
        inline std::ostream& operator<<( std::ostream& os, const EdgeDataWrapper<T>& data ) {
            os << data.dir << " " << data.data.get();
            return os;
        }


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

            std::size_t nodesNum() const {
                return (std::size_t) lemon::countNodes( graph );
            }

            std::size_t edgesNum() const {
                return (std::size_t) lemon::countArcs( graph );
            }

            TNodeData* rootItem() {
                return itemByindex( 0 );
            }

            TNodeData* itemByindex( const std::size_t index ) {
                lemon::ListDigraph::Node node = graph.nodeFromId( index );
                if ( graph.valid( node ) == false ) {
                    return nullptr;
                }
                return getItemPtr( node );
            }

            std::vector< const TNodeData* > itemsList() const {
                std::vector< const TNodeData* > ret;
                ret.reserve( nodesNum() );
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    const TNodeData* nItem = getItemPtr( n );
                    if ( nItem == nullptr ) {
                        continue ;
                    }
                    ret.push_back( nItem );
                }
//                std::reverse( ret.begin(), ret.end() );
                return ret;
            }

            std::vector< TNodeData* > itemsList() {
                std::vector< TNodeData* > ret;
                ret.reserve( nodesNum() );
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    TNodeData* nItem = getItemPtr( n );
                    if ( nItem == nullptr ) {
                        continue ;
                    }
                    ret.push_back( nItem );
                }
                return std::vector< TNodeData* >( ret.rbegin(), ret.rend() );
            }

            std::vector< std::pair<const TNodeData*, const TNodeData*> > connectedItems() const {
                std::vector< std::pair<const TNodeData*, const TNodeData*> > ret;

                const std::vector< const TNodeData* > allItems = itemsList();
                std::map< const TNodeData*, std::set< const TNodeData* > > handled;
                for ( const TNodeData* item: allItems ) {
                    auto& subset = handled[ item ];

                    const std::vector< const TNodeData* > connectedList = connectedItems( *item );
                    for ( const TNodeData* connected: connectedList ) {
                        if ( subset.count(connected) > 0 ) {
                            continue ;
                        }
                        subset.insert(connected);
                        auto& reverseset = handled[ connected ];
                        reverseset.insert(item);

                        ret.push_back( std::make_pair(item, connected) );
                    }

                }

                return ret;
            }

            /// find nodes connected to given 'item'
            std::vector< const TNodeData* > connectedItems( const TNodeData& item ) const {
                const lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    return std::vector< const TNodeData* >();
                }
                std::vector< const TNodeData* > ret;
                const std::vector<Direction>& directions = DirectionValues();
                for ( const Direction dir: directions ) {
                    const TNodeData* target = getItem( item, dir );
                    if ( target == nullptr ) {
                        continue ;
                    }
                    ret.push_back( target );
                }
                return ret;
            }

            /// find nodes connected to given 'item'
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

            std::size_t itemId( const TNodeData& item ) const {
                lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    return (std::size_t) -1;
                }
                return graph.id( itemNode );
            }

            /// return list of directions without connected nodes
            std::vector< Direction > freeDirections( const TNodeData& item ) const {
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

            /// return list of directions with existing edges (connected nodes)
            std::vector< Direction > linkDirections( const TNodeData& item ) const {
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

            /// is there no edge (with connected node)?
            bool isFree( const lemon::ListDigraph::Node& itemNode, const Direction direction ) const {
                lemon::ListDigraph::Arc edge = findEdge( itemNode, direction );
                return ( edge == lemon::INVALID );
            }

            const TNodeData* getItem( const TNodeData& from, const Direction direction ) const {
                const lemon::ListDigraph::Node itemNode = findNode( from );
                if ( graph.valid( itemNode ) == false ) {
                    return nullptr;
                }
                lemon::ListDigraph::Node targetNode = getNode( itemNode, direction );
                if ( graph.valid( targetNode ) == false ) {
                    return nullptr;
                }
                return getItemPtr( targetNode );
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
                return getItemPtr( targetNode );
            }

            TNodeData* addItem() {
                lemon::ListDigraph::Node targetNode = graph.addNode();
                if ( root == lemon::INVALID ) {
                    root = targetNode;
                }
                TNodeData& targetItem = getItem( targetNode );
                return &targetItem;
//                GraphNode& roadPtr = nodesMap[ targetNode ];
//                roadPtr.reset( new TNodeData() );
//                return roadPtr.get();
            }

            void addItem( TNodeData& item ) {
                lemon::ListDigraph::Node targetNode = graph.addNode();
                if ( root == lemon::INVALID ) {
                    root = targetNode;
                }
                GraphNode& roadPtr = nodesMap[ targetNode ];
                roadPtr.reset( &item );
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

            bool removeItem( const TNodeData& item ) {
                const lemon::ListDigraph::Node itemNode = findNode( item );
                if ( graph.valid( itemNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return false;
                }

                /// erase node and connected arcs
                graph.erase( itemNode );
                return true;
            }

            bool setItem( TNodeData& item, const TNodeData& from, const Direction direction, const bool addLink = true ) {
                const lemon::ListDigraph::Node itemNode = findNode( from );
                if ( graph.valid( itemNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return false;
                }
                lemon::ListDigraph::Node targetNode = createNode( itemNode, direction, addLink );
                setItem( targetNode, item );
                return true;
            }

            TEdgeData* getEdge( const TNodeData& from, const TNodeData& to ) {
                const lemon::ListDigraph::Node fromNode = findNode( from );
                if ( graph.valid( fromNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return nullptr;
                }
                const lemon::ListDigraph::Node toNode = findNode( to );
                if ( graph.valid( fromNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return nullptr;
                }

                lemon::ListDigraph::Arc edge = findEdge( fromNode, toNode );
                if ( edge == lemon::INVALID ) {
                    LOG() << "unable to find edge";
                    return nullptr;
                }
                GraphEdge& graphEdge = edgesMap[ edge ];
                return graphEdge.data.get();
            }

            TEdgeData* getEdge( const TNodeData& from, const Direction dir ) {
                const lemon::ListDigraph::Node fromNode = findNode( from );
                if ( graph.valid( fromNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return nullptr;
                }
                lemon::ListDigraph::Arc edge = findEdge( fromNode, dir );
                if ( edge == lemon::INVALID ) {
                    LOG() << "unable to find edge";
                    return nullptr;
                }
                GraphEdge& graphEdge = edgesMap[ edge ];
                return graphEdge.data.get();
            }

            void addEdge( const TNodeData& from, const TNodeData& to, const Direction direction ) {
                const lemon::ListDigraph::Node fromNode = findNode( from );
                if ( graph.valid( fromNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return ;
                }
                const lemon::ListDigraph::Node toNode = findNode( to );
                if ( graph.valid( toNode ) == false ) {
                    LOG() << "unable to find node for given item";
                    return ;
                }
                createEdge( fromNode, toNode, direction );
            }

            std::string print() {
                std::stringstream stream;
                stream << "nodes:\n";
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    const TNodeData* nItem = getItemPtr( n );
                    stream << graph.id( n ) << " " << nItem << "\n";
                }

                stream << "edges:\n";
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    for (lemon::ListDigraph::OutArcIt e(graph, n); e != lemon::INVALID; ++e) {
                        const GraphEdge& edgeData = edgesMap[ e ];
                        const TEdgeData* rawData = edgeData.data.get();
                        if ( rawData == nullptr ) {
                            stream << rawData << " nullptr\n";
                        } else {
                            stream << rawData << " " << (*rawData) << "\n";
                        }
                    }
                }

                stream << "connections:\n";
                for (lemon::ListDigraph::NodeIt n(graph); n != lemon::INVALID; ++n) {
                    const TNodeData* nItem = getItemPtr( n );
                    for (lemon::ListDigraph::OutArcIt e(graph, n); e != lemon::INVALID; ++e) {
                        const lemon::ListDigraph::Node tNode = targetNode( e );
                        const TNodeData* tItem = getItemPtr( tNode );
                        stream << graph.id( n ) << " " << nItem << " " << graph.id( tNode ) << " " << tItem << " " << edgesMap[ e ] << "\n";
                    }
                }
                return stream.str();
            }


        protected:

            lemon::ListDigraph::Node findNode( const TNodeData& item ) const {
                const TNodeData* itemPtr = &item;
                for (lemon::ListDigraph::NodeIt n( graph ); n != lemon::INVALID; ++n) {
                    const TNodeData* currItem = getItemPtr( n );
                    if ( currItem == itemPtr ) {
                        return n;
                    }
                }
                return lemon::ListDigraph::Node();            /// invalid node
            }

            lemon::ListDigraph::Node getNode( const lemon::ListDigraph::Node& from, const Direction direction ) const {
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

            lemon::ListDigraph::Node targetNode( const lemon::ListDigraph::Arc& edge ) const {
                return graph.target( edge );
            }

            void createEdge( const lemon::ListDigraph::Node& from, const lemon::ListDigraph::Node& to, const Direction direction ) {
                lemon::ListDigraph::Arc fromEdge = graph.addArc( from, to );
                std::shared_ptr< TEdgeData > edgeData( new TEdgeData() );
                edgesMap[ fromEdge ] = GraphEdge( direction, edgeData );
                lemon::ListDigraph::Arc toEdge = graph.addArc( to, from );
                edgesMap[ toEdge ] = GraphEdge( opposite( direction ), edgeData );
            }

            lemon::ListDigraph::Arc findEdge( const lemon::ListDigraph::Node& from, const Direction direction ) const {
                /// check node edges
                for (lemon::ListDigraph::OutArcIt e(graph, from); e != lemon::INVALID; ++e) {
                    if ( edgesMap[ e ] == direction ) {
                        return e;
                    }
                }
                return lemon::ListDigraph::Arc( lemon::INVALID );
            }

            lemon::ListDigraph::Arc findEdge( const lemon::ListDigraph::Node& from, const lemon::ListDigraph::Node& to ) {
                /// check node edges
                for (lemon::ListDigraph::OutArcIt e(graph, from); e != lemon::INVALID; ++e) {
                    const lemon::ListDigraph::Node tNode = graph.target( e );
                    if ( tNode == to ) {
                        return e;
                    }
                }
                return lemon::ListDigraph::Arc( lemon::INVALID );
            }

            void setItem( const lemon::ListDigraph::Node& node, TNodeData& item ) {
                GraphNode& roadPtr = nodesMap[ node ];
                roadPtr.reset( &item );
            }

            TNodeData& getItem( const lemon::ListDigraph::Node& node ) {
                GraphNode& roadPtr = nodesMap[ node ];
                if ( roadPtr == nullptr ) {
                    roadPtr.reset( new TNodeData() );
                }
                return *roadPtr;
            }

            const TNodeData* getItemPtr( const lemon::ListDigraph::Node& node ) const {
                const GraphNode& roadPtr = nodesMap[ node ];
                if ( roadPtr == nullptr ) {
                    return nullptr;
                }
                return roadPtr.get();
            }

            TNodeData* getItemPtr( const lemon::ListDigraph::Node& node ) {
                GraphNode& roadPtr = nodesMap[ node ];
                if ( roadPtr == nullptr ) {
                    return nullptr;
                }
                return roadPtr.get();
            }

        };

    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEONGRAPH_H_ */
