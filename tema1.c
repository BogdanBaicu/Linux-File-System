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

void touch (Dir* parent, char* name) {
	// Daca parintele contine directoare, verific ca numele sa nu mai fie folosit
	if(parent->head_children_dirs!=NULL)
	{
		Dir *aux=parent->head_children_dirs;
		if(strcmp(aux->name,name)==0)
		{
			goto afisare;
			return;
		}
		while(aux->next!=NULL)
		{
			aux=aux->next;
			if(strcmp(aux->name,name)==0)
			{
				goto afisare;
				return;
			}
		}
	}

	// Daca parintele nu contine un director cu acel nume
	// Verfic daca directorul parinte nu contine fisiere
	if(parent->head_children_files==NULL)
	{
		// Aloc memorie si creez fisierul
		File *new=malloc(sizeof(File));
		new->name=malloc((strlen(name)+1)*sizeof(char));
		strcpy(new->name,name);
		new->next=NULL;
		new->parent=parent;
		parent->head_children_files=new;
		return;
	}
	else
	{
		// Daca parintele mai contine fisiere, verific ca numele sa nu mai
		// fie folosit
		File *faux=parent->head_children_files;
		if(strcmp(faux->name,name)==0)
		{
			goto afisare;
			return;
		}
		while(faux->next!=NULL)
		{
			faux=faux->next;
			if(strcmp(faux->name,name)==0)
			{
				goto afisare;
				return;
			}
		}

		// Daca numele nu a mai fost folosit
		// Ma deplasez la ultimul fisier (cel ce are legatura catre null)
		faux=parent->head_children_files;
		while(faux->next!=NULL)
			faux=faux->next;
		// Aloc memorie si creez fisierul
		File *new=malloc(sizeof(File));
		new->name=malloc((strlen(name)+1)*sizeof(char));
		strcpy(new->name,name);
		new->next=NULL;
		new->parent=parent;
		faux->next=new;
		return;
	}

afisare:
	printf("File already exists\n");
}

void mkdir (Dir* parent, char* name) {
	// Daca parintele contine fisiere, verific ca numele sa nu mai fie folosit
	if(parent->head_children_files!=NULL)
	{
		File *faux=parent->head_children_files;
		if(strcmp(faux->name,name)==0)
		{
			goto afisare;
			return;
		}
		while(faux->next!=NULL)
		{
			faux=faux->next;
			if(strcmp(faux->name,name)==0)
			{
				goto afisare;
				return;
			}
		}
	}

	// Daca parintele nu contine un fisier cu acel nume
	// Verific daca directorul parinte nu contine directorare
	if(parent->head_children_dirs==NULL)
	{
		// Aloc memorie si creez directorul
		Dir *new=malloc(sizeof(Dir));
		new->name=malloc((strlen(name)+1)*sizeof(char));
		strcpy(new->name,name);
		new->next=NULL;
		new->head_children_dirs=NULL;
		new->head_children_files=NULL;
		new->parent=parent;
		parent->head_children_dirs=new;
		return;
	}
	else
	{
		// Daca parintele contine directoare, verific ca numele sa nu mai 
		// fie folosit
		Dir *aux=parent->head_children_dirs;
		if(strcmp(aux->name,name)==0)
		{
			goto afisare;
			return;
		}
		while(aux->next!=NULL)
		{
			aux=aux->next;
			if(strcmp(aux->name,name)==0)
			{
				goto afisare;
				return;
			}
		}
		
		// Daca numele nu a mai fost folosit
		// Ma deplasez la ultimul director (cel ce are legatura catre null)
		aux=parent->head_children_dirs;
		while(aux->next!=NULL)
			aux=aux->next;
		// Aloc memorie si creez directorul
		Dir *new=malloc(sizeof(Dir));
		new->name=malloc((strlen(name)+1)*sizeof(char));
		strcpy(new->name,name);
		new->next=NULL;
		new->head_children_dirs=NULL;
		new->head_children_files=NULL;
		new->parent=parent;
		aux->next=new;
		return;
	}
	
afisare:
	printf("Directory already exists\n");
}

void ls (Dir* parent) {
	
	// Pentru directoare	
	if(parent->head_children_dirs!=NULL)
	{
		Dir *aux=parent->head_children_dirs;
		printf("%s\n",aux->name);
		while(aux->next!=NULL)
		{
			aux=aux->next;
			printf("%s\n",aux->name);
		}
	}

	// Pentru fisiere
	if(parent->head_children_files!=NULL)
	{
		File *faux=parent->head_children_files;
		printf("%s\n",faux->name);
		while(faux->next!=NULL)
		{
			faux=faux->next;
			printf("%s\n",faux->name);
		}
	}
}

