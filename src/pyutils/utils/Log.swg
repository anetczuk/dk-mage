%module(directors="1") log

%{
#include "utils/Log.h"
%}


%include "std_string.i"


%pythoncode %{####################################################%}


%ignore utils::Logger;


/// allow overriding C++ methods with Python methods
%feature("director") utils::LogSink;


%pythoncode %{

    import inspect

    def log_info( message ):
        caller = inspect.getframeinfo( inspect.stack()[1][0] )
        print_message( LogLevel_LL_INFO, caller.filename, caller.lineno, message )

    def log_warn( message ):
        caller = inspect.getframeinfo( inspect.stack()[1][0] )
        print_message( LogLevel_LL_WARN, caller.filename, caller.lineno, message )

    def log_err( message ):
        caller = inspect.getframeinfo( inspect.stack()[1][0] )
        print_message( LogLevel_LL_ERR, caller.filename, caller.lineno, message )

%}


%include "utils/Log.h"
