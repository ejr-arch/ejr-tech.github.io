#include "student_records.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define BUF_SIZE 1024

FILE *infile;
int count_records(){
	infile = fopen("student_records.txt", "r"); 
	if(infile==NULL){
		printf("failed to open students records file!\n");
		return 0; 		// if file doesn't exist yet
	}
	int count = 0;
	char line[BUF_SIZE];
	while (fgets(line, sizeof(line), infile)) count++;
	fclose(infile);
	return count;
}

void add_student( student s){
	s.sn = count_records()+1;	//automatic numbering of record-- +1 such that it assigns the next number to the record
	int i=0;
	s.total=0;
	
	infile=fopen("student_records.txt","a+");	//a+ for immediate reading after writing
	if(infile==NULL){
		printf("failed to open fail.....!");
		exit(-1);
	}
	printf("Enter name of student: ");
	fgets(s.name, sizeof(s.name),stdin);	//get student name by keyboard
	s.name[strcspn(s.name,"\n")]=0;
	printf("Student_Id: ");
	scanf(" %ld",&s.student_id);
	while(getchar() != '\n');		//clear newline character left in buffer
	printf("Enter marks in the 3 subjects\n");

	do{
		printf("subject %d:",i+1);
		scanf(" %d",&s.marks[i]);
		while(getchar() != '\n');
		s.total+=s.marks[i];
		i++;

	}while(i<3);
	s.average=s.total/3;
	if(s.average<40) s.grade='O';		//grade is just a character so no need for strcpy()
	else if(s.average<60) s.grade='C';
	else if(s.average<90) s.grade='B';
	else s.grade='A';

	fprintf(infile,"%d,%s,%ld,%d,%d,%d,%.2f,%c\n", s.sn,s.name,s.student_id,s.marks[0],s.marks[1],s.marks[2],s.average,s.grade);	//%ld-for long integer of student id
	fclose(infile);
	printf("Student record #%d created succesfully..!\n",s.sn);

}

void table_display(char line[],FILE *infile){
	//printf("no code yet");
	printf("+-----+------------------------+--------------------+----------------+-----------------+--------------+\n");
	printf("|S/n  |Student_Name            |Student_id          |Student's_marks |Student_average  |Student_grade |\n");
	printf("+-----+------------------------+--------------------+----------------+-----------------+--------------+\n");

	while(fgets(line,256,infile)){		//to store recordstring as program fetches it from file and splits it up to tokens separated by commas.
		student s;
		char *token = strtok(line, ",");
		if (!token) continue; s.sn = atoi(token);
		token = strtok(NULL, ",");if (!token) continue; strcpy(s.name, token);	//if(!token)continue;--to test for null tokens before using them, to prevent segmentation fault
		token = strtok(NULL, ",");if (!token) continue; s.student_id=atol(token);
		token = strtok(NULL, ",");if (!token) continue; s.marks[0]=atoi(token);
		token = strtok(NULL, ",");if (!token) continue; s.marks[1]=atoi(token);
		token = strtok(NULL, ",");if (!token) continue; s.marks[2]=atoi(token);
		token = strtok(NULL, ",");if (!token) continue; s.average=atof(token);
		token = strtok(NULL, ",");if (!token) continue; s.grade=token[0];

		printf("|%-5d|%-24s|%-20ld|%-4d, %-4d, %-4d|%17.2f|%-14c|\n",
		s.sn,s.name,s.student_id,s.marks[0],s.marks[1],s.marks[2],s.average,s.grade);//in %-5d ~ -5 for alignment and indentation
		printf("+-----+------------------------+--------------------+----------------+-----------------+--------------+\n");
	}

}

void display(){
	infile=fopen("student_records.txt","r");
	if(infile == NULL){
        	printf("No records found.\n");
	        return;
	}
	char line[200];		
	table_display(line,infile);
	fclose(infile);
}

FILE *result;

