/*
 * modes.cpp
 *
 */

#include "dkmage/Generator.h"

#include "dkmage/mode/CaveMode.h"
#include "dkmage/mode/MazeMode.h"
#include "dkmage/mode/HeroFortressMode.h"

#include "utils/GlobalScope.h"


GLOBAL_SCOPE() {
    dkmage::Generator::registerMode<dkmage::mode::CaveMode>( "cave" );
    dkmage::Generator::registerMode<dkmage::mode::MazeMode>( "maze" );
    dkmage::Generator::registerMode<dkmage::mode::HeroFortressMode>( "herofortress" );
}
