/*
    Name: Macbrandon
    UID: 120417122
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sysexits.h>
#include "document.h"


#define MAX_COMMAND_LEN 1024
/*
* This function removes leading and trailing whitespces from a string
*/
char *trim_whitespace(char *str) {
    char *end;


    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;


    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    return str;
}

int main(int argc, char *argv[]) {
    Document doc;
    char command[MAX_COMMAND_LEN];
    char new_line[MAX_COMMAND_LEN];
    char filename[81];
    int paragraph_num, line_num;
    char *line_start, *comm;
    char target[1024], replacement[1024];
    FILE *input;
    init_document(&doc, "main_document");

    /*
    * Parse command line arguments and check whether the command line is redirecting the file or including it as an argument. 
    */
    if (argc == 2) {
        input = freopen(argv[1], "r", stdin);
        if (input == NULL) {
            fprintf(stderr, "%s cannot be opened.\n", argv[1]);
            exit(EX_OSERR);
        }
    }
    else if (argc == 1){
        input = stdin;
    }
    else{
        fprintf(stderr, "Usage: user_interface\nUsage: user_interface <filename>\n");
        exit(EX_USAGE);
    }

    /*
    * Reading the file until we reach the end of the file.
    */
    while (fgets(command, sizeof(command), input)) {

        comm = trim_whitespace(command);

        /* If command line argument redirects a file. We prompt for input*/
        if(argc == 1) printf("> ");

        /* Ignore comments and blank lines*/
        if (*comm == '#' || *comm  == '\0' || *comm  == '\n') {
            continue;
        }

        /*Handling the add_paragraph_after command*/
        if (strncmp(comm, "add_paragraph_after", 19) == 0) {
            if (sscanf(comm, "add_paragraph_after %d", &paragraph_num) != 1 || paragraph_num < 0) {
                printf("Invalid Command\n");
            } else if (add_paragraph_after(&doc, paragraph_num) == FAILURE) {
                printf("add_paragraph_after failed\n");
            }
        }
        /*Handling the add_line_after command*/
        else if (strncmp(comm, "add_line_after", 14) == 0) {
            line_start = strchr(comm, '*');
            if (line_start == NULL) {

                printf("Invalid Command\n");
            } else {
                if (sscanf(comm, "add_line_after %d %d", &paragraph_num, &line_num) != 2 || line_start == NULL || paragraph_num <= 0 || line_num < 0) {
                    printf("Invalid Command\n");
                } else {

                    strcpy(new_line, line_start + 1);
                    if (add_line_after(&doc, paragraph_num, line_num, new_line) == FAILURE) {
                        printf("add_line_after failed\n");
                    }
                }
            }
        }
        /*Handling the print_document command*/
        else if (strncmp(comm, "print_document", 14) == 0) {
            if (strlen(comm) > 14) {
                printf("Invalid Command\n");
            } else {
                if(print_document(&doc)==FAILURE){
                    printf("print_document failed\n");
                }
            }
        }
        /*Handling the quit and exit command*/
        else if (strncmp(comm, "quit", 4) == 0 || strncmp(comm, "exit", 4) == 0) {
            if (strlen(comm) > 4) {
                printf("Invalid Command\n");
            } else {
                exit(0);
            }
        }
        /*Handling the append_line command*/
        else if (strncmp(comm, "append_line", 11) == 0) {
            line_start = strchr(comm, '*');
            if (sscanf(comm, "append_line %d", &paragraph_num) != 1 || paragraph_num < 0 || line_start == NULL) {
                printf("Invalid Command\n");
            } 
            else {
                strcpy(new_line, line_start + 1);
                if (append_line(&doc, paragraph_num, new_line) == FAILURE) {
                    printf("append_line failed\n");
                }

            }
        }
        /*Handling the remove_line command*/
        else if (strncmp(comm, "remove_line", 11) == 0) {
            if (sscanf(comm, "remove_line %d %d", &paragraph_num, &line_num) != 2 || paragraph_num <= 0 || line_num <= 0) {
                printf("Invalid Command\n");
            } else if (remove_line(&doc, paragraph_num, line_num) == FAILURE) {
                printf("remove_line failed\n");
            }
        }
        /*Handling the load_file command*/
        else if (strncmp(comm, "load_file", 9) == 0) {
            if (sscanf(comm, "load_file %80s", filename) != 1) {
                printf("Invalid Command\n");
            } else if (load_file(&doc, filename) == FAILURE) {
                printf("load_file failed\n");
            }
        }
        /*Handling the replace_text command*/
        else if (strncmp(comm, "replace_text", 12) == 0) {
            if (sscanf(comm, "replace_text \"%[^\"]\" \"%[^\"]\"", target, replacement) != 2) {
                printf("Invalid Command\n");
            } else if (replace_text(&doc, target, replacement) == FAILURE) {
                printf("replace_text failed\n");
            }
        }
        /*Handling the highlight_text command*/
        else if (strncmp(comm, "highlight_text", 14) == 0) {
            if (sscanf(comm, "highlight_text \"%[^\"]\"", target) != 1) {
                printf("Invalid Command\n");
            } else if (highlight_text(&doc, target) == FAILURE) {
                printf("highlight_text failed\n");
            }
        }
        /*Handling the remove_text command*/
        else if (strncmp(comm, "remove_text", 11) == 0) {
            if (sscanf(comm, "remove_text \"%[^\"]\"", target) != 1) {
                printf("Invalid Command\n");
            } else if (remove_text(&doc, target) == FAILURE) {
                printf("remove_text failed\n");
            }
        }
        /*Handling the reset_document command*/
        else if (strncmp(comm, "reset_document", 14) == 0) {
            if (strlen(comm) > 14) {
                printf("Invalid Command\n");
            } else {
                reset_document(&doc);
            }
        }
        /*Handling the save_document command*/
        else if(strncmp(comm, "save_document", 13) == 0){
            if(sscanf(comm, "save_document %80s", filename) != 1){
                printf("Invalid Command\n");
            }
            else if (save_document(&doc, filename) == FAILURE) {
                printf("save_document failed\n");
            }
        }
        else {
            printf("Invalid Command\n");
        }
    }

}