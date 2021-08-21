/*
 * Rect.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_RECT_H_
#define UTILS_INCLUDE_UTILS_RECT_H_

#include <vector>
#include <set>
#include <ostream>


namespace utils {

    /**
     *
     */
    struct Point {
        int x;
        int y;

        Point(): x(0), y(0) {
        }
        Point( const int ax, const int ay ): x(ax), y(ay) {
        }

        bool operator==( const Point& data ) const {
            return ( x == data.x && y == data.y );
        }

        bool operator<( const Point& data ) const {
            if ( x < data.x ) {
                return true;
            }
            if ( x > data.x ) {
                return false;
            }
            return ( y < data.y );
        }

        Point operator-() const {
            Point newPoint = *this;
            newPoint.x *= -1;
            newPoint.y *= -1;
            return newPoint;
        }

        Point operator+( const Point& data ) const {
            Point newPoint = *this;
            newPoint.x += data.x;
            newPoint.y += data.y;
            return newPoint;
        }
        Point operator-( const Point& data ) const {
            Point newPoint = *this;
            newPoint.x -= data.x;
            newPoint.y -= data.y;
            return newPoint;
        }

        Point operator*( const int factor ) const {
            Point newPoint = *this;
            newPoint.x *= factor;
            newPoint.y *= factor;
            return newPoint;
        }

        Point operator/( const int factor ) const {
            Point newPoint = *this;
            newPoint.x /= factor;
            newPoint.y /= factor;
            return newPoint;
        }

        Point& operator+=( const Point& data ) {
            x += data.x;
            y += data.y;
            return *this;
        }
        Point& operator-=( const Point& data ) {
            x -= data.x;
            y -= data.y;
            return *this;
        }

        std::size_t length() const {
            return std::abs(x) + std::abs(y);
        }

        /// ===============================================================

        Point dir() const;

        Point diff( const Point& point ) const;

        std::size_t distanceManhattan( const Point point ) const;

        std::size_t distanceChebyshev( const Point point ) const;

        Point swapped() const {
            return Point( y, x );
        }

    };

    using PointList = std::vector< Point >;

    using PointSet = std::set< Point >;

    inline std::ostream& operator<<( std::ostream& os, const Point& data ) {
        os << "[" << data.x << " " << data.y << "]";
        return os;
    }


    /**
     *
     */
    struct Rect {
        Point min;                                  /// inclusive in range
        Point max;                                  /// inclusive in range

        /// empty rect
        Rect(): min(0,0), max(-1,-1) {
        }

        Rect( const std::size_t aWidth, const std::size_t aHeight ): min(), max(aWidth - 1, aHeight - 1) {
        }

        Rect( const Point& center, const std::size_t aWidth, const std::size_t aHeight ): min(), max(aWidth - 1, aHeight - 1) {
            centerize();
            move( center );
        }

        Rect( const Point& aMin, const Point& aMax ): min(aMin), max(aMax) {
        }

        Rect( const int minX, const int minY, const int maxX, const int maxY ): min(minX, minY), max(maxX, maxY) {
        }

        bool valid() const {
            if ( width() < 1 ) {
                return false;
            }
            if ( height() < 1 ) {
                return false;
            }
            return true;
        }

        int width() const {
            return ( max.x - min.x + 1);
        }
        int height() const {
            return ( max.y - min.y + 1);
        }
        int area() const {
            return width() * height();
        }
        Point vector() const {
            return Point( width() -1, height() -1 );
        }

        Point pointByIndex( const std::size_t pointIndex ) const {
            const int rectWidth = width();
            if ( rectWidth == 0 ) {
                return center();
            }
            const int dx = pointIndex % rectWidth;
            const int dy = pointIndex / rectWidth;
            return min + Point( dx, dy );
        }

        /// ===================================================

        Point leftTop() const {
            return Point( min.x, max.y );
        }

        Point leftCenter( const std::size_t delta = 0 ) const {
            const int cx = min.x - delta;
            const int cy = min.y + height() / 2;
            return Point( cx, cy );
        }

        Point leftBottom() const {
            return Point( min.x, min.y );
        }

        Point centerTop( const std::size_t delta = 0 ) const {
            const int cx = min.x + width() / 2;
            const int cy = min.y - delta;
            return Point( cx, cy );
        }

        Point center() const {
            const int cx = min.x + width() / 2;
            const int cy = min.y + height() / 2;
            return Point( cx, cy );
        }

        Point centerBottom( const std::size_t delta = 0 ) const {
            const int cx = min.x + width() / 2;
            const int cy = max.y + delta;
            return Point( cx, cy );
        }

        Point rightTop() const {
            return Point( max.x, max.y );
        }

        Point rightCenter( const std::size_t delta = 0 ) const {
            const int cx = max.x + delta;
            const int cy = min.y + height() / 2;
            return Point( cx, cy );
        }

        Point rightBottom() const {
            return Point( max.x, min.y );
        }

        /// ===================================================

        void move( const int offsetX, const int offsetY ) {
            min.x += offsetX;
            min.y += offsetY;
            max.x += offsetX;
            max.y += offsetY;
        }

        void move( const Point offset ) {
            move( offset.x, offset.y );
        }

        Rect moved( const Point offset ) const {
            return moved( offset.x, offset.y );
        }

        Rect moved( const int offsetX, const int offsetY ) const {
            Rect r = *this;
            r.move( offsetX, offsetY );
            return r;
        }

        void moveRightTo( const int xCoord ) {
            const int xOffset = xCoord - max.x;
            move( xOffset, 0 );
        }

        void moveLeftTo( const int xCoord ) {
            const int xOffset = xCoord - min.x;
            move( xOffset, 0 );
        }

        void moveBottomTo( const int yCoord ) {
            const int yOffset = yCoord - max.y;
            move( 0, yOffset );
        }

        /// move center of rect to (0,0) point
        void centerize() {
            const Point center = (max + min) / 2;
            move( -center.x, -center.y );
        }

        /// ===================================================

        void normalize() {
            const int w = width();
            const int h = height();
            *this = Rect( w, h );
        }

        void expand( const Rect& rect ) {
            min.x = std::min( min.x, rect.min.x );
            min.y = std::min( min.y, rect.min.y );
            max.x = std::max( max.x, rect.max.x );
            max.y = std::max( max.y, rect.max.y );
        }

        void grow( const int delta ) {
            min.x -= delta;
            min.y -= delta;
            max.x += delta;
            max.y += delta;
        }

        void growWidth( const int delta ) {
            min.x -= delta;
            max.x += delta;
        }

        bool isInside( const int minCoord, const int maxCoord ) const {
            if ( min.x < minCoord || min.x > maxCoord ) {
                return false;
            }
            if ( min.y < minCoord || min.y > maxCoord ) {
                return false;
            }
            if ( max.x < minCoord || max.x > maxCoord ) {
                return false;
            }
            if ( max.y < minCoord || max.y > maxCoord ) {
                return false;
            }
            return true;
        }

        bool isInside( const Point point ) const {
            if ( point.x < min.x || point.x > max.x ) {
                return false;
            }
            if ( point.y < min.y || point.y > max.y ) {
                return false;
            }
            return true;
        }

        /// is whole 'rect' inside rectangle?
        bool hasInside( const Rect& rect ) const {
            if ( isInside( rect.min ) == false ) {
                return false;
            }
            if ( isInside( rect.max ) == false ) {
                return false;
            }
            return true;
        }

        /// is every of 'points' inside rectangle?
        bool hasInside( const std::set< Point >& points ) const;

        bool isCollision( const Rect& rect ) const {
            if ( isSmallerThan(rect) ) {
                return false;
            }
            if ( rect.isSmallerThan( *this ) ) {
                return false;
            }
            return true;
        }

        Point distance( const Rect& rect ) const {
            const Point p1 = center();
            const Point p2 = rect.center();
            const Point pdist = p1.diff( p2 );
            const int wCommon = ( width()  + rect.width()  ) / 2;
            const int hCommon = ( height() + rect.height() ) / 2;

            int xdistance = 0;
            int ydistance = 0;
            if ( pdist.x >= wCommon ) {
                xdistance = pdist.x - wCommon + 1;
            }
            if ( pdist.y > hCommon ) {
                ydistance = pdist.y - hCommon + 1;
            }
            return Point( xdistance, ydistance );
        }

        std::size_t distanceChebyshev( const Point point ) const;

        /// ================================================

        static Rect empty() {
            Rect rect;
            rect.max.x = -1;
            rect.max.y = -1;
            return rect;
        }


    private:

        bool isSmallerThan( const Rect& rect ) const {
            if ( max.x < rect.min.x ) {
                return true;
            }
            if ( max.y < rect.min.y ) {
                return true;
            }
            return false;
        }

    };

    inline std::ostream& operator<<( std::ostream& os, const Rect& data ) {
        os << "{" << data.min << " " << data.max << "}";
        return os;
    }


    ///=============================================================================================


    PointList line( const Point& from, const Point& to );

    PointSet outline( const Rect& rect, const int offset = 0 );

    bool is_in_radius( const PointList& points, const Point& point, const std::size_t radius );

    /**
     * Is 'rect' in radius from 'points' vector?
     * Radius 0 means 'rect' contains any of points. Radius 1 means 'rect' is next to any of points.
     */
    bool is_in_radius( const PointList& points, const Rect& rect, const std::size_t radius );

    bool is_collision( const PointList& points, const Rect& rect );

    bool is_collision( const PointList& pointsA, const PointList& pointsB );

} /* namespace utils */

#endif /* UTILS_INCLUDE_UTILS_RECT_H_ */
