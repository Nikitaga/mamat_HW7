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


struct student{
	int id;
	char *name;
	struct list *courses;

};

struct grades{
	struct list *students;
};

struct course{
	char *course;
	int grade;
};



/* User function, clone “element” to “output”. Returns 0 on success */
int clone_student(void *element, void **output);

/* User function, Destroy “element”. always succeed */
void destroy_student(void *element);

int clone_courses(void *element, void **output);

void destroy_courses(void *element);

//int grades_print_student(struct grades *grades, int id);


/**
 * @brief Initializes the "grades" data-structure.
 * @returns A pointer to the data-structure, of NULL in case of an error
 */
struct grades* grades_init() {
	grades_t *grades_list = (grades_t*)malloc(sizeof(grades_t));
	grades_list->students = list_init(clone_student, destroy_student);
	return grades_list;
};

/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades) {

	list_destroy(grades->students);
	free(grades);

};

/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Failes if "grades" is invalid, or a student with
 * the same "id" already exists in "grades"
 */
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

	new_student->id = id;
	char *name_location = (char*)malloc(sizeof(char)*(strlen(name)+1));
	strcpy(name_location,name);

	new_student->name = name_location;
	new_student->courses = list_init(clone_courses, destroy_courses);

	return list_push_front(grades->students, (void*)new_student);

};

/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
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
				char *course_name = (char*)malloc(sizeof(char)*(strlen(name)+1));
				strcpy(course_name,name);

				new_course->grade = grade;
				new_course->course = course_name;

				return list_push_front(student->courses, (void*)new_course);
			};

		    student_list = list_next(student_list);
		};

    return ERROR;
};

/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
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

/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order
 * in which they were inserted into "grades"
 */
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
/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order
 * in which they were inserted into "grades"
 */
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

int clone_student(void *element, void **output) {

	*output = element;

	if (!*output) {
		free(*output);
		return ERROR;
	};

	return 0;
};

void destroy_student(void *element) {

	student_t *student = (student_t*)element;

	list_destroy(student->courses);
	free(student->name);
	free(student);
};

int clone_courses(void *element, void **output) {

	*output = element;

	if (!*output) {
		free(*output);
		return ERROR;
	};

	return 0;

};

void destroy_courses(void *element) {

	course_t *course =  (course_t*)element;
	free(course->course);
	free(course);

};

