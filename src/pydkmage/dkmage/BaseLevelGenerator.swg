%module(directors="1") baselevelgenerator

%{
#include "dkmage/BaseLevelGenerator.h"
%}


%pythoncode %{####################################################%}


/// prevent "Returning a pointer or reference in a director method is not recommended."
#pragma SWIG nowarn=473


%import(module="adiktedpp.level") "adiktedpp/Level.swg"

%import(module="adiktedpp.gamemap") "adiktedpp/GameMap.swg"

%import(module="dkmage.levelgenerator") "LevelGenerator.swg"


/// allow overriding C++ methods with Python methods
%feature("director") dkmage::BaseLevelGenerator;


%include "dkmage/BaseLevelGenerator.h"
