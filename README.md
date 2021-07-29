# dk-mage: **D**ungeon **K**eeper **MA**p **GE**nerator

Generate maps Dungeon Keeper 1 PC game.


## Generated map types

Following map types can be generated:
- *cave* -- open area with chambers in center with hidden enemy creatures
- *maze* -- big maze in center of map with hidden trasures and traps

[![*cave* type](doc/cave-small.png "*cave* type")](doc/cave-big.png)
[![*maze* type](doc/maze-small.png "*maze* type")](doc/maze-big.png)


## Running generator

Before first run open and edit `config.ini` file placed along main executable. Inside the file fill required variables (paths to game directories).

For simple generation just execute `dkmagecli` to generate random map. Generated map will be stored in game's directory ready to play from within `Free Play levels` game's menu. For advanced use of generator execute `dkmagecli --help`:

```
USAGE: 

   dkmagecli  [-h] [--version] [--outputAuto|--outputId <int>|--outputPath
              <path string>] [--config <path string>] [--outbmp <path
              string>] [--seed <any string>] [--type <cave|maze|random>]


Where: 

   --config <path string>
     Path to configuration INI file

   --type <cave|maze|random>
     Map type

   --seed <any string>
     Generation seed

   --outbmp <path string>
     Path to map's output BMP file

   Either of:
      --outputPath <path string>
        Path to map's output file (relative or absolute)

      --outputId <int>
        Id of output map (will be placed in game's level directory)

      --outputAuto
        Finds unused map id and use it to store map

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   Map and scenario generator for Dungeon Keeper 1 PC game

```


## Building under Linux

To generate build system execute one of scripts placed in `./configure` directory. It will create proper folder in `build` directory.
For example calling `./configure/lin_release_gcc.sh` will create `build/lin_release_gcc` directory. To build the application then go to `build/lin_release_gcc` and run `make`.

Following packages are required to build application under Linux for Windows platform:
- *g++-mingw-w64-i686*
- *gcc-mingw-w64-i686*
- *binutils-mingw-w64-i686*
They can be installed by following command `sudo apt install g++-mingw-w64-i686 gcc-mingw-w64-i686 binutils-mingw-w64-i686`.


## Examples of various used techniques:

- *CMake*'s `FetchContent` and proper configuration for `add_directory` (`target_include_directories`)


## References:

- Mapmaker's Documentation: (https://lubiki.keeperklan.com/dk1_docs/)
- description of Dungeon Keeper data formats: (https://jonskeet.uk/dk/index.html)
- ADiKtEd editor: (https://github.com/dkfans/ADiKtEd)
- maps and tools: (https://lubiki.keeperklan.com/index.php)
- KeeperFX: (https://github.com/dkfans/keeperfx)
- generation of caves: (http://roguebasin.roguelikedevelopment.org/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels)
- Dungeon Keeper 2 mapper: (https://www.killerbeesoftware.com/dk2/)
- Diablo map generator explained: (https://www.boristhebrave.com/2019/07/14/dungeon-generation-in-diablo-1/)

### external libraries
- ADiKtEd: handling DK's file formats (https://github.com/dkfans/ADiKtEd)
- filesystem: pre C++17 `std::filesystem` compatible implementation (https://github.com/gulrak/filesystem)
- LEMON: graphs implementation (https://lemon.cs.elte.hu)
- TCLAP: command line args processing (http://tclap.sourceforge.net/)
- inih: INI files parser (https://github.com/jtilly/inih)
- Catch2: testing framework for unit-tests (https://github.com/catchorg/Catch2)
