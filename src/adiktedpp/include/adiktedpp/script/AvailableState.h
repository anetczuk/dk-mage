/*
 * AvailableState.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_

#include "adiktedpp/script/Type.h"
#include "adiktedpp/PlayerType.h"

#include <map>


namespace adiktedpp {
    namespace script {

        /**
         *
         */
        template <typename TK1, typename TK2, typename TV>
        class Map2 {
        public:

            using SubMap = std::map< TK2, TV >;
            using Map = std::map< TK1, SubMap >;


        private:


            Map data;


        public:

            typename Map::const_iterator begin() const {
                return data.begin();
            }

            typename Map::const_iterator end() const {
                return data.end();
            }

            TV& get( const TK1 k1, const TK2 k2 ) {
                SubMap& submap = data[ k1 ];
                return submap[ k2 ];
            }

            const TV* getPtr( const TK1 k1, const TK2 k2 ) const {
                auto iter = data.find( k1 );
                if ( iter == data.end() ) {
                    return nullptr;
                }
                const SubMap& subMap = iter->second;
                auto sIter = subMap.find( k2 );
                if ( sIter == subMap.end() ) {
                    return nullptr;
                }
                return &( sIter->second );
            }

            void set( const TK1 k1, const TK2 k2, const TV val ) {
                SubMap& submap = data[ k1 ];
                submap[ k2 ] = val;
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


        /**
         * Order of commands during script execution is unspecified, so setting two commands that overwrite results is undefined.
         * Class helps preserve last command state and generate script file without overwriting.
         */
        template <typename TValue>
        class AvailableCommandStateMap {
        public:

            using DataMap = Map2< adiktedpp::PlayerType, TValue, AvailableState >;

            DataMap data;


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
                    setStateRaw( player, item, accessible, available );
                    return ;
                }

                /// all players
                setStateRaw( adiktedpp::PlayerType::PT_0, item, accessible, available );
                setStateRaw( adiktedpp::PlayerType::PT_1, item, accessible, available );
                setStateRaw( adiktedpp::PlayerType::PT_2, item, accessible, available );
                setStateRaw( adiktedpp::PlayerType::PT_3, item, accessible, available );
                setStateRaw( adiktedpp::PlayerType::PT_GOOD, item, accessible, available );
            }


        private:

            void setStateRaw( const adiktedpp::PlayerType player, const TValue item, const int accessible, const int available ) {
                AvailableState& state = data.get( player, item );
                state = AvailableState{ accessible, available };
            }

        };


        /// =============================================================================


        /**
         *
         */
        class RoomsAvailableState: public AvailableCommandStateMap< Room > {
        public:

            void setAllAvailable( const PlayerType player, const AvailableMode mode );

            void setStandard();

        };

        class CreatureAvailableState: public AvailableCommandStateMap< Creature > {
        public:

            void setEvilAvailable( const PlayerType player, const bool available = true );

            void setHeroAvailable( const PlayerType player, const bool available = true );

        };

        class DoorAvailableState: public AvailableCommandStateMap< Door > {
        public:

            void setAllAvailable( const PlayerType player, const int amount );

        };


        class TrapAvailableState: public AvailableCommandStateMap< Trap > {
        public:

            void setAllAvailable( const PlayerType player, const int amount );

        };


        class MagicAvailableState: public AvailableCommandStateMap< Spell > {
        public:

            void setAllAvailable( const PlayerType player, const AvailableMode mode );

            void setStandard( const PlayerType player );

        };

    } /* namespace script */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_ */