void search_by_name(student *s1){
	int option, found=0;
	char query[50];
	char linefile[200];
	char results[200];
	printf("Enter student name to search for: ");
	fgets(query,sizeof(query),stdin);
	query[strcspn(query,"\n")]=0;		//such that name entered as query is stored without newline character
	for(int i=0;query[i];i++){
		query[i] = tolower((unsigned char)query[i]);//change entered name to lowercase such that the search is case insensitive
	}
	while(fgets(linefile,sizeof(linefile),infile)){
		linefile[strcspn(linefile,"\n")]=0;			//2
		for(int j=0;linefile[j];j++){
			linefile[j] = tolower((unsigned char)linefile[j]);
		}
		if(strstr(linefile, query)!=NULL){
			found++;
			fputs(linefile,result);
			fputc('\n',result);//append newline character(formally removed be4 search in 2) in result string to separate CSV strings from the file
		}
	}
	fclose(infile);
	fclose(result);
	if(found>0){
		printf("we found %d results as shown in table below!\n",found);
		result = fopen("search_results.txt","r");
		if(result == NULL){
			printf("No records found.\n");
			return;
		}
		table_display(linefile,result);	
	}
	else printf("no matches found!\n");
	fclose(result);
}

void search_by_id(student *s1){
	int option, found=0;
	char query[50];
	char linefile[200];
	printf("Enter the ID number: ");
	char id[15];
	fgets(id,15,stdin);	
	id[strcspn(id,"\n")]=0;
	
	result = fopen("search_results.txt","w");
	if(result==NULL){
		printf("couldn't open results!\n");
		return;
	}
	infile = fopen("student_records.txt","r");
	if(infile==NULL){
		printf("no search records available!\n");
		return;
	}
	while(fgets(linefile,sizeof(linefile),infile)){
		if(strstr(linefile,id)!=NULL){
			found++;
			fputs(linefile,result);
			fputc('\n',result);
		}
	}
	fclose(result);
	result = fopen("search_results.txt","r");
	if(found>0){
		table_display(linefile,result);
	}else printf("no matches found..!\n");
	fclose(result);
	fclose(infile);
}

void search( student *s1){
	int option, found=0;
	char query[50];
	char linefile[200];
	char results[200];
	infile = fopen("student_records.txt","r");
	if(infile == NULL){
		printf("No records found.\n");
	        return;
	}
	result = fopen("search_results.txt","w");
	if(result == NULL){
        	printf("search history couldn't open!.\n");
        	return;
	}
	printf("Search by\n1- Name\n2- Student ID\n");
	printf("Enter 1 or 2: \n");
	scanf(" %d",&option);
	while(getchar()!='\n');
	switch(option){
		case 1:
			search_by_name(s1);
		break;
		case 2:
			search_by_id(s1);
		break;
		default:
		printf("invalid option entered!\nsearch is by name or id--only!\n");
	}
}

void bubble_sort(char *names[],char *records[], int i){
	char *temp;
	for(int a=0;a<i;a++){
		for(int b=a+1;b<i;b++){
			if(strcasecmp(names[a],names[b])>0){	//difference in the ascii values is actually the return value
				temp = names[a];
				names[a] = names[b];		//uses bubble sort algorithm to arrange the names
				names[b] = temp;
				
				temp = records[a];
				records[a] = records[b];		//uses bubble sort algorithm to arrange the names
				records[b] = temp;
			}
		}
	}

}

