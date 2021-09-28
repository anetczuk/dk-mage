%module rawlevel

%{
#include "adiktedpp/raw/RawLevel.h"
%}


%pythoncode %{####################################################%}


/// prevent unsupported nested class warning
#pragma SWIG nowarn=362


%include "std_string.i"


%include "adiktedpp/raw/RawLevel.h"
