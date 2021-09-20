##
##
##

import unittest

from adiktedpp import script
from adiktedpp import type
import utils.rect as rect


class AvailableCommandTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_AvailableCommand_name(self):
        self.assertEqual( script.AvailableCommand_name( 2 ), "DOOR_AVAILABLE" )
        self.assertEqual( script.AvailableCommand_name( script.AvailableCommand_AC_DOOR_AVAILABLE ), "DOOR_AVAILABLE" )


class ScriptTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_countTunnellerTriggers_empty(self):
        data = script.Script()
        self.assertEqual( data.countTunnellerTriggers(), 0 )

    def test_countTunnellerTriggers(self):
        data = script.Script()
        section = data.mainSection()
        section.ADD_TUNNELLER_PARTY_TO_LEVEL( type.Player_P_P1, "tunneler_1", 1, 1, 3, 500 )
        self.assertEqual( data.countTunnellerTriggers(), 1 )

    def test_build(self):
        data = script.Script()
        section = data.mainSection()
        point = rect.Point(10, 20)
        section.IF_SLAB_TYPE( point, type.Slab_S_GOLD )
        section.REM( "xxx" )
        section.ENDIF()
        content = data.build()
        self.assertEqual( len( content ), 6 )
        self.assertEqual( content[0], "" )
        self.assertEqual( content[1], "" )
        self.assertEqual( content[2], "REM ===============  MAIN  ================" )
        self.assertEqual( content[3], "IF_SLAB_TYPE( 10, 20, GOLD )" )
        self.assertEqual( content[4], "    rem xxx" )
        self.assertEqual( content[5], "ENDIF" )


if __name__ == '__main__':
    unittest.main()
