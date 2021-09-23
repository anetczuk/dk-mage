%module availablemode

%{
#include "adiktedpp/script/AvailableMode.h"

using namespace adiktedpp::script;
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%define ENUM_DEFS( enumName )
%template( enumName ## Vector ) std::vector< enum adiktedpp::script:: ## enumName >;
%template( enumName ## Set ) std::set< enum adiktedpp::script:: ## enumName >;             /// REQUIRES instantiation of std::vector !!!
%enddef


ENUM_DEFS( AvailableMode )
ENUM_DEFS( RoomAvailableMode )
ENUM_DEFS( MagicAvailableMode )
ENUM_DEFS( AvailableCommand )


%pythoncode %{
    def AvailableCommand_name( enumVal ):
        return AvailableCommandName( enumVal )
%}


%include "adiktedpp/script/AvailableMode.h"
