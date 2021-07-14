/*
 * AvailableState.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_

#include "adiktedpp/script/Type.h"
#include "adiktedpp/PlayerType.h"

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


        struct AvailableState {
            int accessible;
            int available;

            AvailableState(): accessible(0), available(0) {
            }
            AvailableState( const int acc, const int av ): accessible(acc), available(av) {
            }
        };


        const std::set< adiktedpp::PlayerType >& Players();


        /**
         * Order of commands during script execution is unspecified, so setting two commands that overwrite results is undefined.
         * Class helps preserve last command state and generate script file without overwriting.
         */
        template <typename TValue>
        class AvailableCommandStateMap {

            using DataMap = Map2< adiktedpp::PlayerType, TValue, AvailableState >;

            DataMap data;

            std::set< adiktedpp::PlayerType > availablePlayers;


        public:

            AvailableCommandStateMap(): availablePlayers( Players() ) {
            }

            AvailableCommandStateMap( const std::set< adiktedpp::PlayerType >& players ): availablePlayers( players ) {
                availablePlayers.insert( adiktedpp::PlayerType::PT_ALL );
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
                std::set< adiktedpp::PlayerType > currPlayers = availablePlayers;
                currPlayers.erase( adiktedpp::PlayerType::PT_ALL );
                for ( const adiktedpp::PlayerType player: currPlayers ) {
                    const AvailableState* state = getState( player, item );
                    if ( state != nullptr ) {
                        ++count;
                    }
                }
                return count;
            }

            const AvailableState* getState( const adiktedpp::PlayerType player, const TValue item ) const {
                return data.getPtr( player, item );
            }

            void setStateMode( const adiktedpp::PlayerType player, const TValue item, const AvailableMode mode ) {
                switch( mode ) {
                case AvailableMode::AM_DISABLED: {
                    setState( player, item, 0, 0 );
                    break ;
                }
                case AvailableMode::AM_POSSIBLE: {
                    setState( player, item, 1, 0 );
                    break ;
                }
                case AvailableMode::AM_ENABLED: {
                    setState( player, item, 1, 1 );
                    break ;
                }
                }
            }

            void setStateFlag( const adiktedpp::PlayerType player, const TValue item, const bool available ) {
                if ( available ) {
                    setStateMode( player, item, AvailableMode::AM_ENABLED );
                } else {
                    setStateMode( player, item, AvailableMode::AM_DISABLED );
                }
            }

            /**
             * "available" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setStateAmount( const adiktedpp::PlayerType player, const TValue item, const int amount ) {
                if ( amount < 0 )
                    setState( player, item, 0, 0 );
                else if ( amount == 0 )
                    setState( player, item, 1, 0 );
                else
                    setState( player, item, 0, amount );
            }

            void setState( const adiktedpp::PlayerType player, const TValue item, const int accessible, const int available ) {
                if ( player != adiktedpp::PlayerType::PT_ALL ) {
                    /// check "all players" entry
                    const AvailableState* allState = getState( adiktedpp::PlayerType::PT_ALL, item );
                    if ( allState != nullptr ) {
                        /// all players state exists -- explode
                        explodeAllPlayers( item, allState->accessible, allState->available );
                    }
                    setStateRaw( player, item, accessible, available );
                    return ;
                }

                /// all players -- remove old entries
                erasePlayers( item );
//                players.clear();
                setStateRaw( player, item, accessible, available );
            }


        private:

            void setStateRaw( const adiktedpp::PlayerType player, const TValue item, const int accessible, const int available ) {
                if ( availablePlayers.count(player) < 1 ) {
                    std::stringstream stream;
                    stream << "invalid argument -- unavailable player: " << player;
                    throw std::invalid_argument( stream.str() );
                }

                AvailableState& state = data.get( player, item );
                state = AvailableState( accessible, available );
            }

            void explodeAllPlayers( const TValue item, const int accessible, const int available ) {
                data.erase( adiktedpp::PlayerType::PT_ALL, item );

                std::set< adiktedpp::PlayerType > currPlayers = availablePlayers;
                currPlayers.erase( adiktedpp::PlayerType::PT_ALL );
                for ( const adiktedpp::PlayerType player: currPlayers ) {
                    setStateRaw( player, item, accessible, available );
                }
            }

            void erasePlayers( const TValue item ) {
                for ( const adiktedpp::PlayerType player: availablePlayers ) {
                    data.erase( player, item );
                }
            }

        };


        /// =============================================================================


        /**
         *
         */
        class RoomsAvailableState: public AvailableCommandStateMap< Room > {
        public:

            RoomsAvailableState(): AvailableCommandStateMap< Room >() {
            }

            RoomsAvailableState( const std::set< adiktedpp::PlayerType >& players ): AvailableCommandStateMap< Room >( players ) {
            }

            void setAllAvailable( const PlayerType player, const AvailableMode mode );

            void setStandard();

        };

        class CreatureAvailableState: public AvailableCommandStateMap< Creature > {
        public:

            CreatureAvailableState(): AvailableCommandStateMap< Creature >() {
            }

            CreatureAvailableState( const std::set< adiktedpp::PlayerType >& players ): AvailableCommandStateMap< Creature >( players ) {
            }

            void setEvilAvailable( const PlayerType player, const bool available = true );

            void setHeroAvailable( const PlayerType player, const bool available = true );

        };

        class DoorAvailableState: public AvailableCommandStateMap< Door > {
        public:

            DoorAvailableState(): AvailableCommandStateMap< Door >() {
            }

            DoorAvailableState( const std::set< adiktedpp::PlayerType >& players ): AvailableCommandStateMap< Door >( players ) {
            }

            void setAllAvailable( const PlayerType player, const int amount );

        };


        class TrapAvailableState: public AvailableCommandStateMap< Trap > {
        public:

            TrapAvailableState(): AvailableCommandStateMap< Trap >() {
            }

            TrapAvailableState( const std::set< adiktedpp::PlayerType >& players ): AvailableCommandStateMap< Trap >( players ) {
            }

            void setAllAvailable( const PlayerType player, const bool available );

            void setAllAvailable( const PlayerType player, const int amount );

        };


        class MagicAvailableState: public AvailableCommandStateMap< Spell > {
        public:

            MagicAvailableState(): AvailableCommandStateMap< Spell >() {
            }

            MagicAvailableState( const std::set< adiktedpp::PlayerType >& players ): AvailableCommandStateMap< Spell >( players ) {
            }

            void setAllAvailable( const PlayerType player, const AvailableMode mode );

            void setStandard( const PlayerType player );

        };

    } /* namespace script */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_ */
