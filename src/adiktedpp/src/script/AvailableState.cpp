/*
 * AvailableState.cpp
 *
 */

#include "adiktedpp/script/AvailableState.h"


namespace adiktedpp {
    namespace script {

        const std::set< Player >& Players() {
            static std::set< Player > container;
            if ( container.empty() ) {
                container.insert( Player::P_P0 );
                container.insert( Player::P_P1 );
                container.insert( Player::P_P2 );
                container.insert( Player::P_P3 );
                container.insert( Player::P_GOOD );
                container.insert( Player::P_ALL );
            }
            return container;
        }

        /// =======================================================

        void RoomsAvailableState::setAllAvailable( const Player player, const AvailableMode mode ) {
            const std::set< Room >& list = RoomsRegular();
            for ( const Room item: list ) {
                setStateMode( player, item, mode );
            }
        }

        void RoomsAvailableState::setStandard() {
            setAllAvailable( Player::P_ALL, AvailableMode::AM_POSSIBLE );
            setStateMode( Player::P_ALL, Room::R_TREASURE, AvailableMode::AM_ENABLED );
            setStateMode( Player::P_ALL, Room::R_LAIR, AvailableMode::AM_ENABLED );
            setStateMode( Player::P_ALL, Room::R_HATCHERY, AvailableMode::AM_ENABLED );
            setStateMode( Player::P_ALL, Room::R_LIBRARY, AvailableMode::AM_ENABLED );
        }

        void CreatureAvailableState::setEvilAvailable( const Player player, const bool available ) {
            std::set< Creature > list = EvilCreatures();
            list.erase( Creature::C_HORNY );
            list.erase( Creature::C_IMP );
            list.erase( Creature::C_TENTACLE );
            list.erase( Creature::C_VAMPIRE );
            list.erase( Creature::C_SKELETON );
            for ( const Creature item: list ) {
                setStateFlag( player, item, available );
            }
        }

        void CreatureAvailableState::setHeroAvailable( const Player player, const bool available ) {
            std::set< Creature > list = HeroCreatures();
            list.erase( Creature::C_AVATAR );
            for ( const Creature item: list ) {
                setStateFlag( player, item, available );
            }
        }

        void DoorAvailableState::setAllAvailable( const Player player, const int amount ) {
            const std::set< Door >& list = Doors();
            for ( const Door item: list ) {
                setStateAmount( player, item, amount );
            }
        }

        void TrapAvailableState::setAllAvailable( const Player player, const bool available ) {
            const std::set< Trap >& list = Traps();
            for ( const Trap item: list ) {
                if ( available ) {
                    setStateMode( player, item, AvailableMode::AM_POSSIBLE );
                } else {
                    setStateMode( player, item, AvailableMode::AM_DISABLED );
                }
            }
        }

        void TrapAvailableState::setAllAvailable( const Player player, const int amount ) {
            const std::set< Trap >& list = Traps();
            for ( const Trap item: list ) {
                setStateAmount( player, item, amount );
            }
        }

        void MagicAvailableState::setAllAvailable( const Player player, const AvailableMode mode ) {
            const std::set< Spell >& list = Spells();
            for ( const Spell item: list ) {
                setStateMode( player, item, mode );
            }
        }

        void MagicAvailableState::setStandard( const Player player ) {
            setAllAvailable( player, AvailableMode::AM_POSSIBLE );
            setStateMode( player, Spell::S_POWER_HAND, AvailableMode::AM_ENABLED );
            setStateMode( player, Spell::S_POWER_POSSESS, AvailableMode::AM_ENABLED );
            setStateMode( player, Spell::S_POWER_SLAP, AvailableMode::AM_ENABLED );
            setStateMode( player, Spell::S_POWER_IMP, AvailableMode::AM_ENABLED );
            setStateMode( player, Spell::S_POWER_SIGHT, AvailableMode::AM_DISABLED );
        }

    } /* namespace script */
} /* namespace adiktedpp */
