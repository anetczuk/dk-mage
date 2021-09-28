%module levelgenerator

%{
#include "dkmage/LevelGenerator.h"
%}


%pythoncode %{####################################################%}


%import(module="dkmage.parameter") "Parameter.swg"


%include "dkmage/LevelGenerator.h"
