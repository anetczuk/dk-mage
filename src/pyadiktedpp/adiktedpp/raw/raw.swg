%module raw


%pythoncode %{####################################################%}


## prevent Warning 509
%ignore adiktedpp::raw::operator<<;


%include "PlayerType.swg"
%include "SlabType.swg"
%include "SubTypeCreature.swg"
%include "SubTypeDoor.swg"
%include "SubTypeItem.swg"
%include "SubTypeTrap.swg"
%include "ThingType.swg"

%include "RawLevel.swg"
