%module loggingtools


%pythoncode %{

import sys
import logging

import utils.log as log


def configure_console( logLevel=None ):
    if logLevel is None:
        logLevel = logging.DEBUG

    consoleHandler = logging.StreamHandler( stream=sys.stdout )

    formatter = create_formatter()

    consoleHandler.setFormatter( formatter )

    logging.root.addHandler( consoleHandler )
    logging.root.setLevel( logLevel )


def create_formatter(loggerFormat=None):
    if loggerFormat is None:
        ## loggerFormat = '%(asctime)s,%(msecs)-3d %(levelname)-8s %(threadName)s [%(filename)s:%(lineno)d] %(message)s'
        loggerFormat = ('%(asctime)s,%(msecs)-3d %(levelname)-8s %(threadName)s  %(name)s:%(funcName)s '
                        '[%(filename)s:%(lineno)d] %(message)s')
    dateFormat = '%Y-%m-%d %H:%M:%S'
    return EmptyLineFormatter( loggerFormat, dateFormat )
    ## return logging.Formatter( loggerFormat, dateFormat )


class EmptyLineFormatter(logging.Formatter):
    """Special formatter storing empty lines without formatting."""

    ## override base class method
    def format(self, record):
        msg = record.getMessage()
        clearMsg = msg.replace('\n', '')
        clearMsg = clearMsg.replace('\r', '')
        if not clearMsg:
            # empty
            return msg
        return super().format( record )


class LoggingSink( log.LogSink ):

    def __init__( self ):
        super().__init__()
        
        consoleHandler = logging.StreamHandler( stream=sys.stdout )
        loggerFormat = ('%(asctime)s,%(msecs)-3d %(levelname)-8s %(threadName)s  %(message)s')
        formatter = create_formatter( loggerFormat )
        consoleHandler.setFormatter( formatter )
    
        self.logger = logging.getLogger( "pyadiktedpp" )
        self.logger.addHandler( consoleHandler )
        self.logger.setLevel( logging.DEBUG )
          
    def printMessage( self, level, file, lineNo, message ):
        prefix = self.logPrefix( file, lineNo )

        if level == log.LogLevel_LL_INFO:
            self.logger.info( "%s: %s", prefix, message )
        elif level == log.LogLevel_LL_WARN:
            self.logger.warning( "%s: %s", prefix, message )
        elif level == log.LogLevel_LL_ERR:
            self.logger.error( "%s: %s", prefix, message )
        else:
            self.logger.info( "%s: %s", prefix, message )

%}
