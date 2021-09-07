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

        void RoomsAvailableState::setAllAvailable( const Player player, const RoomAvailableMode mode ) {
            const std::set< Room >& list = RoomsRegular();
            for ( const Room item: list ) {
                setStateMode( player, item, mode );
            }
        }

        void RoomsAvailableState::setStandard() {
            setAllAvailable( Player::P_ALL,                RoomAvailableMode::RAM_RESEARCHABLE );
            setStateMode( Player::P_ALL, Room::R_TREASURE, RoomAvailableMode::RAM_AVAILABLE );
            setStateMode( Player::P_ALL, Room::R_LAIR,     RoomAvailableMode::RAM_AVAILABLE );
            setStateMode( Player::P_ALL, Room::R_HATCHERY, RoomAvailableMode::RAM_AVAILABLE );
            setStateMode( Player::P_ALL, Room::R_LIBRARY,  RoomAvailableMode::RAM_AVAILABLE );
        }

        void CreatureAvailableState::setEvilAvailable( const Player player, const bool available ) {
            std::set< Creature > list = EvilCreatures();
            list.erase( Creature::C_IMP );
            list.erase( Creature::C_TENTACLE );
            list.erase( Creature::C_SKELETON );
            list.erase( Creature::C_VAMPIRE );
            list.erase( Creature::C_HORNY );
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

        /// =========================================================================================

        void MagicAvailableState::setAllAvailable( const Player player, const MagicAvailableMode mode ) {
            const std::set< Spell >& list = Spells();
            for ( const Spell item: list ) {
                setStateMode( player, item, mode );
            }
        }

        void MagicAvailableState::setStandard( const Player player ) {
            setAllAvailable( player, MagicAvailableMode::AM_RESEARCHABLE );
            setStateMode( player, Spell::S_POWER_HAND,    MagicAvailableMode::AM_AVAILABLE );
            setStateMode( player, Spell::S_POWER_POSSESS, MagicAvailableMode::AM_AVAILABLE );
            setStateMode( player, Spell::S_POWER_SLAP,    MagicAvailableMode::AM_AVAILABLE );
            setStateMode( player, Spell::S_POWER_IMP,     MagicAvailableMode::AM_AVAILABLE );
            setStateMode( player, Spell::S_POWER_SIGHT,   MagicAvailableMode::AM_DISABLED );
        }

    } /* namespace script */
} /* namespace adiktedpp */
