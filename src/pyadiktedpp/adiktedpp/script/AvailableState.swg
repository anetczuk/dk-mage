%module availablestate

%{
#include "adiktedpp/script/AvailableState.h"
%}

%include "std_string.i"


%pythoncode %{####################################################%}


/// prevent unsupported nested class warning
#pragma SWIG nowarn=325

%ignore adiktedpp::Map2;


%import "adiktedpp/Type.h"
%include "adiktedpp/script/AvailableStateBase.h"


%template( WorkshopAvailableDoor ) adiktedpp::script::WorkshopAvailableState< enum adiktedpp::Door >;
%template( WorkshopAvailableTrap ) adiktedpp::script::WorkshopAvailableState< enum adiktedpp::Trap >;


%include "adiktedpp/script/AvailableState.h"
