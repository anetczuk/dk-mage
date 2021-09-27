##
##
##

import unittest

import adiktedpp.rand as rand
import adiktedpp.raw as raw


class CustomRNG( raw.BaseRNG ):

    def __init__( self ):
        super().__init__()
        self.callCounter = 0
          
    def srand( self, seed ):
        ## do nothing
        pass

    def srandr( self ):
        ## do nothing
        pass

    def randi( self ):
        self.callCounter += 1
        return 111

    def randd( self ):
        self.callCounter += 1
        return 0.111


class RandTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        rand.rng_srandr()

    def test_rng_srand(self):
        rand.rng_srand( 111 )
        num = rand.rng_randi()
        self.assertEqual( num, 1211922123 )

    def test_rng_set_generator(self):
        customRNG = CustomRNG()
        rand.rng_set_generator( customRNG )
        
        num = rand.rng_randi()
        self.assertEqual( num, 111 )
        self.assertEqual( customRNG.callCounter, 1 )
        
        num = rand.rng_randd()
        self.assertEqual( num, 0.111 )
        self.assertEqual( customRNG.callCounter, 2 )


if __name__ == '__main__':
    unittest.main()
