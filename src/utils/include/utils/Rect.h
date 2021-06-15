/*
 * Rect.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_RECT_H_
#define UTILS_INCLUDE_UTILS_RECT_H_

#include <vector>
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
            return ( y < data.y );
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

        Point diff( const Point& point ) const;

        std::size_t distance( const Point& point ) const;

    };

    inline std::ostream& operator<<( std::ostream& os, const Point& data ) {
        os << "[" << data.x << " " << data.y << "]";
        return os;
    }


    /**
     *
     */
    struct Rect {
        Point min;
        Point max;

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

        Point center() const {
            const int cx = min.x + width() / 2;
            const int cy = min.y + height() / 2;
            return Point( cx, cy );
        }

        void move( const int offsetX, const int offsetY ) {
            min.x += offsetX;
            min.y += offsetY;
            max.x += offsetX;
            max.y += offsetY;
        }

        Rect moved( const Point offset ) const {
            return moved( offset.x, offset.y );
        }

        Rect moved( const int offsetX, const int offsetY ) const {
            Rect r = *this;
            r.move( offsetX, offsetY );
            return r;
        }

        void move( const Point offset ) {
            move( offset.x, offset.y );
        }

        void centerize() {
            const int diffX = width()  / 2;
            const int diffY = height() / 2;
            move( -diffX, -diffY );
        }

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

        bool isCollision( const Rect& rect ) const {
            if ( isSmallerThan(rect) ) {
                return false;
            }
            if ( rect.isSmallerThan( *this ) ) {
                return false;
            }
            return true;
        }

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


    std::vector<Point> line( const Point& from, const Point& to );

} /* namespace utils */

#endif /* UTILS_INCLUDE_UTILS_RECT_H_ */
