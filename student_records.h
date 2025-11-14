#ifndef _STUDENT_
#define _STUDENT_

typedef struct {	//structure declaration not in main() just to help me to use the structure members for every function
	char name[100];
	long int student_id;
	int marks[3];
	float average;
	char grade;
	float total;
	int sn;
} student;

int count_records();
void add_student( student);
void display();
void deleteStudent();
void editStudent();
void search(student *); //* to enable modifying of struct members
void sort(student );
void menu( student );
void loop( student );

#endif
