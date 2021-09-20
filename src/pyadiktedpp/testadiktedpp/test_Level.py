##
##
##

import unittest

import adiktedpp.level as level

import adiktedpp.type as type
import adiktedpp.raw as raw
import utils.rect


class LevelTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_countSolid_empty(self):
        data = level.Level()
        fullRect = raw.RawLevel.mapSize()
        self.assertEqual( data.countSolid( fullRect ), 7225 )

    def test_getSlab_empty(self):
        data = level.Level()
        point = utils.rect.Point(10, 10)
        slab = data.getSlab( point )
        self.assertEqual( slab, 0 )
        self.assertEqual( type.Slab_name( slab ), "S_ROCK" )

    def test_setRoom(self):
        data = level.Level()
        roomRect = utils.rect.Rect(10, 10, 20, 20)
        data.setRoom( roomRect, type.Room_R_TREASURE )
        
        rawData = data.getRawLevel()
        rawSlab = rawData.getSlab( 15, 15 )
        self.assertEqual( rawSlab, raw.SlabType_ST_TREASURE )


if __name__ == '__main__':
    unittest.main()
