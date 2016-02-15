/*
	Simple tool to concatenate and output files

	I created this for combining simple web application sources 
	into a single file. Large scale concat is easily done in .sh
	or .bat files but C has the nice benefit of being cross-platform
	with essentially no dependencies once built.

	Safety is not guaranteed, please backup your files.

	License: 	Public Domain
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CONCATR_MAX_LINE_LENGTH 512

// An example input file is provided at the end of this source

int main(int argc, char** argv){
    if(argc <= 1){
        printf("\nPlease provide file(s) for parsing\n\n\tExample: concatr.exe example.in\n");
        return -1;
    }

    char line[CONCATR_MAX_LINE_LENGTH];

    for(int i=1; i<argc; i++){

        FILE* file = fopen(argv[i], "r");
        FILE* output_file = NULL;
        FILE* in_file = NULL;

        if(file == NULL){
            printf("\nERROR: Failed to open file: %s\n", argv[i]);
            return -2;
        }

        int line_count = 0;
        while(fgets(line, CONCATR_MAX_LINE_LENGTH, file) != NULL){
        	// read file line by line
        	line_count++;
        	char* token;
        	switch(line[0]){
        		case '+':
        			token = strtok(line+1, " \n");

        			if(output_file != NULL){
        				in_file = fopen(token, "r");
                        if(in_file == NULL){
                            printf("ERROR: Failed to open file %s\n", token);
                            return -3;
                        }

                        // copy each line to output file
        				while(fgets(line, CONCATR_MAX_LINE_LENGTH, in_file) != NULL){
        					fprintf(output_file, "%s", line);
        				}
        				fclose(in_file);
        			}
        			break;
        		case '-':
                    // add a newline
        			if(output_file){
        				fprintf(output_file, "\n");
        			}
        			break;
        		case '=':
                    // close any current output files
        			if(output_file){
        				fclose(output_file);
        				output_file = NULL;
        			}

        			token = strtok(line+1, " \n");

                    // set our new output file target
        			output_file = fopen(token, "w");
        			break;
        		default:
        			break;
        	}
        }

        fclose(output_file);
        fclose(file);
    }

    return 0;
}



/*
	(APPENDIX A) Example input file:



	# all lines start with a command symbol, '#' is for comments and is ignored
	# any unknown symbol (example: whitespace) also results in an ignored line

	# '=' means this is the output file to be created
	= out_text.txt

	# '+' means this is a file to be concatenated
	+ sample1.txt

	# '-' means an empty line should be inserted
	-

	+ sample2.txt



*/