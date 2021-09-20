%module subtypecreature

%{
#include "adiktedpp/raw/SubTypeCreature.h"
%}

%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( SubTypeCreatureVector ) std::vector< enum adiktedpp::raw::SubTypeCreature >;
%template( SubTypeCreatureSet ) std::set< enum adiktedpp::raw::SubTypeCreature >;             /// REQUIRES instantiation of std::vector !!!


%include "adiktedpp/raw/SubTypeCreature.h"
