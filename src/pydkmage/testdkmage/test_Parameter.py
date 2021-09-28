##
##
##

import unittest

import dkmage.parameter as parameter
#import adiktedpp.rand as rand
#import adiktedpp.raw as raw


class ParametersMapTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_ParametersMap_getSizeT_set(self):
        paramsMap = parameter.ParametersMap()
        
        entry = "xxx"
        paramsMap.add( entry, 222 )
        value = paramsMap.getSizeT( entry, 111 )
        self.assertEqual( value, 222 )

    def test_ParametersMap_getSizeT_set(self):
        paramsMap = parameter.ParametersMap()
        
        entry = "xxx"
        value = paramsMap.getSizeT( entry, 111 )
        self.assertEqual( value, 111 )


if __name__ == '__main__':
    unittest.main()
