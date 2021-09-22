%module level

/// %extern "adiktedpp/Type.h"

%{
#include "adiktedpp/Level.h"

using namespace adiktedpp;
%}


%pythoncode %{####################################################%}


%import(module="utils.rect") "utils/Rect.swg"


%import "adiktedpp/Type.h"
%import "adiktedpp/raw/RawLevel.h"

%include "adiktedpp/Level.h"
