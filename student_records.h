#ifndef _STUDENT_
#define _STUDENT_

struct student {	//structure declaration not in main() just to help me to use the structure members for every function
	char name[100];
	long int student_id;
	int marks[3];
	float average;
	char grade;
	float total;
	int sn;
};

int count_records();
void add_student(struct student);

void display();
void edit(void);
void search(struct student *); //*s1 to enable modifying of struct members
void sort(struct student );
void menu(struct student );
void loop(struct student );



#endif
