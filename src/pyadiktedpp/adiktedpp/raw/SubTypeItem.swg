%module subtypedoor

%{
#include "adiktedpp/raw/SubTypeItem.h"
%}

%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( SubTypeItemVector ) std::vector< enum adiktedpp::raw::SubTypeItem >;
%template( SubTypeItemSet ) std::set< enum adiktedpp::raw::SubTypeItem >;             /// REQUIRES instantiation of std::vector !!!


%include "adiktedpp/raw/SubTypeItem.h"
