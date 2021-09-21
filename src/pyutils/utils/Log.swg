%module log

%{
#include "utils/Log.h"
%}


%include "std_string.i"


%pythoncode %{####################################################%}


%pythoncode %{

    import inspect

    def log_info( message ):
        caller = inspect.getframeinfo( inspect.stack()[1][0] )
        loggerObj = Logger( "INFO ", caller.filename, caller.lineno )
        loggerObj.printMessage( message )

    def log_warn( message ):
        caller = inspect.getframeinfo( inspect.stack()[1][0] )
        loggerObj = Logger( "WARN ", caller.filename, caller.lineno )
        loggerObj.printMessage( message )

    def log_err( message ):
        caller = inspect.getframeinfo( inspect.stack()[1][0] )
        loggerObj = Logger( "ERR  ", caller.filename, caller.lineno )
        loggerObj.printMessage( message )

%}


%include "utils/Log.h"
