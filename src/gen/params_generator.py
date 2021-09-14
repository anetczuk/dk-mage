#!/usr/bin/python3

import sys, os
import logging
import pathlib

import csv

from pandas import DataFrame


_LOGGER = logging.getLogger(__name__)

SCRIPT_DIR = os.path.dirname(__file__)      ## full path to script's directory


# ====================================================================
# ====================================================================


def generate():
    csv_path = os.path.join( SCRIPT_DIR, "parameter_name.csv" )
    _LOGGER.info( "parsing file %s", csv_path )
    configDict, dataMatrix = parse_csv( csv_path )
    
    configDict[ 'input_file' ] = csv_path
    
    # print( "config:", configDict )
    # print( "data:", dataMatrix )
    
    includeDir = os.path.join( SCRIPT_DIR, "..", "dkmage", "include", "dkmage" )
    generate_enum_header( configDict, dataMatrix, includeDir )
    
    sourceDir = os.path.join( SCRIPT_DIR, "..", "dkmage", "src" )
    generate_enum_source( configDict, dataMatrix, sourceDir )

    generate_defaults_header( configDict, dataMatrix, includeDir )
    
    mainDir = os.path.join( SCRIPT_DIR, ".." )
    generate_defaults_cmake( configDict, dataMatrix, mainDir )


def generate_enum_header( configDict, dataMatrix, outputDir ):
    _LOGGER.info( "generating enum header file" )
    
    h_output_content = ""

    ### === file header ===
    input_file            = configDict[ "input_file" ]
    header_guard_prefix   = configDict[ "header_guard_prefix" ]
    enum_name             = configDict[ "enum_name" ]
    if len( enum_name ) < 1:
        raise Exception( "Missing 'enum_name' config parameter" )
    
    output_name  = enum_name + ".h"
    header_guard = header_guard_prefix + enum_name.upper() + "_H_"

    h_output_content += \
"""/*
 * %(file_name)s
 *
 */

%(file_preamble_info)s

#ifndef %(header_guard)s
#define %(header_guard)s

#include <set>
#include <string>


namespace dkmage {

    /**
     *
     */
    enum class %(enum_name)s {
""" % {
        "file_name": output_name,
        "file_preamble_info": file_preamble( input_file, "///" ),
        "header_guard": header_guard,
        "enum_name": enum_name
    }


    ### === file content ===

    value_prefix = configDict.get( 'enum_prefix', None )

    lastIndex = dataMatrix.shape[0] - 1

    for index, row in dataMatrix.iterrows():
        enum_value     = row['enum_value']
        parameter_name = parameter_name_from_row( row )
        comment        = row['comment']

        if len( enum_value ) < 1:
            raise Exception( "Missing 'enum_value'" )
        
        if len(comment) > 0:
            comment = " - " + comment

        if enum_value.startswith( "#" ):
            h_output_content += """\n"""
            continue
        if enum_value.startswith( "//" ):
            h_output_content += """        %s\n""" % enum_value
            continue
            
        item_value = value_prefix + enum_value
        if index != dataMatrix.index[-1]:
            entry_line = """        %s,""" % item_value
        else:
            ## last row
            entry_line = """        %s""" % item_value

        indent = max( 50 - len( entry_line ), 5 )
        h_output_content += """%s%s/// %s%s\n""" % (entry_line, " " * indent, parameter_name, comment)


    ### === file footer ===

    h_output_content += \
"""    };


    const std::set< ParameterName >& getAllParameterNames();

    /**
     * User specific parameters (e.g. paths) that does not affect map content itself.
     */
    const std::set< ParameterName >& getLocalParameterNames();

    std::string getParameterName( const ParameterName parameter );

    std::set< std::string > getParameterName( const std::set< ParameterName >& parameters );

} /* namespace dkmage */

#endif /* %(header_guard)s */
""" % {
        "enum_name": enum_name,
        "header_guard": header_guard
    }

    # _LOGGER.debug( "generated file:\n%s" + h_output_content )

    ### === writing to file ===
    os.makedirs( outputDir, exist_ok=True )
    outputFile = os.path.join( outputDir, output_name )
    with open( outputFile, "w" ) as enumFile:
        enumFile.write( h_output_content )


def generate_enum_source( configDict, dataMatrix, outputDir ):
    _LOGGER.info( "generating enum source file" )

    cpp_output_content = ""

    ### === file header ===

    input_file            = configDict[ "input_file" ]
    enum_name             = configDict[ "enum_name" ]
    if len( enum_name ) < 1:
        raise Exception( "Missing 'enum_name' config parameter" )
    
    output_name  = enum_name + ".cpp"
    header_name  = enum_name + ".h"

    cpp_output_content += \
