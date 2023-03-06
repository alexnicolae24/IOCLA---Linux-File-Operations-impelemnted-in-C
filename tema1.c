#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

//Functie pentru inversarea unui sir de caractere
void revstr(char *str1) {  
    int i, len, temp;  
    len = strlen(str1); 

    for (i = 0; i < len/2; i++) {   
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}

void touch (Dir* parent, char* name) {

    //Initializarea si alocarea campurilor unui obiect de tip File
	File * file;
	file = malloc(MAX_INPUT_LINE_SIZE * sizeof(File));
	file->name = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	strcpy(file->name,name);
	file->parent = parent;
	file->next = NULL;

    //Verificare daca primul element de tip File din interiorul directorului parinte exista
    if (parent->head_children_files == NULL)
    {
       parent->head_children_files = file;
    }
    else
    {   int ok = 1;//variabila ce determina daca un fisier exista deja
        File* faux = parent->head_children_files;//retinerea intr-o variabila auxiliara a primului element de tip File din interiorul directorului parinte
        
		//Verificare daca elementul de tipFile exista deja
		while(faux != NULL){
		 	if(strcmp(file->name,faux->name) == 0) {
		 		ok = 0;
		 		printf("File already exists\n");
		 	}
			faux = faux->next;
		}
		
		//Adaugarea la final in lista a unui element de tip File
        if(ok == 1) {
			File* last =  parent->head_children_files;//retinerea intr-o variabila auxiliara a primului element de tip File din interiorul directorului parinte
			while(last->next != NULL)
			{
				last = last->next;
			}
			last->next = file;
		}
    }
}   


void mkdir (Dir* parent, char* name) {
    
	//Initializarea si alocarea campurilor unui obiect de tip Dir
	Dir * dir;
	dir = malloc(MAX_INPUT_LINE_SIZE * sizeof(File));
	dir->name = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	strcpy(dir->name,name);
	dir->parent = parent;
	dir->next = NULL;
    
	//Verificare daca primul element de tip Dir din interiorul directorului parinte exista
    if (parent->head_children_dirs == NULL)
    {
       parent->head_children_dirs = dir;
    }
    else {
    	int ok = 1;//variabila ce determina daca un director exista deja
        Dir* daux = parent->head_children_dirs;//retinerea intr-o variabila auxiliara a primului element de tip Dir din interiorul directorului parinte

		//Verificare daca elementul de tip Dir exista deja
	 	while(daux != NULL){
	 		if(strcmp(dir->name,parent->head_children_dirs->name) == 0) {
				ok = 0;
				printf("Directory already exists\n");
			}
	 		daux = daux->next;
		}

       //Adaugarea la final in lista a unui element de tip File
       if(ok == 1){
			Dir* last =  parent->head_children_dirs;//retinerea intr-o variabila auxiliara a primului element de tip Dir din interiorul directorului parinte
			while(last->next != NULL)
			{
				last = last->next;
			}
			last->next = dir;
		}
    }	
}

void ls (Dir* parent) {
    
	//Afisarea tuturor obiectelor de tip Dir existente
	Dir *daux = parent->head_children_dirs;//aceeasi utilitate ca in functia mkdir
    while(daux != NULL){
	    printf("%s", daux->name);
	    daux = daux->next;
    }

	//Afisarea tuturor obiectelor de tip Dir existente	
	File *faux = parent->head_children_files;//aceeasi utilitate ca in functia touch
	while(faux != NULL){
		printf("%s", faux->name);
		faux = faux->next;
	}
}
	
void rm (Dir* parent, char* name) {

	File *faux = parent->head_children_files;//aceeasi utilitate ca in functia touch
    
	//Stergerea unui elementului corespunzator daca acesta este in primul element de tip File din interiorul directorului parinte
    if(faux != NULL && strcmp(faux->name,name) == 0){
        parent->head_children_files = faux->next; 
        free(faux); 
    }
    
	//Afisarea unui mesaj corespunzator daca fisierul nu se afla in lista
    if(faux == NULL)
      printf("Could not find the file\n");
    
	//Cautarea fisierului in lista de fisiere si stergere acestuia
	File *prevfile = malloc(MAX_INPUT_LINE_SIZE * sizeof(File));//variabila ce ajuta la stergerea elemntului
	prevfile = faux;
    while (faux != NULL  && strcmp(faux->name,name) != 0) {	
        faux = faux->next;
		prevfile->next = faux;
    }
	
	free(faux);//eliberarea memoriei
	
}  

void removeAllFiles(Dir* dir){
	File* faux = dir->head_children_files;
	File* next = malloc(MAX_INPUT_LINE_SIZE * sizeof(File));

	while(faux != NULL){
		next = faux->next;
		free(faux);
		faux = next;
	}

	dir->head_children_files = NULL;
}

void rmdir (Dir* parent, char* name) {
	Dir* daux = parent->head_children_dirs;//aceeasi utilitate ca in functia mkdir

	if(daux != NULL){
	   if(strcmp(daux->name,name) == 0) {
			parent->head_children_dirs = daux->next; 
			free(daux);
			if(daux->head_children_files != NULL){
				removeAllFiles(daux);
			}
        }
	    else{
			if(daux->next->head_children_files != NULL){
				removeAllFiles(daux->next);
			}
            daux->next = daux->next->next;	
	    }  
	}
	else{
		printf("Could not find the dir\n");   
	}
	free(daux);
}

void cd(Dir** target, char *name) {
    Dir *daux = (*target)->head_children_dirs;//variabila auxiliara ce retine primul element de tip Dir din target

    if((*target)->parent != NULL){
		//Conditia pentru cd ..
		if(strcmp(name,"..") == 0) {
			*target = (*target)->parent;	
		}
	}
    
	//Schimbarea target-ului pentru fisierul corespunzator
    while(daux != NULL){
		if(strcmp(daux->name,name) == 0) {
			*target = daux;
		}
		else{
			printf("No directories found!\n");
		}
		daux = daux->next;
    }
}

char *pwd (Dir* target) {
    char *path = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));//variabila in care se va retine path-ul final al directorului
	char *aux = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));//variabila auxiliara ce va ajuta la obtinerea path-ului
    
	//Formarea path-ului
	while(target != NULL){
		//Stergerea caracterului '\n' dupa fiecare nume de director
		if(strcmp(target->name,"home") != 0){
		target->name[strlen(target->name) - 1] = '\0';
		}
		revstr(target->name);//inversarea numelui directorului target
		strcat(aux,target->name);//concatenarea la variabila auxiliara
		strcat(aux, "/");
		target = target->parent;
	}
	
	strcpy(path,aux);
	revstr(path);//inversarea path-ului
	return path;	
}

