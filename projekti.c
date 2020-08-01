#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "projekti.h"

int main(){

    printf("Select one from the options below\n");
    printf(" A: Add student input format\t[A ID lastname firstname]\n");
    printf(" U: Update round point format [U ID 1 95]\n");
    printf(" L: Print student round point input format [L]\n");
    printf(" W: Save result to the file input format [W filename]\n");
    printf(" O: Load result from the file input format [O filename]\n");
    printf(" Q: Quit\n");

    printf("What do you want to do? ");
    char input[80];
    fgets(input, 80, stdin);

    // input variables after splitting max 4
    char first[1];
    char second[78];
    char third[76];
    char fourth[74];

    unsigned int count = 0;
    for (int i=0; i<strlen(input); i++){
        if (input[i] == ' ')
            count++;
    }
    count++;


    Student* collection;
    collection = malloc(sizeof(Student));
    collection->id[0] = '\0';
    //memset(collection, 0, sizeof(*collection));



    while (strlen(input)>0){

        switch(toupper(input[0])){
            case 'A':
                printf("%s\n", input);
                if (count < 4){
                    printf("wrong input for Adding student to the array, [A id lastname firstname]\n");
                    break;
                }
                sscanf(input, "%s %s %s %s", first, second, third, fourth);

                Student *newarray = add_student(collection, second, third, fourth);
                if (!newarray) {
                    printf("ERROR - new collection is NULL\n");
                    return 1;
                }
                collection = newarray;
                break;

            case 'U':
                if (count < 4){
                    printf("wrong input to update scores, [U id (1-6) (0-100)]\n");
                    break;
                }
                sscanf(input, "%s %s %s %s", first, second, third, fourth);
//                printf("%s %s %d %d\n", first, second, atol(third), atol(fourth));
                if (atol(third) < 1 || atol(third) > 6)  {
                    printf("Wrong input round number, should be 1-6\n");
                    break;
                }
                if (atol(fourth) < 0 || atol(fourth) > 100)  {
                    printf("Wrong input score, range is 0-100\n");
                    break;
                }
                if ( (atol(fourth)==0) && ( strcmp(fourth, "0")!=0 ) ){
                    printf("Wrong input score, input should be number 0-100");
                    break;
                }
                collection = update_points(collection, second, atol(third), atol(fourth));
                break;

            case 'L':
                print_points(collection);
                break;

            case 'W':
                if (count < 2){
                    printf("wrong input for saving data to file, add filename [W filename]\n");
                    break;
                }
                sscanf(input, "%s %s ", first, second);
                save_results(collection, second);
                break;

            case 'O':
                if (count < 2){
                    printf("wrong input for loading data from file, add filename [O filename]\n");
                    break;
                }
                sscanf(input, "%s %s ", first, second);

                Student *loaded = load_results(second);
                if (loaded == NULL) {
                    printf("ERROR - Loading the data failed\n");
                    break;
                }
                collection = loaded;
                break;

            case 'Q':
                freeup(collection);
                break;
            default:
                printf("Wrong input, select among those (A U L W O Q)\n");
        }

        if (toupper(input[0]) == 'Q'){
            break;
        }

        printf("What do you want to do? ");
        fgets(input, 80, stdin);
    }

    return 0;
}

Student *add_student(Student* collection, const char* p_id, const char* p_lastname, const char* p_firstname){

    int count = 0;
    int i = 0;
    while(collection[i].id[0] != '\0'){
        count++;
        i++;
    }

    collection = realloc(collection, (count+2)*sizeof(Student));

    if (strlen(p_id)>9)
        return NULL;

    for (int i=0; i<strlen(p_id); i++){
        if (isdigit(p_id[i]) == 0){
            printf("Wrong input of student ID\n");
            return NULL;
        }
    }
    for (int i=0; i<strlen(p_firstname); i++){
        if (isalpha(p_firstname[i]) == 0){
            printf("Wrong input of lastname\n");
            return NULL;
        }
    }
    for (int i=0;i<strlen(p_lastname); i++){
        if (isalpha(p_lastname[i]) == 0){
            printf("Wrong input of firstname\n");
            return NULL;
        }
    }

    strcpy(collection[count].id, p_id);
    collection[count].firstname = malloc(strlen(p_firstname)+1);
    strcpy(collection[count].firstname, p_firstname);
    collection[count].lastname = malloc(strlen(p_lastname)+1);
    strcpy(collection[count].lastname, p_lastname);
    for (int j=0;j<6;j++){
        collection[count].scores[j] = 0;
    }
    collection[count].total = 0;

    collection[count+1].id[0] = '\0';

    printf("New student added to the array successfully\n");
    return collection;
}

