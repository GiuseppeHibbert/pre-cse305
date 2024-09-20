#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>  
#define Line_max 999
#define Field_max 999 

int float_equals(double a, double b, double tolerance) {
    return fabs(a - b) < tolerance;
}
  
int is_a_number(const char *string){
    char *string_to_strip=strdup(string); 
    if(string_to_strip==NULL){return 0;}
    while(isspace(*string_to_strip)){
         string_to_strip++;} // skipping leading spaces
    if(*string_to_strip=='\0'){
        return 0;} // checking if empty string after spaces
    for(int i=0;string_to_strip[i] != '\0'; i++) {
        if(!isdigit(string_to_strip[i])&&string_to_strip[i]!='-'&&string_to_strip[i]!='.'){
            free(string_to_strip);
            return 0;}}
    free(string_to_strip);
    return 1;}
double min_field(int field,int head_line, FILE *file){
    double min = DBL_MAX;// using the max value to compare with the current fields number 
    int first_line_field_val = 1;// first line
    char line_on[Line_max];
    rewind(file);
        while( fgets(line_on,Line_max, file)){
            if(first_line_field_val && head_line){
                first_line_field_val=0;
                continue;}// This skips the head line if its not a header line for descriptions
            char *copying_string = strdup(line_on);
            int current_field = 0;
            char *break_string =strtok(copying_string,",");
            while(break_string){
                if(current_field==field&&is_a_number(break_string)){
                    double value = atof(break_string);
                if(value<min){
                    min=value;}}
                    break_string=strtok(NULL,",");
                    current_field++;}
                    free(copying_string);}
                    return min;}
double max_field(int field,int head_line, FILE *file){
    double max = DBL_MIN;// using the min to use to compare with the current fields number 
    int first_line_field_val = 1;// first line
    char line_on[Line_max];
    rewind(file);
        while( fgets(line_on,Line_max, file)){
            if(first_line_field_val && head_line){
                first_line_field_val=0;
                continue;}// This skips the head line if its not a header line for descriptions
            char *copying_string = strdup(line_on);
            int current_field = 0;
            char *break_string =strtok(copying_string,",");// splitting up by ","
            while(break_string){
                if(current_field==field&&is_a_number(break_string)){
                    double value = atof(break_string);
                if(value>max){
                    max=value;}}
                    break_string=strtok(NULL,",");
                    current_field++;}
                    free(copying_string);}
                    return max;}          

double mean(int field,int head_line, FILE *file){
    int counter= 0;// counts the lines
    double total=0.0; // add amount in each line to this
    int first_line_field_val=1;
    char line_on[Line_max];
    rewind(file);
    while (fgets(line_on,Line_max,file)){
        if (first_line_field_val &&head_line){
            first_line_field_val =0;
            continue;}
        char *copying_string=strdup(line_on);
        int current_field=0;
        char *break_string =strtok(copying_string,",");// splitting up by ","
        while(break_string){
            if(current_field==field &&is_a_number(break_string)) {
                double value= atof(break_string);
                total+=value;
                counter++;}
            break_string =strtok(NULL, ",");
            current_field++;}
        free(copying_string);}
    if(counter==0){
        return 0;}
    double mean =(total/counter);
    return mean;}
// Helper function to remove the newline character if present
void remove_newline(char *line) {
    size_t len = strlen(line);
     if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[len - 1] = '\0';
    }
}

//Trim extra spaces
char *trim(char *str) {
    char *last;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0)
        return str;
    last = str + strlen(str) - 1;
    while (last > str && isspace((unsigned char)*last)) last--;
    *(last + 1) = 0;
    return str;
}

//parse the csv and returning field_count
int csv_parse(char *line, char *fields[], int max_fields) {
    int field_count = 0;
    int quotes = 0;
    char *field_start = line;
    for (char *ptr = line; *ptr != '\0'; ++ptr) {
        if (*ptr == '"') {
            quotes = !quotes;  // change quote state
        } else if (*ptr == ',' && !quotes) {
            *ptr = '\0';
            fields[field_count++] = trim(field_start);  // Trim the field value
            field_start = ptr + 1;
            if (field_count >= max_fields) {
                break;
            }
        }
    }
    fields[field_count++] = trim(field_start);  // Trimming last field
    return field_count;
}

