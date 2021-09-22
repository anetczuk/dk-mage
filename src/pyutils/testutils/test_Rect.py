##
##
##

import unittest

import utils.rect


class PointTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_constructor(self):
        point = utils.rect.Point(1, 2)
        self.assertEqual( point.x, 1 )
        self.assertEqual( point.y, 2 )

    def test_str(self):
        point = utils.rect.Point(1, 2)
        pointString = str( point )
        self.assertEqual( pointString, "[1 2]" )

    def test_repr(self):
        point = utils.rect.Point(1, 2)
        pointString = repr( point )
        self.assertEqual( pointString, "utils::Point[1 2]" )


class PointListTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_str(self):
        pointsList = utils.rect.PointList()
        pointsList.append( utils.rect.Point(1, 2) )
        pointsList.append( utils.rect.Point(2, 2) )
        self.assertEqual( str( pointsList ), "[utils::Point[1 2], utils::Point[2 2]]" )

    def test_repr(self):
        pointsList = utils.rect.PointList()
        pointsList.append( utils.rect.Point(1, 2) )
        pointsList.append( utils.rect.Point(2, 2) )
        self.assertEqual( repr( pointsList ), "[utils::Point[1 2], utils::Point[2 2]]" )

    def test_in(self):
        pointsList = utils.rect.PointList()
        pointsList.append( utils.rect.Point(1, 2) )
        pointsList.append( utils.rect.Point(2, 2) )
        self.assertIn( utils.rect.Point(2, 2), pointsList )


class RectTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_str(self):
        rect = utils.rect.Rect(5, 7)
        rectString = str( rect )
        self.assertEqual( rectString, "{[0 0] [4 6]}" )

    def test_repr(self):
        rect = utils.rect.Rect(5, 7)
        rectString = repr( rect )
        self.assertEqual( rectString, "utils::Rect{[0 0] [4 6]}" )


class FreeFunctionsTest(unittest.TestCase):
    def setUp(self):
        ## called before testfunction is executed
        pass

    def tearDown(self):
        ## called after testfunction was executed
        pass

    def test_line(self):
        fromPoint = utils.rect.Point(10, 10)
        toPoint   = utils.rect.Point(10, 14)
        lineList  = utils.rect.line( fromPoint, toPoint )
        self.assertEqual( len(lineList), 5 )
        self.assertEqual( lineList[0], utils.rect.Point(10, 10) )
        self.assertEqual( lineList[1], utils.rect.Point(10, 11) )
        self.assertEqual( lineList[2], utils.rect.Point(10, 12) )
        self.assertEqual( lineList[3], utils.rect.Point(10, 13) )
        self.assertEqual( lineList[4], utils.rect.Point(10, 14) )

    def test_is_in_radius(self):
        lineList = []
        lineList.append( utils.rect.Point(11, 10) )
        lineList.append( utils.rect.Point(12, 10) )
        lineList.append( utils.rect.Point(13, 10) )
        
        centerPoint = utils.rect.Point(12, 12)
        inRadius = utils.rect.is_in_radius( lineList, centerPoint, 5 )
        self.assertTrue( inRadius, 5 )


if __name__ == '__main__':
    unittest.main()
