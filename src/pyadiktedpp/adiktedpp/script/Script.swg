%module script

%{
#include "adiktedpp/script/Script.h"
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%template( StringVector ) std::vector< std::string >;


%import "adiktedpp/Type.h"
%import "adiktedpp/Level.h"

%include "adiktedpp/script/Script.h"