int index_by_fieldname(const char *header_line, const char *field_name) {
    char *fields[Field_max];
    char *n_header = strdup(header_line);
    remove_newline(n_header);  // Remove newline from the header line
    int field_count = csv_parse(n_header, fields, Field_max);
    int field_index = -1;
    for (int i = 0; i < field_count; i++) {
        if (strcasecmp(fields[i], field_name) == 0) {
            field_index = i;
            break;
        }
    }
    free(n_header);
    return field_index;
}

// Finding the matching record with either the header or the field_index and printing it out
void find_matching_records(const char *field_spec, const char *target_value, int has_header, FILE *file) {
    char line[Line_max];
    int first_line = 1;
    int field_index = -1;
    double target_number = atof(target_value);  // Convert the target value to a number if it's numeric
    double tolerance = 0.001;  // Float comparison tolerance

    rewind(file);  // Reset file pointer to the beginning

    // If the file has a header, find the field index based on the field name
    if (has_header && fgets(line, sizeof(line), file)) {
        remove_newline(line);
        if (is_a_number(field_spec)) {
            field_index = atoi(field_spec);
        } else {
            field_index = index_by_fieldname(line, field_spec);  // Get index by field name
            if (field_index == -1) {
                fprintf(stderr, "Field name '%s' not found in the header.\n", field_spec);
                return;
            }
        }
        first_line = 0;  // Header processed, skip in data
    } else if (!has_header) {
        // field_spec as an index if there's no header
        if (is_a_number(field_spec)) {
            field_index = atoi(field_spec);
        } else {
            fprintf(stderr, "Field name specified but no header found.\n");
            return;
        }
    }
    while (fgets(line, sizeof(line), file)) {
        char final_line[Line_max];
        strcpy(final_line, line);  // Copy the whole line for output
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
        // Compare if both values are numbers
        if (is_a_number(target_value) && is_a_number(fields[field_index])) {
            double field_number = atof(fields[field_index]);
            if (float_equals(field_number, target_number, tolerance)) {
                printf("%s\n", final_line);  // Print the matching line
            }
        } else {  // Compare as strings
            if (strcmp(fields[field_index], target_value) == 0) {
                printf("%s\n", final_line);  // Print the matching line
            }
        }
    }
}
// Determines the record count of the file. It will return the number of lines if no header is present, and if the header is present it will return number of lines minus one.
void find_record_count(FILE *file, int argc, char *argv[]){
    bool headerPresent = false;
    bool recordsRequested = false;
    for (int i=1; i<argc-1; i++){ // Traverse the command-line arguments and search for three things, is header provided, is record provided, and are they both provided?
        if (strcmp(argv[i], "-h") == 0){
            headerPresent = true;
                    
        }
        if (strcmp(argv[i], "-r") == 0){
            recordsRequested = true;
                    
        }
    }
    if (!recordsRequested){
        printf("Records not requested.\n");
        return;
            
    }
    if (recordsRequested && headerPresent){ // W.I.P -- Traverse the whole file and for each line read in not including the header line add one, To be added the same function when the header is not present
        int lineCount = 0;
        char *line = NULL;
        line = fgets(line,Line_max,file);
                   while(line!=NULL){
                    line = fgets(line, Line_max, file);
                    lineCount++;
                            
                }
        printf("%d\n", lineCount);
    }
    else{
        int lineCount = 1;
        char *line = NULL;
        line = fgets(line,Line_max,file);
        while(line!=NULL){

            line = fgets(line,Line_max,file);
            lineCount++;
                    
        }
        printf("%d\n", lineCount);
            
    }
    printf("find_record_count success\n");
    
}
// Reads in the file's first line and determines the number of fields.
void display_field_count(FILE *file, int argc, char * argv[]){
    bool fieldCountRequested = false;
    for (int i=1; i<argc-1;i++){
        if (strcmp(argv[i], "-f") == 0){
            fieldCountRequested = true;
                    
        }
            
    }
    if (!fieldCountRequested){
        printf("Field count not requested.\n");
        return;
            
    }
    char cur;
    int fieldCount = 0;
    bool startquoteFlag = false;
    bool endquoteFlag = false;
    
    while((cur = fgetc(file))!='\0'){
        if(cur == '"' && !startquoteFlag && !endquoteFlag){ // String begun, disregard all commas until the end quote is present, followed by a comma.
            startquoteFlag = true;
            continue;
        }
        if(cur == '"' && startquoteFlag && !endquoteFlag){
            startquoteFlag = false;
            continue;
                    
        }
        if(cur == ',' && !startquoteFlag && !endquoteFlag){
            fieldCount++;
                    
        }
        else{
            continue;
                    
        }


            
    }
    if(cur=='\0'){
        fieldCount++;
            
    }
    printf("%d", fieldCount);

    printf("fieldCount success\n");
    
}

