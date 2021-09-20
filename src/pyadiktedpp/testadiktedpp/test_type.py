##
##
##

import unittest

from adiktedpp import type


class SlabTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_Slab_name(self):
        self.assertEqual( type.Slab_name( 2 ), "S_GEMS" )
        self.assertEqual( type.Slab_name( type.Slab_S_GEMS ), "S_GEMS" )


class RoomTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_Room_name(self):
        self.assertEqual( type.Room_name( 2 ), "R_PORTAL" )
        self.assertEqual( type.Room_name( type.Room_R_TREASURE ), "R_TREASURE" )

    def test_RoomsRegular_len(self):
        rooms = type.RoomsRegular()
        self.assertEqual( len(rooms), 14 )

    def test_RoomsRegular_in(self):
        rooms = type.RoomsRegular()
        self.assertIn( type.Room_R_GRAVEYARD, rooms )

    def test_RoomsRegular_array(self):
        rooms = type.RoomsRegular()
        item = rooms[0]
        self.assertEqual( item , 3 )
        self.assertEqual( type.Room_name( item ), "R_TREASURE" )


if __name__ == '__main__':
    unittest.main()