void rm (Dir* parent, char* name) {
	// Daca directorul parinte nu contine fisiere
	if(parent->head_children_files==NULL)
	{
		goto afisare;
		return;
	}

	// Daca directorul parinte contine un singur fisier
	if(parent->head_children_files!=NULL&&parent->head_children_files->next==NULL)
	{
		// Daca este fisierul ce trebuie sters
		if(strcmp(parent->head_children_files->name,name)==0)
		{
			free(parent->head_children_files->name);
			free(parent->head_children_files);
			parent->head_children_files=NULL;
			return;
		}
		else
		{
			goto afisare;
			return;
		}
	}

	// Daca directorul parinte contine mai multe fisiere
	// Daca fisierul cautat este parent->head_children_files
	if(strcmp(parent->head_children_files->name,name)==0)
	{
		File *aux;
		aux=parent->head_children_files->next;
		free(parent->head_children_files->name);
		free(parent->head_children_files);
		parent->head_children_files=aux;
		return;
	}
	// Daca fisierul cautat nu este nici primul, nici ultimul
	File *aux;
	aux=parent->head_children_files;
	while(aux->next->next!=NULL)
	{
		if(strcmp(aux->next->name,name)==0)
		{
			File *aux2;
			aux2=aux->next;
			aux->next=aux->next->next;
			free(aux2->name);
			free(aux2);
			return;
		}
		aux=aux->next;
	}
	// Daca fisierul cautat este ultimul (are next setat pe NULL)
	if(strcmp(aux->next->name,name)==0)
	{
		free(aux->next->name);
		free(aux->next);
		aux->next=NULL;
		return;
	}

	// Daca fisierul nu a fost gasit
afisare:
	printf("Could not find the file\n");
}

void rmdir (Dir* parent, char* name) {
	// Daca directorul parinte nu contine niciun director
	if(parent->head_children_dirs==NULL)
	{
		goto afisare;
		return;
	}

	// Daca directorul parinte contine un singur fisier
	if(parent->head_children_dirs!=NULL&&parent->head_children_dirs->next==NULL)
	{
		// Daca este directorul ce trebuie sters
		if(strcmp(parent->head_children_dirs->name,name)==0)
		{
			// Daca directorul contine fisiere, le stergem
			while(parent->head_children_dirs->head_children_files!=NULL)
				rm(parent->head_children_dirs,parent->head_children_dirs->head_children_files->name);

			// Daca directorul contine directoare, le stergem
			while(parent->head_children_dirs->head_children_dirs!=NULL)
				rmdir(parent->head_children_dirs,parent->head_children_dirs->head_children_dirs->name);

			// Golesc memoria si sterg legatua directorului parinte
			free(parent->head_children_dirs->name);
			free(parent->head_children_dirs);
			parent->head_children_dirs=NULL;
			return;
		}
		else
		{
			goto afisare;
			return;
		}
	}

	// Daca directorul parinte contine mai multe directoare
	// Daca directorul ce trebuie sters este primul director
	if(strcmp(parent->head_children_dirs->name,name)==0)
	{
		// Daca directorul contine fisiere, le stergem
		while(parent->head_children_dirs->head_children_files!=NULL)
			rm(parent->head_children_dirs,parent->head_children_dirs->head_children_files->name);

		// Daca directorul contine directoare, le stergem
		while(parent->head_children_dirs->head_children_dirs!=NULL)
			rmdir(parent->head_children_dirs,parent->head_children_dirs->head_children_dirs->name);
		
		// Golesc memoria si sterg directorul parinte
		Dir *aux;
		aux=parent->head_children_dirs->next;
		free(parent->head_children_dirs->name);
		free(parent->head_children_dirs);
		parent->head_children_dirs=aux;
		return;
		
	}

	// Daca directorul ce trebuie sters nu este nici primul, nici ultimul
	Dir *aux;
	aux=parent->head_children_dirs;
	while(aux->next->next!=NULL)
	{
		if(strcmp(aux->next->name,name)==0)
		{
			Dir *aux2;
			aux2=aux->next;
			aux->next=aux->next->next;

			// Daca directorul contine fisiere, le stergem
			while(aux2->head_children_files!=NULL)
				rm(aux2,aux2->head_children_files->name);

			// Daca directorul contine directoare, le stergem
			while(aux2->head_children_dirs!=NULL)
				rmdir(aux2,aux2->head_children_dirs->name);
			free(aux2->name);
			free(aux2);
			return;
		}
		aux=aux->next;
	}

	// Daca directorul ce trebuie sters este ultimul
	if(strcmp(aux->next->name,name)==0)
	{
		// Daca direectoul contine fisiere, le stergem
		while(aux->next->head_children_files!=NULL)
			rm(aux->next,aux->next->head_children_files->name);

		// Daca directorul contine directoare, le stergem
		while(aux->next->head_children_dirs!=NULL)
			rmdir(aux->next,aux->next->head_children_dirs->name);

		free(aux->next->name);
		free(aux->next);
		aux->next=NULL;
		return;	
	}

	// Daca directorul nu a fost gasit
afisare:
	printf("Could not find the dir\n");
}