void sort_by_name(char* names[], char linefile[], int field_count, int i, FILE *sorted, char line[]){
	printf("");
	for(int j=0;j<50;j++){
		names[j] = (char *)malloc(256);
	}
	char *records[50];//this string of strings to store up to 50 records from file
	for(int j=0;j<50;j++){
		records[j] = (char *)malloc(256);
	}
	infile = fopen("student_records.txt","r");
	if(infile==NULL){
		printf("Error opening file!\n");
		return;
	}	
	while(fgets(linefile,256,infile)){
		strcpy(records[i],linefile);
		field_count=0;
		linefile[strcspn(linefile,"\n")]=0;	//strcspn returns integer position of \n in the string linefile, which gives linefile[10]=0; so \n is eliminated
		char *token = strtok(linefile, ",");	
		if(token==NULL)continue;
		while(token!=NULL){
			if(field_count==1){		//since we know from our setup, names are in field 2
				strcpy(names[i], token);
				break;
			}
			token = strtok(NULL,",");
			field_count++;
		}
		i++;
	}
	fclose(infile);
	bubble_sort(names,records,i);
	sorted = fopen("sorted_records.txt","w");
	for(int j=0;j<i;j++){
		fputs(records[j],sorted);
		fputc('\n',sorted);
	}
	fclose(sorted);
	sorted = fopen("sorted_records.txt","r");
	table_display(line,sorted);
	fclose(sorted);
}
void sort_by_id(char* names[], char linefile[], int field_count, int i, FILE *sorted, char line[]){
	printf("\n");
	char *ids[50];//this string of strings to store up to 50 ids from file
	for(int j=0;j<50;j++){
		ids[j] = (char *)malloc(256);
	}
	char *record[50];//this string of strings to store up to 50 records from file
	for(int j=0;j<50;j++){
		record[j] = (char *)malloc(256);
	}
	infile = fopen("student_records.txt","r");
	if(infile==NULL){
		printf("Error opening file!\n");
		return;
	}	
	while(fgets(linefile,256,infile)){
		strcpy(record[i],linefile);
		field_count=0;
		linefile[strcspn(linefile,"\n")]=0;	//strcspn returns integer position of \n in the string linefile, which gives linefile[10]=0; so \n is eliminated
		char *token = strtok(linefile, ",");	
		if(token==NULL)continue;
		while(token!=NULL){
			if(field_count==2){		//since we know from our setup, ids are in field 3
				strcpy(ids[i], token);
				break;
			}
			token = strtok(NULL,",");
			field_count++;
		}
		i++;
	}
	fclose(infile);
	char *temp;
	for(int a=0;a<i;a++){
		for(int b=a+1;b<i;b++){
			if(strcasecmp(ids[a],ids[b])>0){	//difference in the ascii values is actually the return value
				temp = ids[a];
				ids[a] = ids[b];		//uses bubble sort algorithm to arrange the names
				ids[b] = temp;
				
				temp = record[a];
				record[a] = record[b];		//uses bubble sort algorithm to arrange the names
				record[b] = temp;
			}
		}
	}
	sorted = fopen("sorted_records.txt","w");
	for(int j=0;j<i;j++){
		fputs(record[j],sorted);
		fputc('\n',sorted);
	}
	fclose(sorted);
	sorted = fopen("sorted_records.txt","r");
	table_display(line,sorted);
}

void sort( student s1){
	int option;
	printf("Sort by?\n1- Name\n2- Student ID\n");
	printf("Enter option: ");
	scanf(" %d",&option);
	char *names[50];//this string of strings to store up to 50 names from file
	char linefile[256];
	int field_count,i=0;
	int a,b;
	FILE *sorted;
	char line[200];
	//printf("no sort snippet coded yet!\n");
	switch(option){
		case 1:
			sort_by_name(names, linefile, field_count, i, sorted, line);
		break;
		case 2:
			sort_by_id(names, linefile, field_count, i, sorted,line);
		break;
		default:
			printf("invalid option enetered!\n");
			return;
	}
	return;
}

void edit(student *s1){
	printf("Enter name of student whose record you want to change: ");
	char query[BUF_SIZE];
	fgets(query,sizeof(query),stdin);
	query[strcspn(query,"\n")]=0;
	search_by_name(s1);
	return;
}

void menu( student s1){
//	printf("no menu snippet coded yet!\n");
	int choice;
	char c;
	do{
	printf("Enter an option to perform a corresponding function.\n");
	printf("1- Add new student information\n");
	printf("2- Display student information\n");
	printf("3- Search for student information by name or id\n");
	printf("4- Sort student information\n");
	printf("Enter an option to execute: ");
	scanf(" %d",&choice);
	while(getchar()!='\n');
	switch(choice){
		case 1:
			add_student(s1);
		break;
		case 2:
			display();
		break;
		case 3:
			search(&s1);
		break;
		case 4:
			sort(s1);
		break;
		default:
			printf("invalid option entered, please enter again!\n");
		}
	printf("\ndo you want to perform another function?(y/n)\n");
	scanf(" %c",&c);
	while(getchar()!='\n');
	}while(c=='y'||c=='Y');
return;
}

