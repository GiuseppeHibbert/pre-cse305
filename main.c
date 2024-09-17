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
                continue;}// This skips the head line if its not a header line for descriptions
            char *copying_string = strdup(line_on);
            double = current_field = 0;
            char *break_string;
            while(copying_string<line_on[current_field]){
                copying_string = line_on[current_field];
                current_field+=1;}
                
                return 0;}}
double max_field(int field,int header, FILE *file){
    return 0;}
double mean(int field,int header, FILE *file){
    return 0;}

// Helper function to remove the newline character if present
void remove_newline(char *line) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}

//CSV parser that handles quoted fields
int csv_parse(char *line, char *fields[], int max_fields) {
    int field_count = 0;
    int quotes = 0;
    char *field_start = line;
    for (char *ptr = line; *ptr != '\0'; ++ptr) {
        if (*ptr == '"') {
            quotes = !quotes;  // Toggle quote state
        } else if (*ptr == ',' && !quotes) {
            *ptr = '\0';  // End of field
            fields[field_count++] = field_start;
            field_start = ptr + 1;
            if (field_count >= max_fields) {
                break;
            }
        }
    }
    fields[field_count++] = field_start;  // Add last field
    // Remove quotes from fields, if any
    for (int i = 0; i < field_count; i++) {
        if (fields[i][0] == '"' && fields[i][strlen(fields[i]) - 1] == '"') {
            fields[i][strlen(fields[i]) - 1] = '\0';  
            fields[i]++; 
        }
    }
    return field_count;
}

// Function to find and print records where the specified field matches the value.
void find_matching_records(int field_index, const char *target_value, int has_header, FILE *file) {
    char line[LINE_MAX];
    int first_line = 1;
    double target_number = atof(target_value);
    rewind(file); 
    while (fgets(line, sizeof(line), file)) {
        char final_line[Line_max];
        strcpy(final_line, line);// copying the whole line to return if match found
        remove_newline(line);
        if (first_line && has_header) {
            first_line = 0;
            continue;  // Skip the header
        }
        char *fields[Field_max];
        int field_count = csv_parse(line, fields, Field_max);
        if (field_index >= field_count) {
            fprintf(stderr, "Error: Field index out of range.\n");
            return;
        }
        double field_number = atof(fields[field_index]);
        if (field_number == target_number) {
            printf("%s\n", final_line);  // Print matching line
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        return EXIT_FAILURE;
    }
    int field_index = atoi(argv[2]);
    char *target_value = argv[3];
    char *filename = argv[4];
    int has_header = (argc == 6 && strcmp(argv[5], "-h") == 0);
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    find_matching_records(field_index, target_value, has_header, file);

    fclose(file);
    return 0;
}