Student *update_points(Student* collection, const char* p_id, const int round, const int score){

    int count = 0;
    int i = 0;
    while(collection[i].id[0] != '\0'){
        count++;
        i++;
    }

    int isFound = 0;
    for (i=0; i<count; i++){
        if (strcmp(collection[i].id, p_id) == 0 ){
            collection[i].total -= collection[i].scores[round-1]; //reduce original score, if there was any point, from total score
            collection[i].scores[round-1] = score;
            collection[i].total += score;
            isFound = 1;
        }
    }

    if (isFound){
        printf("The score is updated successfully\n");
    } else {
        printf("ID was not found in student array\n");
    }

    return collection;
}

void print_points(Student* collection){

    int count = 0;
    int i = 0;
    while(collection[i].id[0] != '\0'){
        count++;
        i++;
    }

    if (count == 0){
        printf("student array is empty");
        return;
    }

    qsort(collection, count, sizeof(Student), sort_descending);

    for (i = 0; i<count; i++) {
        printf("ID:%s  Name: %s %s  Round scores: [", collection[i].id, collection[i].firstname, collection[i].lastname);

        for (int j =0; j<5; j++){
            printf("R%d: %d, ", (j+1), collection[i].scores[j]);
        }
        printf("R6: %d], Total score: %d\n", collection[i].scores[5], collection[i].total);
    }

    printf("Print the student data\n");
}

void save_results(Student *collection, const char* filename){

    FILE *f = fopen(filename, "wb");
    if(!f){
        printf("Failed to open file\n");
        return;
    }

    //int i = 0; //--> ?
    while(collection->id[0] != '\0'){
        fwrite(collection, sizeof(Student), 1, f);
        collection++;
      //  i++; //--> ?
    }

    printf("Result was saved to the %s\n", filename);
    fclose(f);
}


Student *load_results(const char* filename){

    FILE *f;
    f = fopen(filename, "rb");

    if(!f){
        printf("Loading file failed");
        return NULL;
    }


    Student *student_arr;
    student_arr = malloc(sizeof(Student));

    int i = 0;
    Student student;


    while( fread(&student, sizeof(Student), 1, f) == 1 ){

        student_arr = realloc(student_arr, (i+1)*sizeof(Student) );

        strcpy(student_arr[i].id, student.id);
        printf("%s\n", student.firstname);
        student_arr[i].lastname = malloc( strlen(student.lastname)+1 );
        strcpy(student_arr[i].lastname, student.lastname);
        student_arr[i].firstname = malloc( strlen(student.firstname)+1 );
        strcpy(student_arr[i].firstname, student.firstname);

        for (int j=0; j<6; j++){
            student_arr[i].scores[j] = student.scores[j];
        }
        student_arr[i].total = student.total;

        i++;
    }

    printf("element number %d\n", i);
    student_arr[i].id[0] = '\0';
    fclose(f);
    printf("result was loaded n");
    return student_arr;

}

void freeup(Student *collection){

    int count = 0;
    int i = 0;
    while(collection[i].id[0] != '\0'){
        count++;
        i++;
    }

    for (unsigned int i = 0; i < count; i++) {
        free(collection[i].firstname);
        free(collection[i].lastname);
    }
    free(collection);
    printf("free the allocated memory\n");
}

int sort_descending(const void * a, const void * b){
    return ((const Student *)b)->total - ((const Student *)a)->total ;
}
