##
##
##

import unittest

import adiktedpp.version as version


class VersionTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_version(self):
        self.assertIn( "VERSION_FULL_STRING", dir( version ) )


if __name__ == '__main__':
    unittest.main()
