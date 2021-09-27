%module rand

%{
#include "adiktedpp/Rand.h"
%}


%include "std_string.i"
%include "stdint.i"         /// for std::uint32_t


%pythoncode %{####################################################%}


%import "adiktedpp/raw/RNG.h"
%include "adiktedpp/Rand.h"
