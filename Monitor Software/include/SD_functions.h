/************************************************************************
                               SD_functions.h
                        ECD 524 - FSAE DATA AQUISITION
                        Kyle Ferris, Markus Higgins 
                                3/3/2025

This file contains the includes, constants, types,  and function prototypes 
necessary to run the main program on the systems peripheral devices that
interact directly with the sensors in the system.   
*************************************************************************/

#ifndef SD_H
#define SD_H

    #define REASSIGN_PINS

    /************************************************************************
                                    INCLUDES
    *************************************************************************/
    #include "FS.h"
    #include "SD.h"
    #include "SPI.h"

    /************************************************************************
                                    PROTOTYPES
    *************************************************************************/
    void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
    void createDir(fs::FS &fs, const char *path);
    void removeDir(fs::FS &fs, const char *path); 
    void readFile(fs::FS &fs, const char *path); 
    void writeFile(fs::FS &fs, const char *path, const char *message);
    void appendFile(fs::FS &fs, const char *path, const char *message);
    void renameFile(fs::FS &fs, const char *path1, const char *path2);
    void deleteFile(fs::FS &fs, const char *path);
    void testFileIO(fs::FS &fs, const char *path);

#endif 
