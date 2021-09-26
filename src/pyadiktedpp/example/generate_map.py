#!/usr/bin/python3

##
## Demonstration script presenting how to generate random level and store it with bitmap preview to file.
##


import sys
import os.path
import argparse
import logging

import adiktedpp.script as script
import adiktedpp.level as level
import adiktedpp.type as type
import adiktedpp.raw as raw
import adiktedpp.messages as messages
import adiktedpp.rand as rand

import utils.rect as rect
import utils.log as log


## =================================================================================


def configure_console( logLevel=None ):
    if logLevel is None:
        logLevel = logging.DEBUG

    consoleHandler = logging.StreamHandler( stream=sys.stdout )

    formatter = create_formatter()

    consoleHandler.setFormatter( formatter )

    logging.root.addHandler( consoleHandler )
    logging.root.setLevel( logLevel )


def create_formatter(loggerFormat=None):
    if loggerFormat is None:
        ## loggerFormat = '%(asctime)s,%(msecs)-3d %(levelname)-8s %(threadName)s [%(filename)s:%(lineno)d] %(message)s'
        loggerFormat = ('%(asctime)s,%(msecs)-3d %(levelname)-8s %(threadName)s %(name)s:%(funcName)s '
                        '[%(filename)s:%(lineno)d] %(message)s')
    dateFormat = '%Y-%m-%d %H:%M:%S'
    return EmptyLineFormatter( loggerFormat, dateFormat )
    ## return logging.Formatter( loggerFormat, dateFormat )


class EmptyLineFormatter(logging.Formatter):
    """Special formatter storing empty lines without formatting."""

    ## override base class method
    def format(self, record):
        msg = record.getMessage()
        clearMsg = msg.replace('\n', '')
        clearMsg = clearMsg.replace('\r', '')
        if not clearMsg:
            # empty
            return msg
        return super().format( record )


class LoggingSink( log.LogSink ):
    def __init__( self ):
        super().__init__()
        
        consoleHandler = logging.StreamHandler( stream=sys.stdout )
        loggerFormat = ('%(asctime)s,%(msecs)-3d %(levelname)-8s %(threadName)s %(message)s')
        formatter = create_formatter( loggerFormat )
        consoleHandler.setFormatter( formatter )
    
        self.logger = logging.getLogger( "pyadiktedpp" )
        self.logger.addHandler( consoleHandler )
        self.logger.setLevel( logging.DEBUG )
          
    def printMessage( self, level, file, lineNo, message ):
        prefix = self.logPrefix( file, lineNo )

        if level == log.LogLevel_LL_INFO:
            self.logger.info( "%s: %s", prefix, message )
        elif level == log.LogLevel_LL_WARN:
            self.logger.warning( "%s: %s", prefix, message )
        elif level == log.LogLevel_LL_ERR:
            self.logger.error( "%s: %s", prefix, message )
        else:
            self.logger.info( "%s: %s", prefix, message )

logSink = LoggingSink()
log.LogConfig.setLogSink( logSink )


## =================================================================================


