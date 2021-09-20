%module level

/// %extern "adiktedpp/Type.h"

%{
#include "adiktedpp/raw/RawLevel.h"
#include "adiktedpp/Level.h"

using namespace adiktedpp;
%}


%pythoncode %{####################################################%}


%include "std_string.i"


%import "adiktedpp/Type.h"
%import "adiktedpp/raw/RawLevel.h"

%include "adiktedpp/Level.h"