void stop (Dir* target) {}

void tree (Dir* target, int level) {
	level = 0;

	//Afisarea tuturor obiectelor de tip File existente	
	Dir *daux = target->head_children_dirs;//variabila auxiliara ce retine primul element de tip Dir din target
    while(daux != NULL){
	    printf("%s", daux->name);
	    daux = daux->next;
    }

	//Afisarea tuturor obiectelor de tip File existente	
	File *faux = target->head_children_files;//variabila auxiliara ce retine primul element de tip Dir din target
	while(faux != NULL){
		printf("%s", faux->name);
		faux = faux->next;
	}
}

void mv(Dir* parent, char *oldname, char *newname) {
	Dir *daux = parent->head_children_dirs;//aceeasi utilitate ca in functia mkdir
    while(daux != NULL){
	    if(strcmp(daux->name,oldname) == 0){
		   if(strcmp(daux->name,newname) == 0){
			   rmdir(parent,oldname);
			   mkdir(parent,newname);  
		   }
		   else
		   {
			   printf("File/Director already exists\n");
		   }
		}
		else{
			printf("File/Director not found\n");
		}
	    daux = daux->next;
    }

	// //Afisarea tuturor obiectelor de tip Dir existente	
	// File *faux = parent->head_children_files;//aceeasi utilitate ca in functia touch
	// while(faux != NULL){
	// 	if(strcmp(faux->name,oldname) == 0){
	// 	   if(strcmp(faux->name,newname) == 0){
	// 		   rm(parent,oldname);
	// 		   touch(parent,newname);  
	// 	    }
	// 	   else{
	// 		   printf("File/Director already exists\n");
	// 	    }
	// 	}
	// 	else{
	// 		printf("File/Director not found\n");
	// 	}
	// 	faux = faux->next;
	// }
}

int main () {
	Dir *parent;
	parent = malloc(MAX_INPUT_LINE_SIZE * sizeof(Dir));
    parent->name = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
    strcpy(parent->name,"home");
    parent->parent = NULL;
    parent->head_children_dirs = NULL;
	parent->head_children_files = NULL;
	parent->next = NULL;

	char *input = NULL;
	input = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));

	int level = 0;
	
	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
		if(strlen(input) > 0) {
			char *p = strtok(input," ");
			char *p1 = strtok(NULL," "); 
			char *p2 = strtok(NULL," ");

			if(strcmp(p,"mv") == 0){
				mv(parent,p1,p2);
			}

			if(strcmp(p,"touch") == 0){ 
				touch(parent,p1);	 
			}

			if(strcmp(p,"mkdir") == 0){
			  	mkdir(parent,p1);
			}

			if(strcmp(p,"rm") == 0){
				rm(parent,p1);
			}

			if(strcmp(p,"rmdir") == 0){
				rmdir(parent,p1);
			}

			if(strcmp(p,"cd") == 0){
                cd(&parent,p1);
			}

			p[strlen(p) - 1] = '\0';

			if(strcmp(p,"ls") == 0){
				ls(parent);
			}
            
			if(strcmp(p,"pwd") == 0){
				printf("%s",pwd(parent));
			}

			if(strcmp(p,"tree") == 0){
				tree(parent,level);
			}

			if(strcmp(p,"stop") == 0){
				break;
			}

		}
	} while (fgets(input,MAX_INPUT_LINE_SIZE,stdin));
	return 0;
}
