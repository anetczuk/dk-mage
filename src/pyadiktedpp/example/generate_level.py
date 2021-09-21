#!/usr/bin/python3

##
## Demonstration script presenting how to generate random level and store it with bitmap preview to file.
##


import sys
import os.path
import argparse

import adiktedpp.level as level
import adiktedpp.messages as messages

import utils.rand as rand
import utils.log as log


def main():
    parser = argparse.ArgumentParser(description='Simple level generator')
    parser.add_argument('-dp', '--data_path', action='store', required=True, help="Path to game's 'data' directory" )
    parser.add_argument('-od', '--output_dir', action='store', required=False, default=".", help="Output directory" )
    parser.add_argument('-s', '--seed', action='store', required=False, default="", help="Seed for random number generator" )
    
    args = parser.parse_args()
    
    
    ## set random number generator for map
    if args.seed:
        rand.rng_srand( args.seed )
    else:
        rand.rng_srand()

    ## initialize ADiKtEd internal logger
    adiktedLogger = messages.ScopeMessages( "adikted.log" )

    ## initialize dkmage logger
    log.Logger.setLogFile( "dkmage.log" )
    
    log.log_info( "logger initialized" )
    
    mapLevel = level.Level()
    
    log.log_info( "starting generation" )
    
    ## generate random content
    mapLevel.generateRandomMap()
    
    ## set game's 'data' path
    mapLevel.setDataPath( args.data_path )
    
    log.log_info( "storing data to files" )
    
    mapPath = os.path.join( args.output_dir, "map06666" ) 
    mapLevel.saveMapByPath( mapPath )
    
    ## generate and store preview
    previewPath = os.path.join( args.output_dir, "preview.bmp" ) 
    valid = mapLevel.generateBmp( previewPath )
    
    if valid is False:
        print( "Unable to generate preview" )
        sys.exit(1)
    
    log.log_info( "done" )    
    sys.exit(0)
    

if __name__ == '__main__':
    main()
