/*
 * Macro.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_MACRO_H_
#define UTILS_INCLUDE_UTILS_MACRO_H_

/// inclusion: #include "utils/Macro.h"


#define JOIN2(a,b)         JOIN2_PRIV(a,b)
#define JOIN2_PRIV(a,b)    a ## b


#define ENUM_STREAM_CASE( enumClass, enumValue, stream )                            \
    case enumClass::enumValue:    { stream << #enumValue; return stream; }


#endif /* UTILS_INCLUDE_UTILS_MACRO_H_ */
