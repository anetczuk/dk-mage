%module rect

%{
#include "utils/Rect.h"
%}

%include "std_sstream.i"
%include "std_vector.i"


%extend utils::Point {  
    std::string __str__() {
        std::ostringstream out;
        out << *$self;
        return out.str();
    }
};

%extend utils::Rect {  
    std::string __str__() {
        std::ostringstream out;
        out << *$self;
        return out.str();
    }
};


%template( PointList ) std::vector<utils::Point>;


%include "utils/Rect.h"
