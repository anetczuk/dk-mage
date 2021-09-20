%module slabtype

%{
#include "adiktedpp/raw/SlabType.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( SlabTypeVector ) std::vector< enum adiktedpp::raw::SlabType >;
%template( SlabTypeSet ) std::set< enum adiktedpp::raw::SlabType >;             /// REQUIRES instantiation of std::vector !!!


%pythoncode %{
    def SlabType_name( enumVal ):
        return SlabTypeName( enumVal )
%}


%include "adiktedpp/raw/SlabType.h"
