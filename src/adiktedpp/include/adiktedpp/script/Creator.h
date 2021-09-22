/*
 * Creator.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_CREATOR_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_CREATOR_H_

#include "adiktedpp/script/Script.h"
#include "adiktedpp/Type.h"

#include <string>


namespace adiktedpp {
    namespace script {

        /**
         *
         */
        class AmbushCreator {
        public:

            adiktedpp::script::Script& script;

            std::string partyId;                    /// unique value among parties
            std::string description;                /// use in comment
            std::size_t actionPoint = 0;
            std::size_t partyCopies = 1;
            Player owner = Player::P_GOOD;


            AmbushCreator( adiktedpp::script::Script& script ): script(script) {
            }

            void prepare();

            void addToParty( const Creature creature, const std::size_t crNum, const std::size_t crExp, const std::size_t gold, const PartyObjective objective, const std::size_t countdown = 0 );

        };


        /// ====================================================


        class RandomSelectCreator {
        public:

            class Item {

                RandomSelectCreator* parent;


            public:

                Item( RandomSelectCreator& parent, const std::size_t& index ): parent( &parent ) {
                    parent.addIf( index + 1 );
                }

                ~Item() {
                    parent->addEndIf();
                }

            };

            using ItemContainer = std::vector< Item >;


            Player player;
            Flag flag;
            bool reusable;
            script::ScriptCommand* section;


            RandomSelectCreator( const Player player, const Flag flag, const bool reusable, script::ScriptCommand& section ):
                player( player ), flag( flag ), reusable( reusable ), section( &section )
            {
            }

            void addParties( const std::vector< std::string >& data, const int actionPoint ) {
                const std::size_t dSize = data.size();
                start( dSize );
                for ( std::size_t i=0; i<dSize; ++i ) {
                    const Item item = next( i );
                    const std::string& party = data[i];
                    section->ADD_PARTY_TO_LEVEL( player, party, actionPoint, 1 );
                }
            }

            void start( const std::size_t size ) {
                if ( reusable ) {
                    section->NEXT_COMMAND_REUSABLE();
                }
                section->RANDOMISE_FLAG( player, flag, size );
            }

            Item next( const std::size_t index ) {
                return Item{ *this, index };
            }

            void addIf( const std::size_t number ) {
                section->IF_flag( player, flag, "==", number );
                if ( reusable ) {
                    section->NEXT_COMMAND_REUSABLE();
                }
            }

            void addEndIf() {
                section->ENDIF();
            }

        };

    } /* namespace script */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_CREATOR_H_ */