void cd(Dir** target, char *name) {
	// Primirea comenzii cd ..
	if(strcmp(name,"..")==0)
	{
		// Verific daca directorul parinte este diferit de null
		if((*target)->parent!=NULL)
		{
			*target=(*target)->parent;
			return;
		} 
		return;
	}

	// Primirea comenzii cd cu alt parametru
	// Daca directorul target nu contine alte directoare
	if((*target)->head_children_dirs==NULL)
	{
		goto afisare;
		return;
	}
	Dir *aux;
	aux=(*target)->head_children_dirs;
	if(strcmp(aux->name,name)==0)
	{
		*target=aux;
		return;
	}
	while(aux->next!=0)
	{
		aux=aux->next;
		if(strcmp(aux->name,name)==0)
		{	
			*target=aux;
			return;
		}
	}


	// Daca nu a fost gasit directorul
afisare:
	printf("No directories found!\n");
}

char *pwd (Dir* target) {
	char *cale;
	// Daca target nu are parinte
	if(target->parent==NULL)
	{
		// Initializez stringul ce va fi returnat
		// Aloc 6*sizeof(char) pentru ca initial va stoca /home, atunci
		// cand in el va fi salvat numele primului director
		cale=(char *)malloc(6*sizeof(char));
		strcpy(cale,"");
		
	}
	else
	{
		// Reapelez functia pentru parinte si realoc memoria
		cale=pwd(target->parent);
		cale=(char*)realloc(cale,strlen(cale)+strlen(target->name)+2);
	}
	// Concatenez numele directorului
	strcat(cale,"/");
	strcat(cale,target->name);
	return cale;
}

void stop (Dir* target) {
	// Sterg toate fisierele pe care le contine target
	while(target->head_children_files!=NULL)
		rm(target,target->head_children_files->name);

	// Sterg toate directoarele pe care le contine target
	while(target->head_children_dirs!=NULL)
		rmdir(target,target->head_children_dirs->name);
}

void tree (Dir* target, int level) {
	// Daca target contine directoare
	if(target->head_children_dirs!=NULL)
	{
		Dir *aux;
		aux=target->head_children_dirs;
		int i;
		// Afisez spatiile corespunzatoare
		for(i=0;i<level;i++)
			printf("    ");
		// Afisez numele directorului 
		printf("%s\n",aux->name);
		// Avansez in interiorul sau
		tree(aux,level+1);

		// Cat timp are vecini
		while(aux->next!=NULL)
		{
			// Ma deplasez pe vecinul urmator
			aux=aux->next;
			// Repet preocedura de afisare
			for(i=0;i<level;i++)
				printf("    ");
			printf("%s\n",aux->name);
			tree(aux,level+1);
		}
	}

	// Daca target contine fisiere
	if(target->head_children_files!=NULL)
	{
		File *faux;
		faux=target->head_children_files;
		int i;
		// Afisez spatiile corespunzatoare
		for(i=0;i<level;i++)
			printf("    ");
		// Afisez numele fisierului
		printf("%s\n",faux->name);

		// Cat timp are vecini
		while(faux->next!=NULL)
		{
			// Ma deplasez pe vecinul urmator
			faux=faux->next;
			// Repet procedura de afisare
			for(i=0;i<level;i++)
				printf("    ");
			printf("%s\n",faux->name);
		}
	}
}

