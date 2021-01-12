#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked-list.h"
#include "grades.h"

#define ERROR -1

typedef struct course course_t;
typedef struct student student_t;
typedef struct grades grades_t;

/**
 * @brief struct of single student element
 * @param id student's id
 * @param name student's name
 * @param courses pointer to a linked list of the student's courses+grades
 */
struct student{
	int id;
	char *name;
	struct list *courses;
};

/**
 * @brief a linked list of all the students
 * @param students pointer to the student's linked list
 */
struct grades{
	struct list *students;
};

/**
 * @brief struct of single course element
 * @param course's name
 * @param courses grade
 * @note part of a linked list attributed to a single student
 */
struct course{
	char *course;
	int grade;
};

/* User functions, clone “element” to “output”. Returns 0 on success */
int clone_student(void *element, void **output);
int clone_courses(void *element, void **output);

/* User functions, Destroy “element”. always succeed */
void destroy_student(void *element);
void destroy_courses(void *element);


/* Explanation in grades.h file */
struct grades* grades_init() {

    grades_t *grades_list = (grades_t*)malloc(sizeof(grades_t));
    grades_list->students = list_init(clone_student, destroy_student);
    return grades_list;
};

/* Explanation in grades.h file */
void grades_destroy(struct grades *grades) {

    list_destroy(grades->students);
    free(grades);
};

/* Explanation in grades.h file */
int grades_add_student(struct grades *grades, const char *name, int id) {

    if (!grades){
        return ERROR;
    };

    struct node *student_list = list_begin(grades->students);
    while(student_list) {
        student_t *student = (student_t*)list_get(student_list);
        if (id == student->id){
            return ERROR;
        }
    student_list = list_next(student_list);
    };

    student_t *new_student = malloc(sizeof(student_t));
    char *name_location = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(name_location,name);

    new_student->id = id;
    new_student->name = name_location;
    new_student->courses = list_init(clone_courses, destroy_courses);

    return list_push_front(grades->students, (void*)new_student);
};

/* Explanation in grades.h file */
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade) {

    if (grade < 0 || grade > 100 || !grades) {
        return ERROR;
    };

    struct node *student_list = list_begin(grades->students);
        while(student_list) {
            student_t *student = (student_t*)list_get(student_list);
            if (student->id == id) {
                struct node *courses_list = list_end(student->courses);

                while(courses_list){
                    course_t *course = (course_t*)list_get(courses_list);
                    if (!strcmp(name, course->course)) {
                        return ERROR;
                    };
                    courses_list = list_prev(courses_list);
                };

                course_t *new_course = (course_t*)malloc(sizeof(course_t));
                char *course_name=(char*)malloc(sizeof(char)*(strlen(name)+1));
                strcpy(course_name,name);

                new_course->grade = grade;
                new_course->course = course_name;

                return list_push_front(student->courses, (void*)new_course);
            };
            student_list = list_next(student_list);
        };
    return ERROR;
};

/* Explanation in grades.h file */
float grades_calc_avg(struct grades *grades, int id, char **out) {

    if (!grades){
        *out = NULL;
        return ERROR;
    };

    struct node *student_list = list_begin(grades->students);

    while(student_list) {
        student_t *student = (student_t*)list_get(student_list);
        if (student->id == id) {
            char *student_name = malloc(sizeof(char)*(strlen(student->name)+1));
            strcpy(student_name,student->name);
            *out = student_name;

            struct node *courses_list = list_end(student->courses);
            float grade_sum = 0;
            int grade_count = 0;
            while(courses_list){
                course_t *course = (course_t*)list_get(courses_list);
                grade_sum = grade_sum + (float)course->grade;
                grade_count = grade_count + 1;
                courses_list = list_prev(courses_list);
            };

            if (grade_count){
                return grade_sum/((float)grade_count);
            } else {
                return 0;
            };
        };
        student_list = list_next(student_list);
    };
    *out = NULL;
    return ERROR;
};

/* Explanation in grades.h file */
int grades_print_student(struct grades *grades, int id) {

    if (!grades){
        return ERROR;
    };

    struct node *student_list = list_begin(grades->students);

    while(student_list) {
        student_t *student = (student_t*)list_get(student_list);
        if (student->id == id) {
            printf("%s %d:", student->name, id);
            struct node *courses_list = list_end(student->courses);
            while(courses_list){
                struct course *courses = (struct course*)list_get(courses_list);
                if (list_prev(courses_list)){
                    printf(" %s %d,", courses->course, courses->grade);
                } else {
                    printf(" %s %d", courses->course, courses->grade);
                };
                courses_list = list_prev(courses_list);
            };
            printf("\n");
            return 0;
        };
        student_list = list_next(student_list);
    };
    return ERROR;
};

/* Explanation in grades.h file */
int grades_print_all(struct grades *grades) {

    if (!grades){
        return ERROR;
    };

    struct node *student_list = list_end(grades->students);
    while(student_list) {
        student_t *student = (student_t*)list_get(student_list);
        int id = student->id;
        grades_print_student(grades, id);
        student_list = list_prev(student_list);
    };

    return 0;
};

/* clone student node */
int clone_student(void *element, void **output) {

    *output = element;

    if (!*output) {
        free(*output);
        return ERROR;
    };
    return 0;
};

/* destroy student node */
void destroy_student(void *element) {

    student_t *student = (student_t*)element;

    list_destroy(student->courses);
    free(student->name);
    free(student);
};

/* clone course node */
int clone_courses(void *element, void **output) {

    *output = element;

    if (!*output) {
        free(*output);
        return ERROR;
	};
    return 0;
};

/* destroy course node */
void destroy_courses(void *element) {

    course_t *course =  (course_t*)element;
    free(course->course);
    free(course);
};
