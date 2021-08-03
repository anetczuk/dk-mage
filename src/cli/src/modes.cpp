/*
 * modes.cpp
 *
 */

#include "dkmage/Generator.h"

#include "dkmage/mode/CaveMode.h"
#include "dkmage/mode/MazeMode.h"
#include "dkmage/mode/HeroFortressMode.h"

#include "utils/GlobalScope.h"


using namespace dkmage;


GLOBAL_SCOPE() {
    Generator::registerMode< mode::CaveMode >( "cave" );
    Generator::registerMode< mode::MazeMode >( "maze" );
    Generator::registerMode< mode::HeroFortressMode >( "herofortress" );
}
