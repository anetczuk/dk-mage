%module subtypetrap

%{
#include "adiktedpp/raw/SubTypeTrap.h"
%}

%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( SubTypeTrapVector ) std::vector< enum adiktedpp::raw::SubTypeTrap >;
%template( SubTypeTrapSet ) std::set< enum adiktedpp::raw::SubTypeTrap >;             /// REQUIRES instantiation of std::vector !!!


%include "adiktedpp/raw/SubTypeTrap.h"
