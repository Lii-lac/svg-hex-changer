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
  * Helper function that actually writes each individual newly-altered image file (.svg)
  *
  * @param argCount integer count of arguments passed via terminal
  * @param argValues array of strings (character arrays) of each argument passed via terminal
  * @param inFileName pointer to a string of the file name in the form `fileName.svg` 
  * @param curr index of last `.` in inFileName
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
    //printf("%s", buff);

    if (argCount == 3){ //no given hex, we need to get the first on our own

        char toColor[7];
        strncpy(toColor, argValues[2], 6);
        toColor[6] = '\0';

        char fromColor[7] = "GGGGGG\0"; //initialize to all Gs because it's an impossible hex

        char selectedColor[7];
        selectedColor[6] = '\0';
        
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
        
        //if we have a file extention, set the last '.' to a null string terminator.
        if (curr != -1){
            inFileName[curr] = '\0';
        }

        char* outFileName = (char*)malloc(strlen(inFileName)+strlen(argValues[2])+1+4+1); // sizeof("inFile-tocolor.svg\0")
        //repeatedly append pieces of string into final output name
        strcpy(outFileName, inFileName);
        strcat(outFileName, "-");
        strcat(outFileName, toColor);
        strcat(outFileName, ".svg");
        
        fclose(inFile);
        

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
        fclose(outFile);
        free(outFileName);
    }

    if (argCount == 4){ // passed two hexes:
        char toColor[7];
        strncpy(toColor, argValues[3], 6);
        toColor[6] = '\0';

        char fromColor[7];
        strncpy(fromColor, argValues[2], 6);
        fromColor[6] = '\0';

        char selectedColor[7];
        selectedColor[6] = '\0';
        
        for (size_t i = 0; i < fileSize; i++){
            if (buff[i] == '#'){
                //check if we have enough space for a hex here
                if (i+6>fileSize){
                    break;
                }
                //check if the next 6 chars are hex values
                for (size_t j = 0; j < 6; j++){
                    if (isxdigit(buff[i+1+j]) == 0){
                        break; //break if not a hex value
                    }
                    selectedColor[j] = buff[i+1+j];
                }
                for (size_t j=0; j < 5; j++){
                    if (strcmp(selectedColor, fromColor) == 0){
                        buff[i+1+j] = toColor[j];
                    }
                }
            }   
        }       

        //if we have a file extention, set the last '.' to a null string terminator.
        if (curr != -1){
            inFileName[curr] = '\0';
        }

        char* outFileName = (char*)malloc(strlen(inFileName)+strlen(argValues[3])+1+4+1); // sizeof("inFile-tocolor.svg\0")
        //repeatedly append pieces of string into final output name
        strcpy(outFileName, inFileName);
        strcat(outFileName, "-");
        strcat(outFileName, toColor);
        strcat(outFileName, ".svg");
        
        fclose(inFile);

        FILE* outFile = fopen(outFileName, "w");
        if (outFile == NULL) {
            printf("**ERROR: unable to open output file.\n");
            return -1;
        }

        
        long writeSize = fwrite(buff, sizeof(char), fileSize, outFile);
        if (writeSize != fileSize){
            printf("**ERROR WRITING TO FILE: expected to write %ld bytes, but only wrote %ld\n", fileSize, writeSize);
            return -1;
        }

        fclose(outFile);
        free(outFileName);
    }
    free(buff);
    return 0;
}

/**
  * @brief
  * Helper function checks if a given string is a hex code in the form 'FFFFFF'
  *
  * @param hex string to check
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

/*
* ./a.out filename (fromcolor) tocolor
* fromcolor/X, X to go to first hex code
*
* returns 0 on success and -1 on failure
*/
int main(int argc, char* argv[]){
    
    if (argc != 3 && argc != 4){  //check for valid arg count
        printf("**USAGE: fileName.svg (fromColor) toColor\n");
        return -1;
    }
    
    argv[2][6] = '\0'; //ignore anything after the first 6 chars, FFFFFF
    if (!isxcode(argv[2])){
        printf("**ERROR: '%s' is not a hex code in the form 'FFFFFF'\n", argv[2]);
        return -1;
    }

    if (argc == 4){
        argv[3][6] = '\0'; //ignore anything after the first 6 chars, FFFFFF
        if (!isxcode(argv[3])){
            printf("**ERROR: '%s' is not a hex code in the form 'FFFFFF'\n", argv[3]);
            return -1;
        }
    }


    char* inFileName = argv[1];

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


    } else{ //single file case

        process_svg_file(argc, argv, inFileName, curr);
    }
    return 1;
}