/*
 * Dungeon.cpp
 *
 */

#include "dkmage/generator/Dungeon.h"

#include <random>


namespace dkmage {
    namespace generator {

        template <typename T>
        T remove_at( std::vector<T>& v, typename std::vector<T>::size_type n ) {
            std::swap(v[n], v.back());
            T last = v.back();
            v.pop_back();
            return last;
        }

        void moveRect( Rect& rect, const Rect& base, const Direction direction ) {
            rect.centerize();
            const Point baseCenter = base.center();
            rect.move( baseCenter );

            switch( direction ) {
            case Direction::D_NORTH: {
                const int yOffset = ( rect.height() + base.height() ) / 2 + 1;      /// +1 for wall
                rect.move( 0, yOffset );
                break;
            }
            case Direction::D_SOUTH: {
                const int yOffset = ( rect.height() + base.height() ) / 2 + 1;      /// +1 for wall
                rect.move( 0, -yOffset );
                break;
            }
            case Direction::D_EAST: {
                const int xOffset = ( rect.width() + base.width() ) / 2 + 1;        /// +1 for wall
                rect.move( xOffset, 0 );
                break;
            }
            case Direction::D_WEST: {
                const int xOffset = ( rect.width() + base.width() ) / 2 + 1;        /// +1 for wall
                rect.move( -xOffset, 0 );
                break;
            }
            }

        }


        /// ============================================================


        std::string Room::print() const {
            std::stringstream stream;
            stream << "position: " << position();
            return stream.str();
        }


        /// ============================================================


        void Dungeon::generate( const std::size_t roomsNum, const std::size_t roomSize ) {
            std::vector< adiktedpp::SlabType > roomsType;
            {
                roomsType.push_back( adiktedpp::SlabType::ST_TREASURE );
                roomsType.push_back( adiktedpp::SlabType::ST_LAIR );
                roomsType.push_back( adiktedpp::SlabType::ST_HATCHERY );
                roomsType.push_back( adiktedpp::SlabType::ST_TRAINING );
                roomsType.push_back( adiktedpp::SlabType::ST_LIBRARY );
                roomsType.push_back( adiktedpp::SlabType::ST_WORKSHOP );
                roomsType.push_back( adiktedpp::SlabType::ST_PRISONCASE );
                roomsType.push_back( adiktedpp::SlabType::ST_TORTURE );
                roomsType.push_back( adiktedpp::SlabType::ST_GRAVEYARD );
                roomsType.push_back( adiktedpp::SlabType::ST_TEMPLE );
                roomsType.push_back( adiktedpp::SlabType::ST_SCAVENGER );
            }
            {
                Room& first = graph.firstItem();
                first.resize( roomSize );
                Rect& basePos = first.position();
                basePos.centerize();
                first.type( adiktedpp::SlabType::ST_DUNGHEART );
                first.owner( player );
            }
            for (std::size_t i=1; i<roomsNum; ++i) {
                const std::size_t roomIndex = ( i - 1 ) % roomsType.size();
                const adiktedpp::SlabType newRoomType = roomsType[roomIndex];
                /// randomize next room
                std::vector< Room* > roomsList = graph.itemsList();
                while ( roomsList.empty() == false ) {
                    const std::size_t rRoom = rand() % roomsList.size();
                    Room* selected = remove_at( roomsList, rRoom );
                    if ( selected == nullptr ) {
                        continue;
                    }
                    std::vector< Direction > availableDirs = graph.freeDirections( *selected );
                    const std::size_t dirSize = availableDirs.size();
                    if ( dirSize < 1 ) {
                        continue ;
                    }
                    const Rect& basePos = selected->position();
                    const std::size_t rDir = rand() % dirSize;
                    const Direction newDir = availableDirs[ rDir ];
                    Room* newRoom = graph.addItem( *selected, newDir );
                    if ( newRoom == nullptr ) {
                        continue ;
                    }

                    /// new node added
                    newRoom->resize( roomSize );
                    Rect& newPos = newRoom->position();
                    moveRect( newPos, basePos, newDir );
                    newRoom->type( newRoomType );
                    newRoom->owner( player );
                    break ;
                }
            }
        }

        Rect Dungeon::boundingBox() {
            std::vector< Room* > roomsList = graph.itemsList();
            const std::size_t rSize = roomsList.size();
            if ( rSize < 1 ) {
                return Rect();
            }
            Rect minMax = roomsList[0]->position();
            for (std::size_t i=1; i<rSize; ++i) {
                const Room* room = roomsList[ i ];
                const Rect& roomPos = room->position();
                minMax.expand( roomPos );
            }
            return minMax;
        }

        void Dungeon::centerize() {
            const Rect bbox = boundingBox();
            const Point center = bbox.center();
            move( -center.x, -center.y );
        }

        void Dungeon::centerize( const int x, const int y ) {
            centerize();
            move( x, y );
        }

        std::string Dungeon::print() {
            std::stringstream stream;
            stream << "bbox: " << boundingBox() << "\n";
            std::vector< Room* > roomsList = graph.itemsList();
            const std::size_t rSize = roomsList.size();
            for (std::size_t i=0; i<rSize; ++i) {
                const Room* room = roomsList[ i ];
                const std::size_t currId = graph.itemId( *room );
                stream << currId << ": " << room->print() << "\n";
            }
            const std::vector<Direction>& directions = DirectionValues();
            for (std::size_t i=0; i<rSize; ++i) {
                const Room* room = roomsList[ i ];
                if ( room == nullptr ) {
                    continue ;
                }
                const std::size_t currId = graph.itemId( *room );
                for ( const Direction dir: directions ) {
                    Room* next = graph.getItem( *room, dir );
                    if ( next == nullptr ) {
                        continue ;
                    }
                    const std::size_t nextId = graph.itemId( *next );
                    stream << currId << " -> " << dir << " " << nextId << "\n";
                }
            }
            return stream.str();
        }

    } /* namespace generator */
} /* namespace dkmage */
