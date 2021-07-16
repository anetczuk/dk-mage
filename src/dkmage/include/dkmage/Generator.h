/*
 * Generator.h
 *
 */

#ifndef CLI_INCLUDE_DKMAGE_GENERATOR_H_
#define CLI_INCLUDE_DKMAGE_GENERATOR_H_

#include "dkmage/LevelGenerator.h"

#include <vector>
#include <map>
#include <memory>
#include <string>


namespace dkmage {

    /**
     *
     */
    class Generator {
    private:

        struct AbstractItem {
            virtual dkmage::LevelGenerator* create() = 0;
        };

        template <typename T>
        struct Item: public AbstractItem {
            std::unique_ptr<T> data;
            dkmage::LevelGenerator* create() override {
                data.reset( new T() );
                return data.get();
            }
        };

        typedef std::unique_ptr< AbstractItem > LevelGeneratorPtr;

        std::map< std::string, LevelGeneratorPtr > genMap;


    public:

        std::vector< std::string > generatorsList() const;

        dkmage::LevelGenerator* get( const std::string& type );


        static Generator& instance();

        template <typename T>
        static void registerMode( const std::string& code ) {
            Generator& generator = instance();
            generator.add<T>( code );
        }


    private:

        Generator() = default;

        template <typename T>
        void add( const std::string& code ) {
            genMap[ code ].reset( new Item<T>() );
        }

    };


    /**
     *
     */
    template <typename T>
    class GeneratorRegister {
    public:

        GeneratorRegister( const std::string& code ) {
            Generator::registerMode<T>( code );
        }

    };

} /* namespace cli */

#endif /* CLI_INCLUDE_CLI_GENERATOR_H_ */