int h(int has_header, void *argv, FILE *file) {
    rewind(file);
    int indexVal;
    if(has_header == 1){
        if((is_a_number(argv) == 0) ){
            printf("Has Header but Given Integer Arg");
            return -1;
        } else {
            int indexVal;
            char headerLine[Line_max];
            fgets(headerLine, Line_max, file);
            indexVal = index_by_fieldname(headerLine, argv);
        }
    } else if(has_header == 0){
        if((is_a_number(argv) == 1)){
            printf("Has No Header but Given Text Arg");
            return -1;
        } else {
            int indexVal = atoi(argv);
        }
    }
    return indexVal;
}

/*int main(int argc, char *argv[]) {
    if (argc < 4) {
        perror("Invalid arguments provided.\n");
        return EXIT_FAILURE;
    }
    int field_index = atoi(argv[2]);
    char *target_value = argv[3];
    char *filename = argv[4];
    int has_header = (argc == 6 && strcmp(argv[5], "-h") == 0);
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;}
  find_matching_records(field_index, target_value, has_header, file);
  pclose(file);
  return 0;
*/
int main(int argc,char *argv[]){
    if(argc< 3){//program name, an option, filename, more options
        perror("Invalid arguments provided.\n");
        return EXIT_FAILURE;}
    //get filename by taking last argv value
    char *filename= argv[argc-1];  
    int has_header=0; //used to check if header is present or not
    FILE *file=fopen(filename,"r");
    if(!file){
        perror("Error opening file");
        return EXIT_FAILURE;}
    for(int i=1;i <argc -1;i++){// going through command line args except last one 
        if(strcmp(argv[i],"-min") ==0){
            char *lexVal = argv[i++];
            int field = h(has_header,lexVal, file);
            if (field == -1){
                return EXIT_FAILURE;
            }
            double min_val= min_field(field,has_header,file);
            printf("Min:%.2f\n",min_val);
        }
        else if (strcmp(argv[i],"-max")==0){
            char *lexVal = argv[i++];
            int field = h(has_header,lexVal, file);
            if (field == -1){
                return EXIT_FAILURE;
            }
            double max_val= max_field(field,has_header,file);
            printf("Max:%.2f\n",max_val);
        }
        else if(strcmp(argv[i],"-mean")==0){
            char *lexVal = argv[i++];
            int field = h(has_header,lexVal, file);
            if (field == -1){
                return EXIT_FAILURE;
            }
            double mean_val= mean(field,has_header,file);
            printf("Mean:%.2f\n",mean_val);
        }
        else if(strcmp(argv[i], "-records")== 0){
            if(has_header == 1){
                char *field_spec = argv[++i];  // Field name or index
                char *value = argv[++i];  // Value to match
                find_matching_records(field_spec,value, has_header,file);
            } else {
                int field= atoi(argv[++i]);
                char *value =argv[++i];
                find_matching_records(field,value, has_header,file);
            }
        }
        else if(strcmp(argv[i],"-h") == 0) {
            has_header=1;}
        }
    
    fclose(file);
    return EXIT_SUCCESS;
}
