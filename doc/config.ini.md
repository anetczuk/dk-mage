Sample `config.ini`:

```
##
## Generator's configuration file.
##
##
## Data types:
##  - VALUE         -- single value, e.g. numeric or path
##  - VALUES_LIST   -- list of comma separated values, e.g. red,green,blue or 1,2,5,8. List containing single element allowed (without comma then).
##  - NUMERIC_RANGE -- numeric range in format {numA}:{numB}, e.g. 10:30
##  - NUMERIC_SET   -- numeric set defined as VALUES_LIST containing numeric VALUE-s or NUMERIC_RANGE-s, e.g. 1,3,10:16,33
##


[GENERAL]

## Path to DK1 Original or Gold version 'data' directory. Field is required.
## Type: single path
## Example: /path/to/DK1 Gold/DATA
data_path =

## Generated maps default output directory. Field optional (maps will be stored to work directory if left undefined or empty).
## The behaviour can be affected by 'output_path' or 'output_subpath' parameter.
## Type: single path
## Example (keeperfx 0.4.8): /path/to/game/levels
## Example (keeperfx 0.4.9): /path/to/game/levels/personal
levels_path =

## Path to map's output file (absolute or relative to work dir).
## Type: single path
## Example: /path/to/map03333
# output_path =

## Path to map's output file relative to 'level_path' config field.  
## Type: single path
## Example: ../map03333
# output_subpath =

## Id of output map (will be placed in game's level directory).
## For example giving id '3333' will produce/overwrite 'map03333' inside 'levels_path' directory.
## Type: single unsigned integer
## Example: 3333
# output_id =

## Priority of 'output_' parameters: 'output_path', 'output_subpath', 'output_id'. If none given then 'auto' mode applies.

## Path to additional BMP preview of generated map.
## Type: single path
## Example: /path/to/map.bmp
# output_bmp =

## Seed for random number generator. For given it always generate the same map.
## Type: any string
## Example 1: Evil map
## Example 2: ON8SJF3O
# seed =

## Map type, one of: cave maze herofortress random
## Type: VALUES_LIST containing one of above values. In case of multiple values one will be randomized.
## Example 1: cave
## Example 2: cave,maze
# type = random


## Number of failure attempts to perform before exit
## Type: VALUE of unsigned integer
fail_attempts = 7


## Limit of action points on map allowed by game.
## Type: VALUE of unsigned integer
# action_points_limit = 31

## Limit of tunnellers trigger commands (ADD_TUNNELLER_PARTY_TO_LEVEL) allowed by game.
## Type: VALUE of unsigned integer
# script_tunnellers_limit = 256

## Limit of parties and creature trigger commands (ADD_*_TO_LEVEL) allowed by game.
## Type: VALUE of unsigned integer
# script_parties_limit    = 256

## Limit of script commands (almost every) inside IF body allowed by game.
## Type: VALUE of unsigned integer
# script_values_limit     = 256

## Limit of IF conditions (IF, IF_*) allowed by game.
## Type: VALUE of unsigned integer
# script_if_conds_limit   = 256

## Limit of party definitions conditions (CREATE_PARTY) allowed by game.
## Type: VALUE of unsigned integer
# script_party_defs_limit = 256


## Number of gold slabs initailly available for human player.
## Type: NUMERIC_SET of unsigned integers. Number will be random from given set.
## Example: 60:80,90,100
# gold_slabs_number =

## Number of gem slabs available for human player. Slabs number is upper limited by 'gem_faces_number'.
## Type: NUMERIC_SET of unsigned integers. Number will be random from given set.
## Example: 2,3
# gem_slabs_number = 

## Number of gem faces available for human player. Faces number is upper limited by 'gem_slabs_number' * 4.
## Type: NUMERIC_SET of unsigned integers. Number will be random from given set.
## Example: 2,3
# gem_faces_number =

## Gem trap consists of guard chamber and leading corridor, but without gem slab.
## Type: NUMERIC_SET of unsigned integers. Number will be random from given set.
# gem_traps_number = 

## Number of creatures guarding each gem slab. Number will be random from given set.
## Type: NUMERIC_SET of unsigned integers
# gem_guard_number = 3:5

## Experience of gem guards. Number will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
# gem_guard_level = 4:7

## Initial gold amount for human player.
## Type: NUMERIC_SET of unsigned integers. Number will be random from given set.
## Example 1: 10000
## Example 2: 10000,20000
## Example 3: 10000,40000:60000
# init_gold_amount = 20000

## Creatures limit of human player. Parameter is passed to 'MAX_CREATURES' script command. Number will be random from given set.
## Type: NUMERIC_SET of unsigned integers
# creatures_limit = 

## Attitude of enemy AI. Parameter is passed to 'COMPUTER_PLAYER' script command. Number will be random from given set.
## Used in cave and maze mode.
## Oficially supported values: 0, 1, 3, 4, 5 or 10
## Type: NUMERIC_SET of unsigned integers
# enemy_keeper_attitude = 

## Number of rooms enemy keeper have at start apart of dungeon heart and portal.
## Used in cave and maze mode.
## Type: NUMERIC_SET of unsigned integers
# enemy_keeper_rooms_number = 

## Creatures limit of enemy keeper can have. Parameter is passed to 'MAX_CREATURES' script command. Number will be random from given set.
## Used in cave and maze mode.
## Type: NUMERIC_SET of unsigned integers
# enemy_keeper_creatures_limit = 


## Enable testing map for given map type. 
## To disable comment-out parameter.
## Type: any string
# test_mode = on

## Additional goodies for human player: reveal map special item, additional 20 mistresses and +200 000 to initial gold. 
## To disable comment-out parameter.
## Type: any string
# test_addons = on

## Do not re-attempt to generate map on failure. 
## To disable comment-out parameter.
## Type: any string
# stop_on_fail = on


##
## Additional, specific or overwritten paramters for 'cave' type.
##
[CAVE]

# gold_slabs_number = 80
# gem_slabs_number  = 1
# gem_faces_number  = 1
# gem_traps_number  = 2

# creatures_limit = 30

# enemy_keeper_attitude = 0
# enemy_keeper_rooms_number = 11
# enemy_keeper_creatures_limit = 50

## Number of central caverns with hidden creatures. Number will be random from given set.
## Type: NUMERIC_SET of unsigned integers
## Default: 28
# cental_caverns_number = 28

## Number of creatures in each cavern. Number for each cavern will be random from given set.
## Type: NUMERIC_SET of unsigned integers
## Default: 3:7
# cavern_creatures_number = 3:7

## Experience of creatures inside central caverns. The closer to map center, greater the level.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Default: 3:10
# cavern_creatures_level = 3:10


##
## Additional, specific or overwritten paramters for 'maze' type.
##
[MAZE]

# gold_slabs_number = 40
# gem_slabs_number  = 2
# gem_faces_number  = 2
# gem_traps_number  = 4

# creatures_limit = 10

# enemy_keeper_attitude = 0
# enemy_keeper_rooms_number = 11
# enemy_keeper_creatures_limit = 50


##
## Additional, specific or overwritten paramters for 'herofortress' type.
##
[HEROFORTRESS]

# gold_slabs_number = 52
# gem_slabs_number  = 1
# gem_faces_number  = 1
# gem_traps_number  = 2

# creatures_limit = 25

## Allowed number of entrances to fortress. There will be highest possible entrances number limited to values from given set.
## Type: NUMERIC_SET of unsigned integers
## Example 1: 3     -- there will be 3 entrances
## Example 2: 3,4,5 -- there will be 3, 4 or 5 entrances
## Example 3: 1,3:4 -- there will be 1, 3 or 4 entrances
# entrances_number = 2:5

## Experience level of bridge guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
# bridge_guard_level = 1:3

## Experience level of guards wandering through corridors. The closer to dungeon heart, higher the level.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
# corridor_guard_level = 4:7

## Experience level of torture chamber guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
# torture_guard_level = 5:8

## Experience level of graveyard guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
# graveyard_guard_level = 7:10

## Experience level of dungeon heart guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
# dungeon_header_guard_level = 8:10
```
