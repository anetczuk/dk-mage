%module script

%{
#include "adiktedpp/script/Script.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( StringVector ) std::vector< std::string >;


## prevent Warning 509
%ignore adiktedpp::script::script_keyword;
%ignore adiktedpp::script::Script::operator[];


%import "adiktedpp/Type.h"
%import "adiktedpp/Level.h"

%include "adiktedpp/script/Script.h"
