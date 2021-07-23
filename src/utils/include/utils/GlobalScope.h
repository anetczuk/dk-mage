/*
 * GlobalScope.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_GLOBALSCOPE_H_
#define UTILS_INCLUDE_UTILS_GLOBALSCOPE_H_

/// inclusion: #include "utils/GlobalScope.h"


#define JOIN2(a,b)         JOIN2_PRIV(a,b)
#define JOIN2_PRIV(a,b)    a ## b


#define GLOBAL_SCOPE_REG( name )                                    \
        struct name {                                               \
            name() {                                                \
                execute();                                          \
            }                                                       \
            void execute();                                         \
        };                                                          \
        name JOIN2( var, name );                                    \
        void name::execute()                                        \

#define GLOBAL_SCOPE()      GLOBAL_SCOPE_REG( JOIN2( GlobalScope, __LINE__ ) )


#endif /* UTILS_INCLUDE_UTILS_GLOBALSCOPE_H_ */
