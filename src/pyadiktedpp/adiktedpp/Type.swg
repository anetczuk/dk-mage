%module type

%{
#include "adiktedpp/Type.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%define ENUM_DEFS( enumName )
%template( enumName ## Vector ) std::vector< enum adiktedpp:: ## enumName >;
%template( enumName ## Set ) std::set< enum adiktedpp:: ## enumName >;             /// REQUIRES instantiation of std::vector !!!
%enddef


%define ENUM_TYPES( enumName )
%template( enumName ## Vector ) std::vector< enum adiktedpp:: ## enumName >;
%template( enumName ## Set ) std::set< enum adiktedpp:: ## enumName >;             /// REQUIRES instantiation of std::vector !!!

%pythoncode %{
    def enumName ## _name( enumVal ):
        return enumName ## Name( enumVal )
%}
%enddef


## prevent Warning 509
%ignore adiktedpp::operator<<;


ENUM_TYPES( Slab )
ENUM_DEFS( Item )
ENUM_TYPES( Room )
ENUM_DEFS( Trap )
ENUM_DEFS( Door )
ENUM_DEFS( Spell )
ENUM_DEFS( Creature )
ENUM_DEFS( Player )


%include "adiktedpp/Type.h"
