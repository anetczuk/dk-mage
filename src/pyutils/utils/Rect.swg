%module rect

%{
#include "utils/Rect.h"
%}

%include "std_sstream.i"
%include "std_vector.i"
%include "std_set.i"


%pythoncode %{####################################################%}


%extend utils::Point {  
    std::string __str__() {
        std::ostringstream out;
        out << *$self;
        return out.str();
    }
    std::string __repr__() {
        std::ostringstream out;
        out << "utils::Point" << *$self;
        return out.str();
    }
};


%template( PointList ) std::vector<utils::Point>;

%pythoncode %{
    def PointList():
        return []
%}


/*
/// workaround allowing to print type to string in nice way (disabled) 
%pythoncode %{
    ## monkey patching, cuz "%extend" does not work as expected
    def PointList_str( object ):
        ret = "["
        for item in object:
            ret += str(item) + ", "
        retSize = len(ret)
        if retSize > 1:
            ret = ret[:retSize-2]
        ret += "]"
        return ret
    #PointList.__str__ = PointList_str
    
    def PointList_repr( object ):
        ret = "PointList["
        for item in object:
            ret += repr(item) + ", "
        retSize = len(ret)
        if retSize > 1:
            ret = ret[:retSize-2]
        ret += "]"
        return ret
    #PointList.__repr__ = PointList_repr
%}
*/


%extend utils::Rect {  
    std::string __str__() {
        std::ostringstream out;
        out << *$self;
        return out.str();
    }
    std::string __repr__() {
        std::ostringstream out;
        out << "utils::Rect" << *$self;
        return out.str();
    }
};


%include "utils/Rect.h"