"""/*
 * %(file_name)s
 *
 */

%(file_preamble_info)s

#include "dkmage/%(header_name)s"

#include "utils/Log.h"


namespace dkmage {
""" % {
        "file_name": output_name,
        "file_preamble_info": file_preamble( input_file, "///" ),
        "header_name": header_name
    }

    value_prefix = configDict.get( 'enum_prefix', None )
    

    ### === getAllParameterNames ===

    cpp_output_content += \
"""
    const std::set< %(enum_name)s >& getAll%(enum_name)ss() {
        static std::set< %(enum_name)s > data;
        if ( data.empty() ) {
""" % { "enum_name": enum_name }

    for index, row in dataMatrix.iterrows():
        enum_value = row['enum_value']

        if len( enum_value ) < 1:
            raise Exception( "Missing 'enum_value'" )

        if enum_value.startswith( "#" ):
            cpp_output_content += """\n"""
            continue
        if enum_value.startswith( "//" ):
            cpp_output_content += """        %s\n""" % enum_value
            continue

        item_value = value_prefix + enum_value

        cpp_output_content += """            data.insert( %s::%s );\n""" % ( enum_name, item_value )

    cpp_output_content += \
"""        }
        return data;
    }
"""

    ### === getLocalParameterNames ===

    cpp_output_content += \
"""
    const std::set< %(enum_name)s >& getLocal%(enum_name)ss() {
        static std::set< %(enum_name)s > data;
        if ( data.empty() ) {
""" % { "enum_name": enum_name }

    for index, row in dataMatrix.iterrows():
        is_local = row['is_local']
        if is_local.lower() in ['', 'false', '0']:
            continue;
        
        enum_value = row['enum_value']
        if len( enum_value ) < 1:
            raise Exception( "Missing 'enum_value'" )

        item_value = value_prefix + enum_value

        cpp_output_content += """            data.insert( %s::%s );\n""" % ( enum_name, item_value )

    cpp_output_content += \
"""        }
        return data;
    }
"""

    ### === getParameterName ===

    cpp_output_content += \
"""
    std::string get%(enum_name)s( const %(enum_name)s parameter ) {
        switch( parameter ) {
""" % { "enum_name": enum_name }


    for index, row in dataMatrix.iterrows():
        enum_value     = row['enum_value']
        parameter_name = parameter_name_from_row( row )

        if len( enum_value ) < 1:
            raise Exception( "Missing 'enum_value'" )

        if enum_value.startswith( "#" ):
            cpp_output_content += """\n"""
            continue
        if enum_value.startswith( "//" ):
            cpp_output_content += """        %s\n""" % enum_value
            continue

        item_value = value_prefix + enum_value

        entry_line = """        case %s::%s: """ % ( enum_name, item_value )
        indent = max( 70 - len( entry_line ), 5 )
        cpp_output_content += """%s%sreturn "%s";\n""" % (entry_line, " " * indent, parameter_name)


    cpp_output_content += \
"""        }

        std::stringstream stream;
        stream << FILE_NAME << ": invalid argument -- unknown parameter: " << (int)parameter;
        LOG() << stream.str();
        throw std::invalid_argument( stream.str() );
    }

    std::set< std::string > getParameterName( const std::set< ParameterName >& parameters ) {
        std::set< std::string > ret;
        for ( const auto item: parameters ) {
            const std::string name = getParameterName( item );
            ret.insert( name );
        }
        return ret;
    }

} /* namespace dkmage */
"""

    # _LOGGER.debug( "generated cpp:\n%s" + cpp_output_content )

    ### === writing to file ===
    os.makedirs( outputDir, exist_ok=True )
    outputFile = os.path.join( outputDir, output_name )
    with open( outputFile, "w" ) as enumFile:
        enumFile.write( cpp_output_content )


def generate_defaults_header( configDict, dataMatrix, outputDir ):
    _LOGGER.info( "generating defaults header file" )
    
    h_output_content = ""

    ### === file header ===
    input_file           = configDict[ "input_file" ]
    header_guard_prefix  = configDict[ "header_guard_prefix" ]
    header_name          = "ParameterDefaults"
    output_name          = header_name + ".h"
    header_guard         = header_guard_prefix + header_name.upper() + "_H_"

    h_output_content += \
