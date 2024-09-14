#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Line_max 999
#define Field_max 999 


int is_a_number(const char *string){//for max min mean fields
    for(int i=0;string[i]!='\0';i++){// iterating through string
        if(!isdigit(str[i]) && str[i] != '-' && str[i] !='.')
            return 0;}
    return 1;} // is a num if returns 0.
double min_field(int field,int header, FILE *file){
    return 0;
}
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
