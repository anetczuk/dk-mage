/*
 * Parameter.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_
#define DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_

/// inclusion: #include "dkmage/Parameter.h"

#include <map>
#include <set>


namespace dkmage {

    /**
     *
     */
    enum class ParameterName {
        PN_DATA_PATH,
        PN_LEVELS_PATH,
        PN_OUTPUT_PATH,
        PN_OUTPUT_SUBPATH,
        PN_OUTPUT_ID,
        PN_OUTPUT_BMP,

        PN_SEED,
        PN_TYPE,
        PN_GOLD_SLABS_NUMBER,
        PN_GEM_FACES_NUMBER,
        PN_INIT_GOLD_AMOUNT,

        PN_TEST_MODE
    };

    const std::set< ParameterName >& getAllParameterNames();

    std::string getParameterName( const ParameterName parameter );


    /**
     * Pre C++17 implementation of std::optional.
     */
    template <typename T>
    class Optional {
    public:

        bool initialized;
        T data;

        Optional(): initialized(false) {
        }

        Optional( const T& value ): initialized(true), data(value) {
        }

        void operator =( const T& value ) {
            initialized = true;
            data = value;
        }

        operator bool() const {
            return initialized;
        }

        bool has_value() const {
            return initialized;
        }
        /// alias
        bool isSet() const {
            return initialized;
        }

        const T& value() const {
            return data;
        }
        /// alias
        const T& getValue() const {
            return data;
        }

        const T& value_or( const T& defaultValue ) const {
            if ( initialized )
                return data;
            return defaultValue;
        }
        /// alias
        const T& getValue( const T& defaultValue ) const {
            if ( initialized )
                return data;
            return defaultValue;
        }

    };


    /// ================================================


    class Parameter {
    public:

        Parameter();

    };


    /// ================================================


    class ParametersMap {
    public:

        using Data = std::map< std::string, std::string >;


    protected:

        Data data;


    public:

        ParametersMap() {
        }

        void appendData( const Data& parameters );

        void add( const std::string& name, const std::string& value );

        void add( const std::string& name, const std::size_t value );

        bool isSet( const std::string& parameter ) const;

        bool isSet( const ParameterName parameter ) const;

        Optional< std::string > getString( const std::string& parameter ) const;

        Optional< std::string > getString( const ParameterName parameter ) const;

        std::string getString( const std::string& parameter, const std::string& defaultValue ) const;

        std::string getString( const ParameterName parameter, const std::string& defaultValue ) const;

        Optional< std::size_t > getSizeT( const std::string& parameter ) const;

        Optional< std::size_t > getSizeT( const ParameterName parameter ) const;

        std::size_t getSizeT( const std::string& parameter, const std::size_t defaultValue ) const;

        std::size_t getSizeT( const ParameterName parameter, const std::size_t defaultValue ) const;

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_ */
