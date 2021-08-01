/*
 * LakeGenerator.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_LAKEGENERATOR_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_LAKEGENERATOR_H_

#include "utils/Rect.h"

#include <algorithm>
#include <set>


namespace adiktedpp {

    /**
     *
     */
    class LakeGenerator {
    public:

        std::set< utils::Point > available;             /// points to use
        std::set< utils::Point > added;                 /// points already visited/added


        void generateLake( const utils::Rect& boundingLimit, const double areaFactor = 0.7 ) {
            const int area = boundingLimit.area() * areaFactor;
            generate( boundingLimit, area );
        }

        void generate( const utils::Rect& boundingLimit, const std::size_t size ) {
            const int boundingArea = boundingLimit.area();
            if ( boundingArea < 1 ) {
                return;
            }

            const utils::Point center = boundingLimit.center();
            add( center );

            const std::size_t area = std::min( (std::size_t) boundingArea, size );
            while ( added.size() < area ) {
                const std::size_t avSize = available.size();
                const std::size_t vIndex = rand() % avSize;
                const utils::Point point = getAvailable( vIndex );
                if ( boundingLimit.isInside( point ) == false ) {
                    continue ;
                }
                add( point );
            }
        }


    protected:

        void add( const utils::Point& point ) {
            if ( added.count( point ) > 0 ) {
                return ;
            }
            added.insert( point );
            addAvailable( point + utils::Point(  1, 0 ) );
            addAvailable( point + utils::Point( -1, 0 ) );
            addAvailable( point + utils::Point( 0,  1 ) );
            addAvailable( point + utils::Point( 0, -1 ) );
            available.erase( point );
        }

        void add( const std::size_t availableIndex ) {
            auto avpos = available.begin();
            std::advance( avpos, availableIndex );
            add( *avpos );
        }

        utils::Point getAvailable( const std::size_t availableIndex ) {
            auto avpos = available.begin();
            std::advance( avpos, availableIndex );
            const utils::Point point = *avpos;              /// yes, copy
            available.erase( avpos );
            return point;
        }

        void addAvailable( const utils::Point& point ) {
            if ( added.count( point ) > 0 ) {
                return ;
            }
            available.insert( point );
        }

        std::string print() const;

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LAKEGENERATOR_H_ */
