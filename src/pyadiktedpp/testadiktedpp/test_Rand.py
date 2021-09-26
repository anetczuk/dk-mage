##
##
##

import unittest

import adiktedpp.rand as rand


class RandTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_rng_srand(self):
        rand.rng_srand( 111 )
        num = rand.rng_randi()
        self.assertEqual( num, 1211922123 )


if __name__ == '__main__':
    unittest.main()
