#include "student_records.h"
#include<stdio.h>
#include<string.h>


int main(int argc, char *argv[]){	//argc-argument count ~argv-argument vector
	student s1; 
	if(argc<2){		//if no surplus arguments are provided like ./student_records
		menu(s1);
		return 0;
	}
	else if(strcmp(argv[1],"add")==0){	// ./student_records add
		add_student(s1);
		return 0;
	}
	else if(strcmp(argv[1],"display")==0){		// ./student_records display
		display();
		return 0;
	}
	else if(strcmp(argv[1],"search")==0){		// ./student_records search
		search(&s1);
		return 0;
	}
	else if(strcmp(argv[1],"sort")==0){		// ./student_records sort
		sort(s1);
		return 0;
	}
	else if(strcmp(argv[1],"delete")==0){		// ./student_records delete
		deleteStudent();
		return 0;
	}
	else if(strcmp(argv[1],"edit")==0){		// ./student_records edit
		editStudent();
		return 0;
	}
	
	else {
		printf("unknown command \"%s\" entered\n",argv[1]);	// any other commands are not catered for
		return 0;
	}		
	return 0;
}