void mv(Dir* parent, char *oldname, char *newname) {
	// Daca directorul parent este gol
	if(parent->head_children_dirs==NULL&&parent->head_children_files==NULL)
		goto afisare1;
	// Verific daca exista un director sau un fisier cu numele primit ca oldname
	int exd=0,exf=0;
	// Daca directorul parinte contine directoare, verific daca exista unul
	// cu numele primit ca parametru oldname
	Dir *aux;
	File *faux;
	if(parent->head_children_dirs!=NULL)
	{
		aux=parent->head_children_dirs;
		if(strcmp(aux->name,oldname)==0)
			exd=1;
		if(exd==0)
			while(aux->next!=NULL)
			{
				aux=aux->next;
				if(strcmp(aux->name,oldname)==0)
				{
					exd=1;
					break;
				}
			}
	} 
	// Daca nu a fost gasit un director cu numele primit ca oldname, verific
	// daca exista un fisier cu acel nume
	if(exd==0)
		if(parent->head_children_files!=NULL)
		{
			faux=parent->head_children_files;
			if(strcmp(faux->name,oldname)==0)
				exf=1;
			if(exf==0)
				while(faux->next!=NULL)
				{
					faux=faux->next;
					if(strcmp(faux->name,oldname)==0)
					{
						exf=1;
						break;
					}
				}
		}

	// Daca nu au fost gasite un director sau un fisier cu numele primit,
	// afisez mesajul corespunzator
	if(exf==0&&exd==0)
		goto afisare1;

	// Daca exista un fisier/director cu oldname, verific sa nu existe
	// un fisier/director cu newname

	// Verific pentru directoare
	if(parent->head_children_dirs!=NULL)
	{
		Dir *aux2;
		aux2=parent->head_children_dirs;
		if(strcmp(aux2->name,newname)==0)
			goto afisare2;
		while(aux2->next!=NULL)
		{
			aux2=aux2->next;
			if(strcmp(aux2->name,newname)==0)
				goto afisare2;
		}
	}
	// Daca nu exista un director cu acel nume, verific pentru fisiere
	if(parent->head_children_files!=NULL)
	{
		File *faux2;
		faux2=parent->head_children_files;
		if(strcmp(faux2->name,newname)==0)
			goto afisare2;
		while(faux2->next!=NULL)
		{
			faux2=faux2->next;
			if(strcmp(faux2->name,newname)==0)
				goto afisare2;
		}
	}

	// Daca exista un director/fisier cu numele vechi si nu exista niciun
	// fisier/director cu noul nume, incep schimbarea
	if(exd==1||exf==1)
	{
		// Daca este un fisier
		if(exf==1)
		{
			// Sterg vechiul fisier
			rm(parent,oldname);
			// Creez noul fisier
			touch(parent,newname);
			return;
		}

		// Daca este un director
		// Retin adresele catre head_children_dirs si head_children_files
		Dir *dircp=aux->head_children_dirs;
		File *filecp=aux->head_children_files;
		// Sterg legaturile vechiului director catre directoarele si fisierele
		// pe care le contine
		aux->head_children_dirs=NULL;
		aux->head_children_files=NULL;
		// Sterg vechiul director
		rmdir(parent,oldname);
		// Creez noul director
		mkdir(parent,newname);
		// Creez legaturile catre direcctoarele si fisierele pe care le continea
		// directorul sters
		aux=parent->head_children_dirs;
		while(aux->next!=NULL)
			aux=aux->next;
		aux->head_children_dirs=dircp;
		aux->head_children_files=filecp;
		return;
	}

afisare1:
	printf("File/Director not found\n");
	return;
afisare2:
	printf("File/Director already exists\n");
}

int main (int argc, char **argv) {
	char *buffer=malloc(MAX_INPUT_LINE_SIZE*sizeof(char));
	
	// Instantiez directorul home si ii aloc memorie
	Dir *home;
	home=malloc(sizeof(Dir));
	home->parent=NULL;
	home->head_children_files=NULL;
	home->head_children_dirs=NULL;
	home->next=NULL;
	home->name=malloc(5*sizeof(char));
	strcpy(home->name,"home");


	// Instantiez un cursor pentru a ma putea deplasa in directoare
	Dir *cursor;
	cursor=home;
	
	// Rezolvarea comenzilor
	while(fgets(buffer,MAX_INPUT_LINE_SIZE,stdin))
	{
		char *tok=strtok(buffer," ");
		if(strcmp(tok,"mkdir")==0)
		{
			tok=strtok(NULL,"\n");
			mkdir(cursor,tok);
		}
		if(strcmp(tok,"ls\n")==0)
			ls(cursor);
		if(strcmp(tok,"stop\n")==0)
		{
			stop(home);
			free(home->name);
			free(home);
			free(buffer);
			return 0;
		}
		if(strcmp(tok,"touch")==0)
		{
			tok=strtok(NULL,"\n");
			touch(cursor,tok);
		}
		if(strcmp(tok,"rm")==0)
		{
			tok=strtok(NULL,"\n");
			rm(cursor,tok);
		}
		if(strcmp(tok,"rmdir")==0)
		{
			tok=strtok(NULL,"\n");
			rmdir(cursor,tok);
		}
		if(strcmp(tok,"cd")==0)
		{
			tok=strtok(NULL,"\n");
			cd(&cursor,tok);
		}
		if(strcmp(tok,"pwd\n")==0)
		{
			char *cale=pwd(cursor);
			printf("%s\n",cale);
			free(cale);
		}
		if(strcmp(tok,"tree\n")==0)
		{
			tree(cursor,0);
		}
		if(strcmp(tok,"mv")==0)
		{
			tok=strtok(NULL," ");
			char *tok2=strtok(NULL,"\n");
			mv(cursor,tok,tok2);

		}
	}
	
	free(buffer);
	return 0;
}