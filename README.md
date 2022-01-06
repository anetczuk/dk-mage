# dk-mage: **D**ungeon **K**eeper **MA**p **GE**nerator

Generate maps for Dungeon Keeper 1 PC game.


## Table of content

 1. [Generated map types](#map_types)
 2. [Running the generator](#running_the_generator)
     1. [Order of parameters processing](#parameters_processing)
     2. [Generated files](#generated_files)
 3. [Building under Linux (including WLS)](#building_under_linux)
     1. [Linux target](#linux_target)
     2. [Windows target](#windows_target)
     3. [Running unit tests](#running_tests)
 4. [Building under other platforms](#other_platforms)
 5. [Building Python bindings](#python_bindings)
 6. [Config parameters](#config_params)
     - [Example with detailed description](/doc/config.ini.md)
 7. [Releasing](#releasing)
 8. [Examples of various used techniques](#techniques)
 9. [Sources of randomness](#sources_randomness)
10. [References](#references)
     - [External libraries](#libraries)


## <a name="map_types"></a>Generated map types

Following map types can be generated:
- *cave* -- open area with chambers in center with hidden enemy creatures ([sample](doc/samples/cave.zip))
- *maze* -- big maze in center of map with hidden trasures and traps ([sample](doc/samples/maze.zip))
- *herofortress* -- siege of hero fortress ([sample](doc/samples/herofortress.zip))
- *random* -- randomly picks one of modes described above


Linked samples and following previews were generated with *Sample* seed.

[![cave type](doc/samples/cave-small.png "cave type")](doc/samples/cave/map04444.bmp)
[![maze type](doc/samples/maze-small.png "maze type")](doc/samples/maze/map04445.bmp)
[![herofortress type](doc/samples/herofortress-small.png "herofortress type")](doc/samples/herofortress/map04446.bmp)


## <a name="running_the_generator"></a>Running the generator

Before first run edit `config.ini` placed along main executable. Inside the file fill required variables (paths to game directories). Example of configuration file is described [here](doc/config.ini.md).

For simple generation just execute `dkmagecli` to generate random map. Generated map will be stored in `level_path` defined in config. If the path is game's directory then map will be ready to play from within `Free Play levels` game's menu. For advanced use of generator execute `dkmagecli --help` or open [dkmagecli help](doc/dkmagecli-help.md) description.

### <a name="parameters_processing"></a>Processing generator parameters

Generator parameters can be passed by command line argument or by configuration file. Processing sequence of parameters:
1. load `config.ini` from file passed by command line argument `--config`, if no cmd argument given then use default location
2. load all parameters from `general` section of configuration file
3. load all parameters from type section pointed by `--type` command line argument or `type` from `general` section
4. overwrite loaded parameters by arguments passed through command line (if any)

Processing sequence defines following priority of parameters (from most to least important):
1. command line parameters
2. type section from configuration file
3. `general` section from configuration file
4. default values if no parameters set

There are few exceptions from priority rule described above:
- `config` parameter is only read from command line
- `seed` parameter is only read from command line or `general` section from configuration file
- `type` parameter is only read from command line or `general` section from configuration file

### <a name="generated_files"></a>Generated files

Generator produces the same files as *ADiKtEd* does. Moreover there is one additional file `*.mage.ini` along stored map containing parameters used for generate map. The Ini file can be passed to generator to repeat the generation (it produces the same map). It is useful to share maps or for debug purpose.

In addition in work directory can be found two log files:
- `adikted.log.txt` containing logs from execution of *ADiKtEd* library
- `dkmage.log.txt` containing logs from execution of generator modules


## <a name="building_under_linux"></a>Building under Linux (including WLS)

To generate build system execute one of scripts placed in `configure` directory. It will create proper folder in `build` directory.
For example calling `configure/lin_release_gcc.sh` will create `build/lin_release_gcc` directory. To build the application then go to `build/lin_release_gcc` and run `make`.

It is also possible to configure prepare customized build by using `cmake` directly.

### <a name="linux_target"></a>Linux target

To build Linux executables run `configure/lin_release_gcc.sh` or `configure/lin_debug_gcc.sh` scripts then run `make` in proper subdirectory in `build`.

Following packages are required to proceed with build: 
- *cmake*
- *gcc*
- *g++*

They can be installed by following command `sudo apt install cmake g++ gcc`.

### <a name="windows_target"></a>Windows target

To build Windows executables run `configure/win_release_gcc.sh` or `configure/win_debug_gcc.sh` scripts then run `make` in proper subdirectory in `build`.

Following packages are required to build application under Linux for Windows platform:
- *g++-mingw-w64-i686*
- *gcc-mingw-w64-i686*
- *binutils-mingw-w64-i686*

They can be installed by following command `sudo apt install g++-mingw-w64-i686 gcc-mingw-w64-i686 binutils-mingw-w64-i686`.

### <a name="running_tests"></a>Running unit tests

Unit tests can be executed from within proper `build` subdirectory by executing `ctest` command.


## <a name="other_platforms"></a>Building under other platforms

To build one have to execute `cmake` directly in order to configure project and then execute `cmake --build .` from within configured build directory.


## <a name="python_bindings"></a>Building Python bindongs

Generation of Python bindings requires two elements:
1. *Python*
2. *swig* generator (*swig3.0* package on Ubuntu)

To generate bindings one has to check `BUILD_PYTHON_BINDINGS` in *cmake* configuration (e.g. by executing `cmake-gui`). Next step is to build the source code and preferably execute tests (`make` and `ctest` respectively). Final step is to execute `./deploy_zip.sh`. Script will package all bindings and required libraries into `deploy/pydkmage_{version}_{platform}.zip` archive.

Archive contains numerous unit tests and simple examples (`generate_map.py`, `generator.py`) how to use the bindings under Python. Unit tests can be executed from bindings root directory by following command: `python3 -m unittest`.


## <a name="config_params"></a>Config parameters

Adding new config parameters consists of several steps:
1. add or edit entry in *src/gen/parameter_name.csv*
2. run parameters generator `src/gen/params_generator.py`
3. use new `ParameterName::PN_*` enum value (defined in *ParameterName.h*)
4. use one of generated default value constants `PN_DEFAULT_*` (defined in *ParameterDefaults.h*)
5. extend *config.ini.in* with proper description of new parameter. Default values can be referenced using cmake constants `PN_DEFAULT_*` (defined in *ParameterDefaults.cmake*) 


## <a name="releasing"></a>Releasing

To release binaries proceed with following steps:
1. update version number of project (`project()` command in *src/CMakeLists.txt*)
2. build source code
3. ensure all tests pass (execute `ctest` from build directory)
3. generate documentation (usage info, samples, previews etc.) by running `generate_doc.sh` script from build directory (script will use prepared executable to generate required artifacts)
4. commit and push changes
5. run `deploy_zip.sh` from build directory
6. upload binaries (_build/bin/dkmage*.zip_ file)
7. upload bindings (_build/bin/pydkmage*.zip_ file)

Note that `deploy_zip.sh` prepares deploymnet package for platform configured in build directory. It might be desirable to execute the script on multiple build directories to collect proper artifacts. 


## <a name="techniques"></a>Examples of various used techniques:

- *CMake*'s `FetchContent` and proper configuration for `add_directory` (`target_include_directories`)
- generating sourcecode (_*.h_, _*.cpp_ and _*.cmake_) based on content of _*.csv_ files using Python
- generation of Python bindings using *swig* generator on non-trivial codebase


### <a name="sources_randomness"></a>Source of randomness

- random number generator (```stdlib.h```)
- uninitialized variables
- order of objects stored as pointers containers having order, especially pointers in ```std::set``` -- e.g.
```
std::set< int* > data;
data.insert( new int(1) );
data.insert( new int(2) );
data.insert( new int(3) );
/// each time order of numbers can be different
```


## <a name="references"></a>References:

- Mapmaker's Documentation (https://lubiki.keeperklan.com/dk1_docs/)
- description of Dungeon Keeper data formats (https://jonskeet.uk/dk/index.html)
- ADiKtEd editor (https://github.com/dkfans/ADiKtEd)
- maps and tools (https://lubiki.keeperklan.com/index.php)
- KeeperFX (https://github.com/dkfans/keeperfx)
- generation of caves (http://roguebasin.roguelikedevelopment.org/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels)
- Dungeon Keeper 2 mapper (https://www.killerbeesoftware.com/dk2/)
- Diablo map generator explained (https://www.boristhebrave.com/2019/07/14/dungeon-generation-in-diablo-1/)
- Swig: bindings generator (http://www.swig.org/)


### <a name="libraries"></a>External libraries
- ADiKtEd: handling DK's file formats (https://github.com/dkfans/ADiKtEd)
- filesystem: pre C++17 `std::filesystem` compatible implementation (https://github.com/gulrak/filesystem)
- LEMON: graphs implementation (https://lemon.cs.elte.hu)
- TCLAP: command line args processing (http://tclap.sourceforge.net/)
- inih: INI files parser (https://github.com/jtilly/inih)
- Catch2: testing framework for unit-tests (https://github.com/catchorg/Catch2)