"""/*
 * %(file_name)s
 *
 */

%(file_preamble_info)s

#ifndef %(header_guard)s
#define %(header_guard)s

/// inclusion: #include "dkmage/ParameterDefaults.h"


""" % {
        "file_name": output_name,
        "file_preamble_info": file_preamble( input_file, "///" ),
        "header_guard": header_guard
    }


    ### === file content ===
    
    defaults_prefix = configDict[ "defaults_prefix" ]

    lastIndex = dataMatrix.shape[0] - 1
    empty_added = False

    for index, row in dataMatrix.iterrows():
        enum_value = row['enum_value']
        if len( enum_value ) < 1:
            raise Exception( "Missing 'enum_value'" )
    
        if enum_value.startswith( "#" ):
            if empty_added is True:
                continue
            empty_added = True
            h_output_content += """\n"""
            continue
        empty_added = False
        if enum_value.startswith( "//" ):
            h_output_content += """    %s\n""" % enum_value
            continue

        new_empty = True        
        ret = header_param_default_value( row, enum_value, defaults_prefix, "" )
        if len(ret) > 0:
            h_output_content += ret
            new_empty = False
        
        ret = header_param_default_value( row, enum_value, defaults_prefix, "cave" )
        if len(ret) > 0:
            h_output_content += ret
            new_empty = False
        
        ret = header_param_default_value( row, enum_value, defaults_prefix, "maze" )
        if len(ret) > 0:
            h_output_content += ret
            new_empty = False
        
        ret = header_param_default_value( row, enum_value, defaults_prefix, "herofortress" )
        if len(ret) > 0:
            h_output_content += ret
            new_empty = False

        if new_empty is True:
            empty_added = True


    ### === file footer ===

    h_output_content += \
"""

#endif /* %(header_guard)s */
""" % {
        "header_guard": header_guard
    }

    # _LOGGER.debug( "generated file:\n%s" + h_output_content )

    ### === writing to file ===
    os.makedirs( outputDir, exist_ok=True )
    outputFile = os.path.join( outputDir, output_name )
    with open( outputFile, "w" ) as enumFile:
        enumFile.write( h_output_content )
        

def header_param_default_value( row, enum_value, defaults_prefix, mode ):
    def_value = ""
    item_suffix = ""
    if len(mode) < 1:
        def_value = row[ 'default_value' ]
    else:
        def_value = row[ 'default_value_' + mode ]
        item_suffix = "_" + mode
    if len( def_value ) < 1:
        return ""

    item_value = defaults_prefix + enum_value.upper() + item_suffix.upper()
    entry_line = """%s""" % item_value
    indent = max( 60 - len( entry_line ), 5 )
    return """#define %s%s "%s"\n""" % (entry_line, " " * indent, def_value)

## =========================================================================
        
def generate_defaults_cmake( configDict, dataMatrix, outputDir ):
    _LOGGER.info( "generating defaults cmake file" )
    
    output_content = ""

    ### === file header ===
    input_file           = configDict[ "input_file" ]
    header_name          = "ParameterDefaults"
    output_name          = header_name + ".cmake"

    output_content += \
"""##
## %(file_name)s
##

%(file_preamble_info)s


""" % {
        "file_name": output_name,
        "file_preamble_info": file_preamble( input_file, "##" )
    }


    ### === file content ===
    
    defaults_prefix = configDict[ "defaults_prefix" ]

    lastIndex = dataMatrix.shape[0] - 1
    empty_added = False

    for index, row in dataMatrix.iterrows():
        enum_value = row['enum_value']
        if len( enum_value ) < 1:
            raise Exception( "Missing 'enum_value'" )
    
        if enum_value.startswith( "#" ):
            if empty_added is True:
                continue
            empty_added = True
            output_content += """\n"""
            continue
        empty_added = False
        if enum_value.startswith( "//" ):
            output_content += """    %s\n""" % enum_value
            continue

        new_empty = True        
        ret = cmake_param_default_value( row, enum_value, defaults_prefix, "" )
        if len(ret) > 0:
            output_content += ret
            new_empty = False
        
        ret = cmake_param_default_value( row, enum_value, defaults_prefix, "cave" )
        if len(ret) > 0:
            output_content += ret
            new_empty = False
        
        ret = cmake_param_default_value( row, enum_value, defaults_prefix, "maze" )
        if len(ret) > 0:
            output_content += ret
            new_empty = False
        
        ret = cmake_param_default_value( row, enum_value, defaults_prefix, "herofortress" )
        if len(ret) > 0:
            output_content += ret
            new_empty = False

        if new_empty is True:
            empty_added = True


    # _LOGGER.debug( "generated file:\n%s" + output_content )

    ### === writing to file ===
    os.makedirs( outputDir, exist_ok=True )
    outputFile = os.path.join( outputDir, output_name )
    with open( outputFile, "w" ) as enumFile:
        enumFile.write( output_content )


