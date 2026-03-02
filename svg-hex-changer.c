/*svg-hex-changer.c*/

/**
  * @brief svg hex code changer for my friends here at Northwestern University :]
  *
  * Reads an entire svg into a buffer and then edits the buffer
  * in-place before writing it to a new file named accordingly. 
  *
  * @note Alex Kosowski
  *
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <dirent.h>

/**
  * @brief
  * Helper function that iterates through the buffer, replacing fromColor to toColor.
  * If no fromColor exists, it was initialized as "GGGGGG" and will be replaced by
  * the first instance of a hex code in the .svg
  *
  * @param buff pointer to character array (string) holding the contents of the .svg
  * @param fileSize integer holding the size of the svg, so we know how long to iterate
  * @param fromColor pointer to a character array (string) holding the color to change from
  * @param toColor pointer to a character array (string) holding the color to change to
  *
  * @return nothing.
  */
void swap_hexes(char* buff, int fileSize, char* fromColor, const char* toColor){
    
    //current color while iterating:
    char selectedColor[7];
    selectedColor[6] = '\0';

    //iteration:
    for (size_t i = 0; i < fileSize; i++){
        if (buff[i] == '#'){ //find where a hex is possibly mentioned
            
            //check if we have enough space for a hex here
            if (i+6>fileSize){
                break;
            }
            
            for (size_t j = 0; j < 6; j++){
                if (isxdigit(buff[i+1+j]) == 0){ //check if the next 6 chars are valid hex values
                    break; //break if not
                }
                if (fromColor[5] == 'G'){ //puts the first hex we save into fromColor, overwriting the last G when a valid hex is read.
                    fromColor[j] = buff[i+1+j];
                }
                selectedColor[j] = buff[i+1+j]; //save the hex we just read to selectedColor too, every time
            }
            for (size_t j=0; j < 5; j++){
                if (strcmp(selectedColor, fromColor) == 0){ //if the current hex we're reading is also the fromColor we need to change,
                    buff[i+1+j] = toColor[j];               //replace the hexes
                }
            }
        }   
    }
}

/**
  * @brief
  * Helper function that writes each individual newly-altered image file (.svg)
  *
  * @param argCount integer count of arguments passed via terminal
  * @param argValues array of strings (character arrays) of each argument passed via terminal
  * @param inFileName pointer to a string of the file name in the form `fileName.svg` 
  * @param curr index of last `.` in inFileName
  *
  * @return 0 on success, -1 if not.
  */
int process_svg_file(int argCount, char* argValues[], char* inFileName, int curr){

    FILE* inFile = fopen(inFileName, "r");
    if (inFile == NULL) {
        printf("**ERROR: unable to open input file.\n");
        return -1;
    }

    fseek(inFile, 0, SEEK_END);
    long fileSize = ftell(inFile); //seek to end of file and check note large it is
    fseek(inFile, 0, SEEK_SET); //seek back to start

    char* buff = (char*)malloc(fileSize+1); //buffer enough for entire file plus sting-terminating null \0
    if (buff == NULL){
        printf("**ERROR: initial buffer malloc failed, likely out of memory?\n");
        return -1;
    }

    fread(buff, 1, (size_t)fileSize, inFile); //read entire file into buffer
    buff[fileSize] = '\0';
    //By this line, buff is the svg as a string in full


    //initialize hex color variables -----
    //color to change to:
    char toColor[7];
    strncpy(toColor, argValues[argCount-1], 6);
    toColor[6] = '\0';

    //color to change from:
    char fromColor[7] = "GGGGGG\0";
    if (argCount == 4){
        strncpy(fromColor, argValues[2], 6);
    }

    //iterate!
    swap_hexes(buff, fileSize, fromColor, toColor);

    //make a string holding the output file name
    char* outFileName = (char*)malloc(strlen(inFileName)+strlen(argValues[argCount-1])+1+4+1); // sizeof("inFile-tocolor.svg\0")
    //if we have a file extension, set the last '.' to a null string terminator.
    if (curr != -1){
        inFileName[curr] = '\0';
    }
    //repeatedly append pieces of string into final output name
    strcpy(outFileName, inFileName);
    strcat(outFileName, "-");
    strcat(outFileName, toColor);
    strcat(outFileName, ".svg");
    
    FILE* outFile = fopen(outFileName, "w");
    if (outFile == NULL) {
        printf("**ERROR: unable to open input file.\n");
        return -1;
    }
    
    long writeSize = fwrite(buff, sizeof(char), fileSize, outFile);
    if (writeSize != fileSize){
        printf("**ERROR WRITING TO FILE: expected to write %ld bytes, but only wrote %ld\n", fileSize, writeSize);
        return -1;
    }
    
    fclose(inFile);
    fclose(outFile);
    free(outFileName);
        
    free(buff);
    return 0;
}

/**
  * @brief
  * Helper function checks if a given string is a hex code in the form 'FFFFFF'
  *
  * @param hex string to check
  *
  * @return true if hex, false if not 
  */
bool isxcode(char* hex){
    if (strlen(hex) != 6){
        return false;
    }
    for(int i = 0; i<strlen(hex); i++){
        if(isxdigit(hex[i]) == 0){
            return false;
        }
    }
    return true;
}

/**
  * @brief main()
  *
  * usage: program.exe folderName/fileName.svg [fromColor] toColor 
  *
  * - If no "fromColor" is given, instead replaces all instances
  *   of the first hex code in the .svg
  * - "fileName" must include .svg at the end, or else will be read like a folder
  * - "fromColor" and toColor must be hex in the form 'FFFFFF'
  *
  * @return 0 if successful, -1 if not.
  */
int main(int argc, char* argv[]){
    
    //Check for valid arg count
    if (argc != 3 && argc != 4){  
        printf("**USAGE: fileName.svg (fromColor) toColor\n");
        return -1;
    }
    
    //Check for valid toColor hex
    argv[2][6] = '\0'; //ignore anything after the first 6 chars, FFFFFF
    if (!isxcode(argv[2])){
        printf("**ERROR: '%s' is not a hex code in the form 'FFFFFF'\n", argv[2]);
        return -1;
    }

    //Check for valid fromColor hex, if present
    if (argc == 4){
        argv[3][6] = '\0'; //ignore anything after the first 6 chars, FFFFFF
        if (!isxcode(argv[3])){
            printf("**ERROR: '%s' is not a hex code in the form 'FFFFFF'\n", argv[3]);
            return -1;
        }
    }

    char* inFileName = argv[1];

    //store in curr the index of the last '.' in inputted file name, or -1 if none (folder name)
    int curr = -1;
    for (int i = 0; i < strlen(inFileName); i++){
        if(inFileName[i] == '.'){
            curr = i;
        }
    }

    if (curr == -1){ //directory case

        /*
        i dont got that in me lowkey
        */

    }
    else { //single file case
        if (strcmp(inFileName+curr, ".svg") != 0){
            printf("**ERROR: expected file extension to be \".svg\", but got \"%s\"\n", inFileName+curr);
            return -1;
        }
        return process_svg_file(argc, argv, inFileName, curr);
    }

    printf("**ERROR: Should not be able to get here (process_svg_file was not called)\n");
    return -1;
}