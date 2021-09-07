/*
 * AvailableState.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_

#include "adiktedpp/Type.h"
#include "adiktedpp/script/AvailableMode.h"

#include "utils/Log.h"

#include <map>
#include <sstream>
#include <stdexcept>


namespace adiktedpp {
    namespace script {

        /**
         *
         */
        template <typename TK1, typename TK2, typename TV>
        class Map2 {
        public:

            using SubMap         = std::map< TK2, TV >;
            using Map            = std::map< TK1, SubMap >;
            using const_iterator = typename Map::const_iterator;


        private:


            Map data;


        public:

            const_iterator begin() const {
                return data.begin();
            }

            const_iterator end() const {
                return data.end();
            }

            std::size_t size() const {
                return data.size();
            }

            const SubMap* getPtr( const TK1 k1 ) const {
                auto iter = data.find( k1 );
                if ( iter == data.end() ) {
                    return nullptr;
                }
                const SubMap& subMap = iter->second;
                return &subMap;
            }

            SubMap* getPtr( const TK1 k1 ) {
                auto iter = data.find( k1 );
                if ( iter == data.end() ) {
                    return nullptr;
                }
                SubMap& subMap = iter->second;
                return &subMap;
            }

            TV& get( const TK1 k1, const TK2 k2 ) {
                SubMap& submap = data[ k1 ];
                return submap[ k2 ];
            }

            const TV* getPtr( const TK1 k1, const TK2 k2 ) const {
                const SubMap* subMap = getPtr( k1 );
                if ( subMap == nullptr ) {
                    return nullptr;
                }
                auto sIter = subMap->find( k2 );
                if ( sIter == subMap->end() ) {
                    return nullptr;
                }
                return &( sIter->second );
            }

            void set( const TK1 k1, const TK2 k2, const TV val ) {
                SubMap& submap = data[ k1 ];
                submap[ k2 ] = val;
            }

            void erase( const TK1 k1 ) {
                data.erase( k1 );
            }

            void erase( const TK1 k1, const TK2 k2 ) {
                SubMap* subMap = getPtr( k1 );
                if ( subMap == nullptr ) {
                    return ;
                }
                subMap->erase( k2 );
                if ( subMap->size() < 1 ) {
                    data.erase( k1 );
                }
            }

        };


        const std::set< adiktedpp::Player >& Players();


        /**
         * Order of commands during script execution is unspecified, so setting two commands that overwrite results is undefined.
         * Class helps preserve last command state and generate script file without overwriting.
         */
        template <typename TValue, typename TState>
        class AvailableCommandStateMap {
        public:

            using DataMap = Map2< adiktedpp::Player, TValue, TState >;


        private:

            DataMap data;

            std::set< adiktedpp::Player > availablePlayers;


        public:

            AvailableCommandStateMap(): availablePlayers( Players() ) {
            }

            AvailableCommandStateMap( const std::set< adiktedpp::Player >& players ): availablePlayers( players ) {
                availablePlayers.insert( adiktedpp::Player::P_ALL );
            }

            typename DataMap::const_iterator begin() const {
                return data.begin();
            }

            typename DataMap::const_iterator end() const {
                return data.end();
            }

            std::size_t size() const {
                return data.size();
            }

            std::size_t countStates( const TValue item ) const {
                std::size_t count = 0;
                std::set< adiktedpp::Player > currPlayers = availablePlayers;
                currPlayers.erase( adiktedpp::Player::P_ALL );
                for ( const adiktedpp::Player player: currPlayers ) {
                    const TState* state = getState( player, item );
                    if ( state != nullptr ) {
                        ++count;
                    }
                }
                return count;
            }

            const TState* getState( const adiktedpp::Player player, const TValue item ) const {
                return data.getPtr( player, item );
            }

            void setState( const adiktedpp::Player player, const TValue item, const TState newState ) {
                if ( player != adiktedpp::Player::P_ALL ) {
                    /// check "all players" entry
                    const TState* allState = getState( adiktedpp::Player::P_ALL, item );
                    if ( allState != nullptr ) {
                        /// all players state exists -- explode
                        explodeAllPlayers( item, *allState );
                    }
                    setStateRaw( player, item, newState );
                    return ;
                }

                /// all players -- remove old entries
                erasePlayers( item );
//                players.clear();
                setStateRaw( player, item, newState );
            }


        protected:

            void explodeAllPlayers( const TValue item, const TState newState ) {
                data.erase( adiktedpp::Player::P_ALL, item );

                std::set< adiktedpp::Player > currPlayers = availablePlayers;
                currPlayers.erase( adiktedpp::Player::P_ALL );
                for ( const adiktedpp::Player player: currPlayers ) {
                    setStateRaw( player, item, newState );
                }
            }

            void setStateRaw( const adiktedpp::Player player, const TValue item, const TState newState ) {
                if ( availablePlayers.count(player) < 1 ) {
                    std::stringstream stream;
                    stream << FILE_NAME << ": invalid argument -- unavailable player: " << (int)player;
                    throw std::invalid_argument( stream.str() );
                }

                TState& state = data.get( player, item );
                state = newState;
            }

            void erasePlayers( const TValue item ) {
                for ( const adiktedpp::Player player: availablePlayers ) {
                    data.erase( player, item );
                }
            }

        };


        /// =============================================================================


        /**
         *
         */
        class RoomsAvailableState {
        public:

            using Data = AvailableCommandStateMap< Room, RoomAvailableMode >;
            using DataMap = Data::DataMap;


        private:

            Data data;


        public:

            RoomsAvailableState() {
            }

            RoomsAvailableState( const std::set< adiktedpp::Player >& players ): data( players ) {
            }

            typename DataMap::const_iterator begin() const {
                return data.begin();
            }

            typename DataMap::const_iterator end() const {
                return data.end();
            }

            std::size_t size() const {
                return data.size();
            }

            const RoomAvailableMode* getState( const adiktedpp::Player player, const Room item ) const {
                return data.getState( player, item );
            }

            void setStateMode( const adiktedpp::Player player, const Room item, const RoomAvailableMode mode ) {
                data.setState( player, item, mode );
            }

            void setAllAvailable( const Player player, const RoomAvailableMode mode );

            void setStandard();

        };


        /// ================================================================================


        class CreatureAvailableState {
        public:

            struct State {
                bool available;
                int numberForced;

                State(): available(false), numberForced(0) {
                }
                State( const bool av, const int num ): available(av), numberForced(num) {
                }
            };

            using Data = AvailableCommandStateMap< Creature, State >;
            using DataMap = Data::DataMap;


        private:

            Data data;


        public:

            CreatureAvailableState() {
            }

            CreatureAvailableState( const std::set< adiktedpp::Player >& players ): data( players ) {
            }

            typename DataMap::const_iterator begin() const {
                return data.begin();
            }

            typename DataMap::const_iterator end() const {
                return data.end();
            }

            std::size_t size() const {
                return data.size();
            }

            const State* getState( const adiktedpp::Player player, const Creature item ) const {
                return data.getState( player, item );
            }

            void setStateMode( const adiktedpp::Player player, const Creature item, const AvailableMode mode ) {
                switch( mode ) {
                case AvailableMode::AM_DISABLED: {
                    const State newState( false, 0 );
                    data.setState( player, item, newState );
                    break ;
                }
                case AvailableMode::AM_POSSIBLE: {
                    const State newState( true, 0 );
                    data.setState( player, item, newState );
                    break ;
                }
                case AvailableMode::AM_ENABLED: {
                    const State newState( true, 0 );
                    data.setState( player, item, newState );
                    break ;
                }
                }
            }

            void setStateFlag( const adiktedpp::Player player, const Creature item, const bool available ) {
                if ( available ) {
                    setStateMode( player, item, AvailableMode::AM_ENABLED );
                } else {
                    setStateMode( player, item, AvailableMode::AM_DISABLED );
                }
            }

            void setEvilAvailable( const Player player, const bool available = true );

            void setHeroAvailable( const Player player, const bool available = true );

        };


        /// ================================================================================


        template <typename TValue>
        class WorkshopAvailableState {
        public:

            struct State {
                bool constructable;
                int numberAvailable;

                State(): constructable(0), numberAvailable(0) {
                }
                State( const int con, const int num ): constructable(con), numberAvailable(num) {
                }
            };

            using Data = AvailableCommandStateMap< TValue, State >;
            using DataMap = typename Data::DataMap;


        private:

            Data data;


        public:

            WorkshopAvailableState() {
            }

            WorkshopAvailableState( const std::set< adiktedpp::Player >& players ): data( players ) {
            }

            typename DataMap::const_iterator begin() const {
                return data.begin();
            }

            typename DataMap::const_iterator end() const {
                return data.end();
            }

            std::size_t size() const {
                return data.size();
            }

            const State* getState( const adiktedpp::Player player, const TValue item ) const {
                return data.getState( player, item );
            }

            void setStateMode( const adiktedpp::Player player, const TValue item, const AvailableMode mode ) {
                switch( mode ) {
                case AvailableMode::AM_DISABLED: {
                    const State newState( false, 0 );
                    data.setState( player, item, newState );
                    break ;
                }
                case AvailableMode::AM_POSSIBLE: {
                    const State newState( true, 0 );
                    data.setState( player, item, newState );
                    break ;
                }
                case AvailableMode::AM_ENABLED: {
                    const State newState( true, 1 );
                    data.setState( player, item, newState );
                    break ;
                }
                }
            }

            void setStateFlag( const adiktedpp::Player player, const TValue item, const bool available ) {
                if ( available ) {
                    this->setStateMode( player, item, AvailableMode::AM_ENABLED );
                } else {
                    this->setStateMode( player, item, AvailableMode::AM_DISABLED );
                }
            }

            /**
             * "available" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setStateAmount( const adiktedpp::Player player, const TValue item, const int amount ) {
                if ( amount < 0 ) {
                    const State newState( false, 0 );
                    data.setState( player, item, newState );
                } else if ( amount == 0 ) {
                    const State newState( true, 0 );
                    data.setState( player, item, newState );
                } else {
                    const State newState( true, amount );
                    data.setState( player, item, newState );
                }
            }

        };


        class DoorAvailableState: public WorkshopAvailableState< Door > {
        public:

            DoorAvailableState(): WorkshopAvailableState< Door >() {
            }

            DoorAvailableState( const std::set< adiktedpp::Player >& players ): WorkshopAvailableState< Door >( players ) {
            }

            void setAllAvailable( const Player player, const bool available ) {
                const std::set< Door >& list = Doors();
                for ( const Door item: list ) {
                    if ( available ) {
                        this->setStateMode( player, item, AvailableMode::AM_POSSIBLE );
                    } else {
                        this->setStateMode( player, item, AvailableMode::AM_DISABLED );
                    }
                }
            }

            void setAllAvailableAmount( const Player player, const int amount ) {
                const std::set< Door >& list = Doors();
                for ( const Door item: list ) {
                    setStateAmount( player, item, amount );
                }
            }

        };


        class TrapAvailableState: public WorkshopAvailableState< Trap > {
        public:

            TrapAvailableState(): WorkshopAvailableState< Trap >() {
            }

            TrapAvailableState( const std::set< adiktedpp::Player >& players ): WorkshopAvailableState< Trap >( players ) {
            }

            void setAllAvailable( const Player player, const bool available ) {
                const std::set< Trap >& list = Traps();
                for ( const Trap item: list ) {
                    if ( available ) {
                        this->setStateMode( player, item, AvailableMode::AM_POSSIBLE );
                    } else {
                        this->setStateMode( player, item, AvailableMode::AM_DISABLED );
                    }
                }
            }

            void setAllAvailableAmount( const Player player, const int amount ) {
                const std::set< Trap >& list = Traps();
                for ( const Trap item: list ) {
                    setStateAmount( player, item, amount );
                }
            }

        };


        /// ================================================================================


        class MagicAvailableState {
        public:

            using Data = AvailableCommandStateMap< Spell, MagicAvailableMode >;
            using DataMap = Data::DataMap;


        private:

            Data data;


        public:

            MagicAvailableState() {
            }

            MagicAvailableState( const std::set< adiktedpp::Player >& players ): data( players ) {
            }

            typename DataMap::const_iterator begin() const {
                return data.begin();
            }

            typename DataMap::const_iterator end() const {
                return data.end();
            }

            std::size_t size() const {
                return data.size();
            }

            const MagicAvailableMode* getState( const adiktedpp::Player player, const Spell item ) const {
                return data.getState( player, item );
            }

            void setStateMode( const adiktedpp::Player player, const Spell item, const MagicAvailableMode mode ) {
                data.setState( player, item, mode );
            }

            void setAllAvailable( const Player player, const MagicAvailableMode mode );

            void setStandard( const Player player );

        };

    } /* namespace script */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_ */
