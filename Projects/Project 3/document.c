#include <stdio.h>
#include <string.h>
#include "document.h"
#define MAX_COMMAND_LEN 1024


int init_document(Document *doc, const char *name){
    int i,j;
    if(doc == NULL || name == NULL || strlen(name)> MAX_STR_SIZE){
        return FAILURE;
    }
    /* 0 paragraphs
     * Setting the document's name to the name given in the parameter
     */
    strcpy(doc->name, name);
    return SUCCESS;
}
int reset_document(Document *doc){
    /*
     * Checking whether the doc is null
     */
    if(doc == NULL) return FAILURE;
    doc -> number_of_paragraphs = 0;
    return SUCCESS;
}
int print_document(Document *doc){
    int i,j;
    /*
     * Checking whether document is NULL
     */
    if(doc == NULL) return FAILURE;

    /*
     * Procedure:
     * Print name and number of paragraphs in the document
     * Iterate through the current number of paragraphs in doc
     * For each paragraph, print the line for each paragraph
     *
     */
    printf("Document Name: \"%s\"\n",doc->name);

    printf("Number of Paragraphs: %d\n",doc->number_of_paragraphs);

    for(i =0; i <doc->number_of_paragraphs;i++){
        for(j = 0; j<doc->paragraphs[i].number_of_lines;j++){
            if(doc->paragraphs[i].lines[j][0] != '\0'){ printf("%s\n", doc->paragraphs[i].lines[j]); }
        }
        if(i != doc->number_of_paragraphs-1 && doc->paragraphs[i].lines[j-1][0] != '\0' && doc->paragraphs[i].number_of_lines != 0) {
            printf("\n");
        }
    }
    return SUCCESS;

}

void clearDoc(Document *doc){
    /*Clears all content from the doc*/
    int i,j,k;
    for(i =0; i <doc->number_of_paragraphs;i++){
        for(j = 0; j<doc->paragraphs[i].number_of_lines;j++){
            for(k = 0; k<MAX_STR_SIZE;k++){
                doc->paragraphs[i].lines[j][k] = ' ';
            }
        }
        doc->paragraphs[i].number_of_lines = 0;
    }
    reset_document(doc);
}

int add_paragraph_after(Document *doc, int paragraph_number){
    int i;
    Document temp;
    init_document(&temp, "temp");
    /*
     * Checking whether the document is null, the document has already exceed maximum characters
     * or paragraph number is greater than the number of paragraphs available
     *
     */
    if(doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }

    /* Idea: The idea is to shift each paragraph from the range [paragraph_number,last] down,
     * To do this, we can create a new document and start adding the ith paragraph from doc
     * to the i+1th paragraph
     */
    doc->number_of_paragraphs++;

    /*
     * We can only do this if paragraph_number is less than the current paragraphs available or else we'll be out of bounds
     */
    if(paragraph_number < doc->number_of_paragraphs){
        temp.number_of_paragraphs = doc->number_of_paragraphs;
        for (i = paragraph_number; i < temp.number_of_paragraphs; i++) {
            temp.paragraphs[i + 1] = doc->paragraphs[i];
        }

        for (i = paragraph_number; i < doc->number_of_paragraphs; i++) {
            doc->paragraphs[i] = temp.paragraphs[i];
        }
    }
    clearDoc(&temp);
    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number,const char *new_line){
    int i;
    Paragraph pg;

    /*
     * Checking if doc is null, paragraph number exceeds the number of paragraphs available,
     * paragraph has already exceed the max number of paragraphs, line number is larger than the available number of lines
     * new line is null
     */

    if(doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS ||
       paragraph_number > doc->number_of_paragraphs|| new_line == NULL ||
       doc->paragraphs[paragraph_number-1].number_of_lines >= MAX_PARAGRAPH_LINES) return FAILURE;

    /* Idea: It's similar to add_paragraph_after, we shift all the lines from [line_number, last] down
     *
     */
    if(line_number < doc->paragraphs[paragraph_number-1].number_of_lines){
        pg.number_of_lines = doc->paragraphs[paragraph_number-1].number_of_lines;
        for (i = line_number+1; i <= doc->paragraphs[paragraph_number-1].number_of_lines; i++) {
            strcpy(pg.lines[i], doc->paragraphs[paragraph_number-1].lines[i - 1]);
        }
        doc->paragraphs[paragraph_number-1].number_of_lines++;
        for (i = line_number+1; i < doc->paragraphs[paragraph_number-1].number_of_lines; i++) {
            strcpy(doc->paragraphs[paragraph_number-1].lines[i], pg.lines[i]);
        }
        strcpy(doc->paragraphs[paragraph_number-1].lines[line_number], new_line);
        return SUCCESS;
    }
    doc->paragraphs[paragraph_number-1].number_of_lines++;
    strcpy(doc->paragraphs[paragraph_number-1].lines[line_number], new_line);
    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines){
    /*
     * Checking if doc is NULL, number of lines is null or paragraph number is larger than the number of paragraphs available
     */
    if(doc == NULL || paragraph_number > doc->number_of_paragraphs ||
       number_of_lines == NULL){
        return FAILURE;
    }

    *number_of_lines = doc->paragraphs[paragraph_number-1].number_of_lines;
    return SUCCESS;
}
int get_number_lines(Document *doc, int *number_of_lines){
    int i, cnt = 0;
    if(doc == NULL || number_of_lines == NULL) return FAILURE;

    for(i = 0; i<doc->number_of_paragraphs;i++){
        cnt += doc->paragraphs[i].number_of_lines;
    }
    *number_of_lines = cnt;
    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line){
    int i;

    if(doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS ||
       paragraph_number > doc->number_of_paragraphs || new_line == NULL ||
       doc->paragraphs[paragraph_number-1].number_of_lines >= MAX_PARAGRAPH_LINES) {
        return FAILURE;
    }
    strcpy(doc->paragraphs[paragraph_number-1].lines[doc->paragraphs[paragraph_number-1].number_of_lines], new_line);
    doc->paragraphs[paragraph_number-1].number_of_lines++;
    return SUCCESS;
}

