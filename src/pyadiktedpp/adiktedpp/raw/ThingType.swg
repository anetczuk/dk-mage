%module thingtype

%{
#include "adiktedpp/raw/ThingType.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( ThingTypeVector ) std::vector< enum adiktedpp::raw::ThingType >;
%template( ThingTypeSet ) std::set< enum adiktedpp::raw::ThingType >;             /// REQUIRES instantiation of std::vector !!!


%pythoncode %{
    def ThingType_name( enumVal ):
        return ThingTypeName( enumVal )
%}


%include "adiktedpp/raw/ThingType.h"
