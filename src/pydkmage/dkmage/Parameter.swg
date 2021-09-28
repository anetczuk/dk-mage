%module parameter

%{
#include "dkmage/Parameter.h"
%}


%pythoncode %{####################################################%}


%include "std_string.i"
%include "stdint.i"         /// for std::uint32_t


/// prevent unsupported nested class warning
#pragma SWIG nowarn=362


%import(module="dkmage.parametername") "ParameterName.swg"


%include "dkmage/Parameter.h"