int remove_line(Document *doc, int paragraph_number, int line_number){
    int i;
    if(doc == NULL || paragraph_number > doc -> number_of_paragraphs ||
       line_number > doc-> paragraphs[paragraph_number-1].number_of_lines) {
        return FAILURE;
    }

    for(i = line_number-1;i<doc->paragraphs[paragraph_number-1].number_of_lines;i++){
        strcpy(doc->paragraphs[paragraph_number-1].lines[i],doc->paragraphs[paragraph_number-1].lines[i+1]);
    }
    doc->paragraphs[paragraph_number-1].number_of_lines--;

    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
    int i, cnt = 1;
    if(doc == NULL || data == NULL || data_lines == 0) return FAILURE;

    add_paragraph_after(doc,0);
    for(i = 0; i < data_lines; i++) {
        if(data[i][0] == '\0' || data[i][0] == '\n' || data[i][0] == ' ') {
            add_paragraph_after(doc, cnt);
            cnt++;
        }
        append_line(doc, cnt, data[i]);
    }

    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j, idx, cnt, m;
    char str[MAX_STR_SIZE], *line, *pos;
    Paragraph *ptr;

    if (doc == NULL || target == NULL || replacement == NULL) return FAILURE;

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        ptr = &doc->paragraphs[i];
        for (j = 0; j < ptr->number_of_lines; j++) {
            line = ptr->lines[j];
            pos = NULL;
            cnt = 0;

            str[0] = '\0';

            while ((pos = strstr(line, target)) != NULL) {
                idx = pos - line;
                strncat(str, line, idx);
                cnt += idx;

                strncat(str, replacement, strlen(replacement));
                cnt += strlen(replacement);

                line = pos + strlen(target);
            }

            strncat(str, line, strlen(line));

            strncpy(ptr->lines[j], str, MAX_STR_SIZE);
        }
    }

    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    /*This function highlights each occurence of the word in the doc
    * Idea: We create the string with the string highlighted and
    * replace every word with the string we highlighted earlier
    * with the help of target, this simplified things
    */
    char new_string[MAX_STR_SIZE + 1];
    if(doc == NULL || target == NULL) return FAILURE;

    strcpy(new_string, HIGHLIGHT_START_STR);
    strcat(new_string, target);
    strcat(new_string, HIGHLIGHT_END_STR);
    replace_text(doc, target, new_string);

    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {
    int i, j, k,l, m,cnt = 0,n;
    char str[MAX_STR_SIZE], temp[MAX_STR_SIZE];
    Paragraph *ptr;
    if(doc == NULL || target == NULL){
        return FAILURE;
    }
    replace_text(doc, target, "");

    return SUCCESS;
}

int load_file(Document *doc, const char *filename) {
    FILE *fp;
    int cnt = 1;
    char command[MAX_COMMAND_LEN];
    char *str;
    fp = fopen(filename, "r");
    if(doc == NULL || filename == NULL || doc-> number_of_paragraphs >= MAX_PARAGRAPHS || fp == NULL) {
        return FAILURE;
    }

    add_paragraph_after(doc, 0);

    while(fgets(command, sizeof(command), fp)){
        str = command + strlen(command) -1;
        *str = '\0';
        if(command[0] == '\0' || command[0] == ' ' || command[0] == '\n'){
            add_paragraph_after(doc, cnt);
            cnt++;
        }
        append_line(doc, cnt, command);
    }
    fclose(fp);
    return SUCCESS;

}
int save_document(Document *doc, const char *filename) {
    FILE *fp;
    int i,j;
    fp = fopen(filename, "w");
    if(doc == NULL || filename == NULL || fp == NULL) {
        return FAILURE;
    }
    for(i = 0; i < doc -> number_of_paragraphs; i++) {
        for(j = 0; j < doc -> paragraphs[i].number_of_lines; j++) {
            if(doc->paragraphs[i].lines[j][0] != '\0'){ fprintf(fp,"%s\n", doc->paragraphs[i].lines[j]); }
        }
        if(i != doc->number_of_paragraphs-1 && doc->paragraphs[i].lines[j-1][0] != '\0' && doc->paragraphs[i].number_of_lines != 0) {
            fprintf(fp,"\n");
        }
    }

    fclose(fp);
    return SUCCESS;

}
