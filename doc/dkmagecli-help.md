
USAGE: 

   dkmagecli  [-h] [--version] [--output_auto|--output_id <int>|
              --output_path <path string>|--output_subpath <path string>]
              [--config <path string>] [--output_bmp <path string>] [--seed
              <any string>] [--type <cave|herofortress|maze|random>]


Where: 

   --config <path string>
     Path to configuration INI file

   --type <cave|herofortress|maze|random>
     Map type

   --seed <any string>
     Generation seed

   --output_bmp <path string>
     Path to map's output BMP file

   Either of:
      --output_path <path string>
        Path to map's output file (absolute or relative to work dir)

      --output_subpath <path string>
        Path to map's output file relative to 'level_path' config field

      --output_id <int>
        Id of output map (will be placed in game's level directory)

      --output_auto
        Finds unused map id and use it to store map

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   Map and scenario generator for Dungeon Keeper 1 PC game

