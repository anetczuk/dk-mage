/*
 * Rect.cpp
 *
 */

#include "utils/Rect.h"


namespace utils {

    Point Point::dir() const {
        Point ret;
        if ( x > 0 ) {
            ret.x = 1;
        } else if ( x < 0 ) {
            ret.x = -1;
        }

        if ( y > 0 ) {
            ret.y = 1;
        } else if ( y < 0 ) {
            ret.y = -1;
        }

        return ret;
    }

    Point Point::diff( const Point& point ) const {
        const int xDiff = std::abs(x - point.x);
        const int yDiff = std::abs(y - point.y);
        return Point(xDiff, yDiff);
    }

    std::size_t Point::distanceManhattan( const Point point ) const {
        const int xDiff = std::abs(x - point.x);
        const int yDiff = std::abs(y - point.y);
        return xDiff + yDiff;
    }

    std::size_t Point::distanceChebyshev( const Point point ) const {
        const int xDiff = std::abs(x - point.x);
        const int yDiff = std::abs(y - point.y);
        return std::max(xDiff, yDiff);
    }

    /// ==============================================================

    bool Rect::hasInside( const std::set< Point >& points ) const {
        for ( const Point item: points ) {
            if ( isInside( item ) == false ) {
                return false;
            }
        }
        return true;
    }

    std::size_t Rect::distanceChebyshev( const Point point ) const {
        std::size_t distanceX = 0;
        const int diffMinX = min.x - point.x;
        if ( diffMinX > 0 ) {
            distanceX = diffMinX;
        } else {
            const int diffMaxX = point.x - max.x;
            if ( diffMaxX > 0 ) {
                distanceX = diffMaxX;
            }
        }

        std::size_t distanceY = 0;
        const int diffMinY = min.y - point.y;
        if ( diffMinY > 0 ) {
            distanceY = diffMinY;
        } else {
            const int diffMaxY = point.y - max.y;
            if ( diffMaxY > 0 ) {
                distanceY = diffMaxY;
            }
        }

        return std::max( distanceX, distanceY );
    }

    /// ==============================================================

    /// implementation taken and adapted from: https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)
    PointList line_dda( const Point& from, const Point& to ) {
        PointList ret;

        const int x1 = from.x;
        const int y1 = from.y;
        const int x2 = to.x;
        const int y2 = to.y;
        double dx = (x2 - x1);
        double dy = (y2 - y1);
        double step = 0;
        if ( std::abs(dx) >= std::abs(dy) )
            step = std::abs(dx);
        else
            step = std::abs(dy);

        dx = dx / step;
        dy = dy / step;
        double x = x1;
        double y = y1;
        double i = 1;
        while (i <= step) {
            ret.push_back( Point(x,y) );
            x = x + dx;
            y = y + dy;
            i = i + 1;
        }
        ret.push_back( to );

        return ret;
    }

    /// implementation taken and adapted from: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    PointList line_bresenham( const Point& from, const Point& to ) {
        PointList ret;

        int x0 = from.x;
        int y0 = from.y;
        int x1 = to.x;
        int y1 = to.y;

        const int dx = std::abs( x1 - x0 );
        const int sx = ( x0 < x1 ) ? 1 : -1;
        const int dy = -std::abs( y1 - y0 );
        const int sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;                          /// error value e_xy
        while (true) {
            ret.push_back( Point(x0,y0) );
            if (x0 == x1 && y0 == y1)
                break;
            int e2 = 2 * err;
            const int diffY = e2 - dy;
            const int diffX = dx - e2;
            if ( diffY > diffX ) {
                if (diffY >= 0) {                         /// e_xy+e_x > 0
                    err += dy;
                    x0 += sx;
                } else if (diffX >=0) {                         /// e_xy+e_y < 0
                    err += dx;
                    y0 += sy;
                }
            } else {
                if (diffX >=0) {                         /// e_xy+e_y < 0
                    err += dx;
                    y0 += sy;
                } else if (diffY >= 0) {                         /// e_xy+e_x > 0
                    err += dy;
                    x0 += sx;
                }
            }

//            if (e2 >= dy) {                         /// e_xy+e_x > 0
//                err += dy;
//                x0 += sx;
//            }
//            if (e2 <= dx) {                         /// e_xy+e_y < 0
//                err += dx;
//                y0 += sy;
//            }
        }

        return ret;
    }

    PointList line( const Point& from, const Point& to ) {
        return line_bresenham( from, to );
//        return line_dda( from, to );
    }

    PointSet outline( const Rect& rect, const int offset ) {
        PointSet ret;
        for ( int y = rect.min.y - offset; y<= rect.max.y + offset; ++y ) {
            ret.insert( Point(rect.min.x - offset, y) );
            ret.insert( Point(rect.max.x + offset, y) );
        }
        for ( int x = rect.min.x - offset; x<= rect.max.x + offset; ++x ) {
            ret.insert( Point(x, rect.min.y - offset) );
            ret.insert( Point(x, rect.max.y + offset) );
        }
        return ret;
    }

    bool is_in_radius( const PointList& points, const Point& point, const std::size_t radius ) {
        const std::size_t pSize = points.size();
        for ( std::size_t i=0; i<pSize; ++i ) {
            const std::size_t dist = point.distanceChebyshev( points[i] );
            if ( dist <= radius ) {
                return true;
            }
        }
        return false;
    }

    bool is_in_radius( const PointList& points, const Rect& rect, const std::size_t radius ) {
        const std::size_t pSize = points.size();
        for ( std::size_t i=0; i<pSize; ++i ) {
            const std::size_t dist = rect.distanceChebyshev( points[i] );
            if ( dist <= radius ) {
                return true;
            }
        }
        return false;
    }

    bool is_collision( const PointList& points, const Rect& rect ) {
        const std::size_t pSize = points.size();
        for ( std::size_t i=0; i<pSize; ++i ) {
            if ( rect.isInside( points[i] ) ) {
                return true;
            }
        }
        return false;
    }

    bool is_collision( const PointList& pointsA, const PointList& pointsB ) {
        const std::size_t pSize = pointsA.size();
        for ( std::size_t i=0; i<pSize; ++i ) {
            if ( is_in_radius( pointsB, pointsA[i], 1 ) ) {
                return true;
            }
        }
        return false;
    }

} /* namespace utils */
