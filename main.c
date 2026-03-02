#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <dirent.h>




int process_svg_file(int argc, char* argv[], char* inFileName){
    
    if (argc != 3 && argc != 4){
        printf("**USAGE: fileName.svg (fromColor) toColor\n");
        return -1;
    }

    FILE* infile = fopen(inFileName, "r");
    if (infile == NULL) {
        printf("**ERROR: unable to open input file.\n");
        return -1;
    }

    fseek(infile, 0, SEEK_END);
    long fileSize = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    char* buff = (char*)malloc(fileSize+1);
    if (buff == NULL){
        printf("**ERROR: initial buffer malloc failed, out of memory?\n");
        return -1;
    }

    fread(buff, 1, (size_t)fileSize, infile);
    buff[fileSize] = '\0';

    //By this line, buff is the svg as a string in full
    //printf("%s", buff);
    if (argc == 3){ //no given hex, we need to get the first on our own
        char toColor[7];
        strncpy(toColor, argv[2], 6);
        toColor[6] = '\0';

        char fromColor[7] = "GGGGGG\0";

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
                    if (fromColor[5] == 'G'){ //put the first hex we hit into fromColor
                        fromColor[j] = buff[i+1+j];
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
            
        char* outFileName = (char*)malloc(strlen(argv[1])+strlen(argv[2])+1+1); // [infile-tocolor.svg\0]
        strcat(outFileName, inFileName);
        strcat(outFileName, "-");
        strcat(outFileName, toColor);
        strcat(outFileName, ".svg");
        
        fclose(infile);
        

        FILE* outfile = fopen(outFileName, "w");
        if (outfile == NULL) {
            printf("**ERROR: unable to open input file.\n");
            return -1;
        }

        
        long writeSize = fwrite(buff, sizeof(char), fileSize, outfile);
        if (writeSize != fileSize){
            printf("**ERROR WRITING TO FILE: expected to write %ld bytes, but only wrote %ld\n", fileSize, writeSize);
            return -1;
        }
    }

    if (argc == 4){ // given two hexes
        char toColor[7];
        strncpy(toColor, argv[3], 6);
        toColor[6] = '\0';

        char fromColor[7];
        strncpy(fromColor, argv[2], 6);
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
            
        char* outFileName = (char*)malloc(strlen(argv[1])+strlen(argv[3])+1+1); // [infile-tocolor.svg\0]
        strcat(outFileName, inFileName);
        strcat(outFileName, "-");
        strcat(outFileName, toColor);
        strcat(outFileName, ".svg");
        
        fclose(infile);
        

        FILE* outfile = fopen(outFileName, "w");
        if (outfile == NULL) {
            printf("**ERROR: unable to open input file.\n");
            return -1;
        }

        
        long writeSize = fwrite(buff, sizeof(char), fileSize, outfile);
        if (writeSize != fileSize){
            printf("**ERROR WRITING TO FILE: expected to write %ld bytes, but only wrote %ld\n", fileSize, writeSize);
            return -1;
        }
    }

    return 0;
}

/*
* ./a.out filename (fromcolor) tocolor
* fromcolor/X, X to go to first hex code
*
* returns 0 on success and -1 on failure
*/
int main(int argc, char* argv[]){
    
    char* inFileName = argv[1];
    int curr = -1;
    for (int i = 0; i < strlen(inFileName); i++){
        if(inFileName[i] == '.'){
            curr = i;
        }
    }
    if (curr != -1){
        inFileName[curr] = '\0';
    }

    if (curr == -1){ //directory case

        /*
        i dont got that in me lowkey
        */


    } else{ //single file case
        

        int fileNameLen = strlen(inFileName) + 4 + 1;
        char inFileNameSvg[fileNameLen];
        inFileNameSvg[0] = '\0';
        strcat(inFileNameSvg, inFileName);
        strcat(inFileNameSvg, ".svg");
        
        process_svg_file(argc, argv, inFileName);
    }
    
}