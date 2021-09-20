##
##
##

import unittest

from adiktedpp import raw

import utils.rect


class SlabTypeTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_SlabType_name(self):
        self.assertEqual( raw.SlabType_name( 2 ), "ST_EARTH" )
        self.assertEqual( raw.SlabType_name( raw.SlabType_ST_EARTH ), "ST_EARTH" )

    def test_SlabRooms_len(self):
        rooms = raw.SlabRooms()
        self.assertEqual( len(rooms), 14 )

    def test_SlabRooms_in(self):
        rooms = raw.SlabRooms()
        self.assertIn( raw.SlabType_ST_GRAVEYARD, rooms )

    def test_SlabRooms_array(self):
        rooms = raw.SlabRooms()
        item = rooms[0]
        self.assertEqual( item , 16 )
        self.assertEqual( raw.SlabType_name( item ), "ST_TREASURE" )

    def test_isEarth(self):
        self.assertTrue( raw.isEarth( raw.SlabType_ST_EARTH ) )


class ThingTypeTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_ThingType_name(self):
        self.assertEqual( raw.ThingType_name( 2 ), "TT_SHOT" )
        self.assertEqual( raw.ThingType_name( raw.ThingType_TT_SHOT ), "TT_SHOT" )


class RawLevelTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_countSolid_new(self):
        rawLevel = raw.RawLevel()
        fullRect = raw.RawLevel.mapSize()
        self.assertEqual( rawLevel.countSolid( fullRect ), 7225 )

    def test_countAllCreatures_empty(self):
        rawLevel = raw.RawLevel()
        self.assertEqual( rawLevel.countAllCreatures(), 0 )

    def test_countAllCreatures(self):
        rawLevel = raw.RawLevel()
        point = utils.rect.Point(10, 10)
        rawLevel.setSlab( point, raw.SlabType_ST_PATH )
        rawLevel.setCreature( point, 4, raw.SubTypeCreature_STC_BILEDEMN )
        self.assertEqual( rawLevel.countAllCreatures(), 1 )

    def test_countActionPoints(self):
        rawLevel = raw.RawLevel()
        apIndex = rawLevel.addActionPoint( 10, 10, 4, 3 )
        self.assertEqual( apIndex, 1 )
        self.assertEqual( rawLevel.countActionPoints(), 1 )


if __name__ == '__main__':
    unittest.main()
