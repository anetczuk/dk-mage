/*
 * AvailableState.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLESTATE_H_

#include "adiktedpp/script/AvailableStateBase.h"


namespace adiktedpp {
    namespace script {

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


        /**
         * Set creatures availability through Portal.
         */
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
