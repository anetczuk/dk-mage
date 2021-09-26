%module(directors="1") rng

%{
#include "adiktedpp/raw/RNG.h"
%}


%pythoncode %{####################################################%}


/// allow overriding C++ methods with Python methods
%feature("director") adiktedpp::raw::AbstractRNG;
%feature("director") adiktedpp::raw::BaseRNG;


%include "adiktedpp/raw/RNG.h"
