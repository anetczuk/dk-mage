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
#output_path =

## Path to map's output file relative to 'level_path' config field.  
## Type: single path
## Example: ../map03333
#output_subpath =

## Id of output map (will be placed in game's level directory).
## For example giving id '3333' will produce/overwrite 'map03333' inside 'levels_path' directory.
## Type: single unsigned integer
## Example: 3333
#output_id =

## Priority of 'output_' parameters: 'output_path', 'output_subpath', 'output_id'. If none given then 'auto' mode applies.

## Path to additional BMP preview of generated map.
## Type: single path
## Example: /path/to/map.bmp
#output_bmp =

## Seed for random number generator. For given it always generate the same map.
## Type: any string
## Example 1: Evil map
## Example 2: ON8SJF3O
#seed =

## Map type, one of: cave maze herofortress random
## Type: VALUES_LIST containing one of above values. In case of multiple values one will be randomized.
## Example 1: cave
## Example 2: cave,maze
#type =

## Number of gold slabs initailly available for human player.
## Type: NUMERIC_SET of unsigned integers. One value will be picked.
## Example: 60:80,90,100
#gold_slabs_number =

## Number of gem faces initailly available for human player.
## Type: NUMERIC_SET of unsigned integers. One value will be picked.
## Example: 2,3
#gem_faces_number =

## Initial gold amount for human player.
## Type: NUMERIC_SET of unsigned integers. One value will be picked.
## Example 1: 10000
## Example 2: 10000,20000
## Example 3: 10000,40000:60000
#init_gold_amount =

## Additional goodies for human player: reveal map special item, additional 20 mistresses and +200 000 to initial gold. 
## To disable comment-out parameter.
## Type: any string
#test_mode = on

## Do not re-attempt to generate map on failure. 
## To disable comment-out parameter.
## Type: any string
#stop_on_fail = on


##
## Additional, specific or overwritten paramters for 'cave' type.
##
[CAVE]

#gold_slabs_number = 80

## Max allowed value: 4
#gem_faces_number = 1

#init_gold_amount = 20000


##
## Additional, specific or overwritten paramters for 'maze' type.
##
[MAZE]

#gold_slabs_number = 40

## Max allowed value: 4
#gem_faces_number = 2

#init_gold_amount = 20000


##
## Additional, specific or overwritten paramters for 'herofortress' type.
##
[HEROFORTRESS]

#gold_slabs_number = 40

## Max allowed value: 4
#gem_faces_number = 2

#init_gold_amount = 20000

## Allowed number of entrances to fortress. There will be highest possible entrances number limited to values from given set.
## Type: NUMERIC_SET of unsigned integers
## Example 1: 3     -- there will be 3 entrances
## Example 2: 3,4,5 -- there will be 3, 4 or 5 entrances
## Example 3: 1,3:4 -- there will be 1, 3 or 4 entrances
entrances_number = 2:5

## Experience level of bridge guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
bridge_guard_level = 2:5

## Experience level of guards wandering through corridors. The closer to dungeon heart, higher the level.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
corridor_guard_level = 4:7

## Experience level of torture chamber guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
torture_guard_level = 5:8

## Experience level of graveyard guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
graveyard_guard_level = 7:10

## Experience level of dungeon heart guards. Level of each guardian will be random from given set.
## Type: NUMERIC_SET of unsigned integers in range [1..10]
## Example: 2,3:5
dungeon_header_guard_level = 7:10
```