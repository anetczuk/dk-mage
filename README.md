# dk-mage: **D**ungeon **K**eeper **MA**p **GE**nerator

Generate maps Dungeon Keeper 1 PC game.


## Building under Linux

Following packages are required to build application under Linux:
- *g++-mingw-w64-i686*
- *gcc-mingw-w64-i686*
- *binutils-mingw-w64-i686*
They can be installed by following command `sudo apt install gcc-mingw-w64-i686 gcc-mingw-w64-i686 binutils-mingw-w64-i686`.

Next step is to install application dependencies. It can be done by executing script `./external/build.sh`. Script, among other things, will build *libadikted*.

To generate build system call one of scripts placed in `./configure` directory. It will create proper folder in `build` directory.
For example calling `./configure/release_gcc.sh` will create `build/release_gcc` directory. To build the application then go to `build/release_gcc` and run `make`.


## References:

- Mapmaker's Documentation: (https://lubiki.keeperklan.com/dk1_docs/)
- description of Dungeon Keeper data formats: (https://jonskeet.uk/dk/index.html)
- ADiKtEd editor: (https://github.com/dkfans/ADiKtEd)
- maps and tools: (https://lubiki.keeperklan.com/index.php)
- KeeperFX: (https://github.com/dkfans/keeperfx)
- generation of caves: (http://roguebasin.roguelikedevelopment.org/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels)
- Dungeon Keeper 2 mapper: (https://www.killerbeesoftware.com/dk2/)
- Diablo map generator explained: (https://www.boristhebrave.com/2019/07/14/dungeon-generation-in-diablo-1/)
