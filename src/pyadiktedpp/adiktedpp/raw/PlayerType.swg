%module playertype

%{
#include "adiktedpp/raw/PlayerType.h"
%}

%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( PlayerTypeVector ) std::vector< enum adiktedpp::raw::PlayerType >;
%template( PlayerTypeSet ) std::set< enum adiktedpp::raw::PlayerType >;             /// REQUIRES instantiation of std::vector !!!


%include "adiktedpp/raw/PlayerType.h"
