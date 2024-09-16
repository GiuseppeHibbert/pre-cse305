#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#define Line_max 999
#define Field_max 999 


int is_a_number(const char *string){//for max min mean fields
    for(int i=0;string[i]!='\0';i++){// iterating through string
        if(!isdigit(str[i]) && str[i] != '-' && str[i] !='.')
            return 0;}
    return 1;} // is a num if returns 0.
double min_field(int field,int head_line, FILE *file){
    double min = DBL_MAX;// using the max to get compare with the current fields number 
    int first_line_field_val = 1;// first line
    char line_on[Line_max];
    rewind(file)
        while( fgets(line_on,Line_max, file)){
            if(first_line_field_val && head_line){
                first_line_field_val=0;
                continue;}// This skips the head line if its not
            char *copying = strdup(line_on);
            double = current_field = 0;
            char * break_string; 
                return 0;
        }}
double max_field(int field,int header, FILE *file){
    return 0;
}
double mean(int field,int header, FILE *file){
    return 0;}



int main(int argc, char * argv[]){
    // load the csv file in for input.
    printf("Hello world.\n");
    return 0;
}
