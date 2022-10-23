/**
* (basic description of the program or class)
*
* Completion time: (estimation of hours spent on this program)
*
* @author Paul Carmichael, (anyone else, e.g., Acuna, whose code you used)
* @version 1.0 10/23/2022
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS
#define STR_LENGTH 1024


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef enum {
    SER = 0, EGR = 1, CSE = 2, EEE = 3
} Subject;

struct CourseNode{
    Subject subject;
    int subject_number;
    char teacher[STR_LENGTH];
    int credits;

    int credits_tot;
    struct CourseNode* next;
};

typedef enum {True, False} boolean;

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

//place to store course information
struct CourseNode* course_collection = NULL;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void branching(char option);
void course_insert(struct CourseNode* course_in);
void course_drop(Subject subject, int subject_number);
void schedule_load();
void schedule_save();
const char* getSubject(Subject subject);

////////////////////////////////////////////////////////////////////////////////
//main entry point. Starts the program by displaying a welcome and beginning an 
//input loop that displays a menu and processes user input. Pressing q quits.      
int main() {
	char input_buffer;
    course_collection->credits_tot = 0;
	printf("\n\nWelcome to ASU Class Schedule\n");

    schedule_load();

	//menu and input loop
	do {
		printf("\nMenu Options\n");
		printf("------------------------------------------------------\n");
		printf("a: Add a class\n");
		printf("d: Drop a class\n");
		printf("s: Show your classes\n");
		printf("q: Quit\n");
		printf("\nTotal Credits: %d\n\n", course_collection->credits_tot);
		printf("Please enter a choice ---> ");

		scanf(" %c", &input_buffer);

		branching(input_buffer);
	} while (input_buffer != 'q');

    void schedule_save();

	return 0;
}

//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. display an error message if the character is
//not recognized.
void branching(char option) {
    struct CourseNode* course = (struct CourseNode*)malloc(sizeof(struct CourseNode));

    Subject temp_sub;
    int temp_sub_num, temp_credits;
    char temp_instructor[STR_LENGTH];

	switch (option) {
	case 'a':
        printf("What is the subject? (SER-0, EGR-1, CSE-2, EEE-3)?\n");
        scanf(" %d", &temp_sub);
        course->subject = temp_sub;

        printf("What is the subject number? (e.g. 334)?\n");
        scanf(" %d", &temp_sub_num);
        course->subject_number = temp_sub_num;

        printf("How many credits is the class? (e.g. 3)?\n");
        scanf(" %d", &temp_credits);
        course->credits = temp_credits;

        printf("What is the instructor's name?\n");
        scanf(" %s", &temp_instructor);
        strcpy(course->teacher, temp_instructor);

        course_insert(course);
		break;

	case 'd':
        printf("What is the subject? (SER-0, EGR-1, CSE-2, EEE-3)?\n");
            scanf(" %d", &temp_sub);
            course->subject = temp_sub;

            printf("What is the subject number? (e.g. 334)?\n");
            scanf(" %d", &temp_sub_num);
            course->subject_number = temp_sub_num;

            course_drop(temp_sub, temp_sub_num);
		break;

	case 's':
        void schedule_print();
		break;

	case 'q':
		// main loop will take care of this.
		break;

	default:
		printf("\nError: Invalid Input.  Please try again...");
		break;
	}
}

void course_insert(struct CourseNode* course_in){
    struct CourseNode* prev = NULL;
    struct CourseNode* iter = course_collection;
    course_collection->credits_tot += course_in->credits;

    if (course_collection == NULL || course_collection->subject_number >= course_in->subject_number){
        course_in->next = course_collection;
        course_collection = course_in;
    }else{
        iter = course_collection;
        while (iter->next!=NULL && iter->next->subject_number < course_in->subject_number){
            iter = iter->next;
        }
        course_in->next = iter->next;
        iter->next = course_in;
    }
}

void schedule_print(){

    struct CourseNode* iter = course_collection;
    printf("Class Schedule:\n");
    while(iter != NULL){
        printf("%s%d\t%d\t%s\n", iter->subject, iter->subject_number, iter->credits, iter->teacher);
        iter = iter->next;
    }
}

void course_drop(Subject subject, int subject_number){
    struct CourseNode* iter = course_collection;
    struct CourseNode* prev = NULL;

    if (iter != NULL && iter->subject_number == subject_number){
        course_collection = iter->next;
        free(iter);
        return;
    }

    while (iter != NULL && iter->subject_number != subject_number){
        prev = iter;
        iter = iter->next;
    }

    if (iter == NULL){
        printf("You are not enrolled in %d %d\n", subject, subject_number);
        return;
    }

    prev->next = iter->next;
    free(iter);
    return;
}

void schedule_load(){
    struct CourseNode* course = (struct CourseNode*)malloc(sizeof(struct CourseNode));
    int read;

    FILE *file;
    file = fopen("Schedule.txt", "r");

    if(file == NULL){
        printf("ERROR OPENING FILE Schedule.txt.\n");
        return;
    }
    do{
        read = fscanf(file,
                      "%3[^,],%d,%d,%1024[^,]\n",
                      course->subject,
                      course->subject_number,
                      course->credits,
                      course->teacher);
        if(read == 4){
            course_insert(course);
        }
        if(read != 4){
            printf("FILE FORMAT ERROR\n");
        }
        if(ferror(file)){
            printf("READ ERROR\n");
        }
    }while(!feof(file));
}

void schedule_save(){
    struct CourseNode* iter = course_collection;


    FILE *f = fopen("Schedule.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    while(iter != NULL){
        fprintf(f, "%s,%d,%d,%s\n", getSubject(iter->subject), iter->subject_number, iter->credits, iter->teacher);
        iter = iter->next;
    }

    fclose(f);
}

const char* getSubject(Subject subject){
    switch(subject){
        case SER: return "SER";
        case EGR: return "EGR";
        case CSE: return "CSE";
        case EEE: return "EEE";
    }
}