##
##
##

import unittest

import utils.log as log


class PrinterSink( log.LogSink ):

    def __init__( self ):
        super().__init__()
        self.callCounter = 0
          
    def printMessage( self, level, file, lineNo, message ):
        self.callCounter += 1 


class LogTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_setSink(self):
        sink = PrinterSink()
        self.assertEqual( sink.callCounter, 0 )
        sink.printMessage( log.LogLevel_LL_INFO, "xxx.py", 111, "asdf" )
        self.assertEqual( sink.callCounter, 1 )
        log.LogConfig.setLogSink( sink )
        log.log_info( "test log" )
        self.assertEqual( sink.callCounter, 2 )


if __name__ == '__main__':
    unittest.main()