def cmake_param_default_value( row, enum_value, defaults_prefix, mode ):
    def_value = ""
    item_suffix = ""
    if len(mode) < 1:
        def_value = row[ 'default_value' ]
    else:
        def_value = row[ 'default_value_' + mode ]
        item_suffix = "_" + mode
    if len( def_value ) < 1:
        return ""
        
    parameter_name = parameter_name_from_row( row )

    item_value = defaults_prefix + enum_value.upper() + item_suffix.upper()
    entry_line = """%s""" % item_value
    indent = max( 60 - len( entry_line ), 5 )
    return """set( %s %s "%s" )        # %s\n""" % ( entry_line, " " * indent, def_value, parameter_name )


## =========================================================================


def file_preamble( input_file, line_comment ):
    info = \
"""%(line_comment)s
%(line_comment)s        File was automatically generated by: %(generator)s
%(line_comment)s        Input file: %(input_file)s
%(line_comment)s
%(line_comment)s        Any change in file will be lost.
%(line_comment)s""" % { 
                "line_comment": line_comment,
                "generator": generator_name(), 
                "input_file": input_dir_name( input_file )
            }
    return info


def generator_name():
    path = pathlib.Path( *pathlib.Path(__file__).parts[-4:] )
    return str(path)


def input_dir_name( input_file ):
    workdir = os.path.join( SCRIPT_DIR, ".." )
    path = os.path.relpath( input_file, workdir)
    return str(path)


def parameter_name_from_row( row ):
    parameter_name = row['parameter_name']
    if len( parameter_name ) > 0:
        return parameter_name
    
    enum_value = row['enum_value']
    return enum_value.lower()


# ===================================================================


## returns tuple ( config_dict, data_matrix )
def parse_csv( csv_path ):
    with open( csv_path, newline='' ) as csvfile:
        dataReader = csv.reader( csvfile, delimiter=',', quotechar='|' )
        
        configPart = False
        dataPart   = False
    
        configList = list()
        dataList   = list()
        
        for line in dataReader:
            # print( line )
            rawLine = ''.join( line )
            if len(rawLine) > 0:
                if rawLine == "Config:":
                    configPart = True
                    continue
                if rawLine == "Data:":
                    dataPart = True
                    continue
                # if rawLine[0] == '#':
                #    continue
                # if rawLine.startswith( "//" ):
                #    continue
            else:
                configPart = False
                dataPart   = False
                continue

            if configPart is True:
                configList.append( line )
            elif dataPart is True:
                dataList.append( line )

        configMatrix = create_matrix( configList )
        dataMatrix   = create_matrix( dataList )
        
        ## convert matrix to dict
        zip_iterator = zip( configMatrix["parameter"], configMatrix["value"] )
        configDict   = dict( zip_iterator )
        
        return ( configDict, dataMatrix )
    return ( None, None )


def create_matrix( dataList ):
    if len(dataList) < 1:
        raise Exception( "No data field found" )

    matrixHeader = dataList[ 0 ]
    matrixData   = DataFrame( dataList )

    ## remove redundant columns
    headerSize = len(matrixHeader)
    colsNum = len(matrixData.columns)
    if colsNum > headerSize:
        for _ in range( headerSize, colsNum ):
            colName = matrixData.columns[len(matrixData.columns)-1]
            matrixData.drop(colName, axis=1, inplace=True)

    matrixData.columns = matrixHeader

    matrixData = matrixData.iloc[1:]        ## remove first row (containing header labels)

    return matrixData


# ====================================================================
# ====================================================================


def configure_logger( level=None ):
    formatter = create_formatter()
    consoleHandler = logging.StreamHandler( stream=sys.stdout )
    consoleHandler.setFormatter( formatter )

    logging.root.addHandler( consoleHandler )
    if level is None:
        logging.root.setLevel( logging.INFO )
    else:
        logging.root.setLevel( level )


def create_formatter(loggerFormat=None):
    if loggerFormat is None:
        loggerFormat = '%(asctime)s,%(msecs)-3d %(levelname)-8s [%(filename)s:%(lineno)d] %(message)s'
#        loggerFormat = ('%(asctime)s,%(msecs)-3d %(levelname)-8s %(threadName)s %(name)s:%(funcName)s '
#                        '[%(filename)s:%(lineno)d] %(message)s')
    dateFormat = '%H:%M:%S'
#    dateFormat = '%Y-%m-%d %H:%M:%S'
    return logging.Formatter( loggerFormat, dateFormat )


def main():
    configure_logger( "INFO" )
    generate()
    _LOGGER.info( "done" )


if __name__ == '__main__':
    main()