def main():
    parser = argparse.ArgumentParser(description='Simple level generator')
    parser.add_argument('-dp', '--data_path', action='store', required=False, default="", help="Path to game's 'data' directory" )
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

    log.log_info( "logger initialized" )
    
    mapLevel = level.Level()
    rawLevel = mapLevel.getRawLevel()
    
    
    log.log_info( "starting generation" )
    
    ## generate random content
    mapLevel.generateRandomMap(25)
    
    ## add some items
    owner = type.Player_P_P0
    mapRect = raw.RawLevel.mapSize()
    heartCenter = mapRect.center()
    heartRect = rect.Rect( heartCenter, 5, 5 )
    mapLevel.setRoom( heartRect, type.Room_R_DUNGEON_HEART, owner, True )
    mapLevel.setCreature( heartCenter - rect.Point(0, 2), 1, type.Creature_C_IMP, 3, 1, owner )
    mapLevel.setItem( heartCenter + rect.Point(0, 2), 1, type.Item_I_SPECIAL_REVMAP )
    
    treasureCenter = heartCenter + rect.Point(10, 0)
    mapLevel.digLine( heartCenter, treasureCenter, owner, True )
    treasureRect = rect.Rect( treasureCenter, 5, 5 )
    mapLevel.setRoom( treasureRect, type.Room_R_TREASURE, owner, True )
    mapLevel.setSlab( treasureCenter, type.Slab_S_GEMS )
    mapLevel.setDoor( treasureCenter - rect.Point(3, 0), type.Door_D_IRON )
    
    pointsList = rect.PointList()
    pointsList.append( rect.Point( heartCenter + rect.Point( 0, -3) ) )
    pointsList.append( rect.Point( heartCenter + rect.Point( 0, -4) ) )
    pointsList.append( rect.Point( heartCenter + rect.Point( 0, -5) ) )
    mapLevel.digClaimed( pointsList, owner, False )
    mapLevel.setItem( pointsList[2], 4, type.Item_I_GOLDCHEST )
    
    ## golder island
    goldRect = heartRect.moved( rect.Point(-20, 0) )
    goldRect.grow( 3 )
    lakeRect = rect.Rect( goldRect )
    lakeRect.grow( 1 )
    mapLevel.setSlab( lakeRect, type.Slab_S_WATER )
    lakeRect.grow( 2 )
    mapLevel.setCave( lakeRect, type.Slab_S_WATER )
    mapLevel.setCave( goldRect, type.Slab_S_GOLD )
    
    ## script
    mapScript = script.Script()
    mapScript.setHeaderInfo()
    scriptInit = mapScript.initSection()
    mapScript.setFXLevel()
    scriptInit.START_MONEY( owner, 10000 )
    ###
    scriptInit.addEmptyLine()
    roomsAvailable = script.RoomsAvailableState()
    roomsAvailable.setStandard()
    mapScript.set( roomsAvailable )
    ###
    scriptInit.addEmptyLine()
    createAvailable = script.CreatureAvailableState()
    createAvailable.setEvilAvailable( owner, True )
    mapScript.set( createAvailable )
    ###
    scriptInit.addEmptyLine()
    doorAvailable = script.DoorAvailableState()
    doorAvailable.setStateAmount( owner, type.Door_D_WOOD, 2 )
    mapScript.set( doorAvailable )
    ###
    scriptInit.addEmptyLine()
    trapAvailable = script.TrapAvailableState()
    trapAvailable.setStateFlag( owner, type.Trap_T_BOULDER, False )
    mapScript.set( trapAvailable )
    ###
    scriptInit.addEmptyLine()
    magicAvailable = script.MagicAvailableState()
    magicAvailable.setAllAvailable( owner, script.MagicAvailableMode_AM_AVAILABLE )
    mapScript.set( magicAvailable )
    ###
    mapScript.setWinConditionKillGood()
    mapScript.setLoseConditionStandard( owner )
    script.applyScript( mapLevel, mapScript )
    
    ## map stats
    log.log_info( "map stats:" )
    log.log_info( "separated areas: " + str( mapLevel.countSeparatedAreas() ) )
    log.log_info( "claim areas:     " + str( mapLevel.countClaimAreas() ) )
    log.log_info( "solid slabs:     " + str( mapLevel.countSolid( mapRect ) ) )
    log.log_info( "total items:     " + str( mapLevel.countItems( mapRect ) ) )
    log.log_info( "creatures:       " + str( rawLevel.countAllCreatures() ) )
    log.log_info( "action points:   " + str( rawLevel.countActionPoints() ) )

    log.log_info( "\nitems list:\n" + rawLevel.printItems() )
    
    
    valid = mapLevel.verifyMap( False, True )
    if valid is False:
        log.log_warn( "map warnings detected" )
    
    
    log.log_info( "storing data to files" )
    
    ## create output directories if needed
    os.makedirs( args.output_dir, exist_ok=True )
    
    mapPath = os.path.join( args.output_dir, "map06666" ) 
    mapLevel.saveMapByPath( mapPath )
    
    if len(args.data_path) > 0:
        ## set game's 'data' path
        mapLevel.setDataPath( args.data_path )
        
        ## generate and store preview
        previewPath = os.path.join( args.output_dir, "preview.bmp" ) 
        valid = mapLevel.generateBmp( previewPath )
            
        if valid is False:
            print( "Unable to generate preview" )
            sys.exit(1)
    else:
        log.log_warn( "'data_path' parameter not given -- preview generation unavailable" )
    
    log.log_info( "done" )    
    sys.exit(0)
    

if __name__ == '__main__':
    main()
