%module subtypedoor

%{
#include "adiktedpp/raw/SubTypeDoor.h"
%}

%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( SubTypeDoorVector ) std::vector< enum adiktedpp::raw::SubTypeDoor >;
%template( SubTypeDoorSet ) std::set< enum adiktedpp::raw::SubTypeDoor >;             /// REQUIRES instantiation of std::vector !!!


%include "adiktedpp/raw/SubTypeDoor.h"
