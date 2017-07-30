 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintHex(char buffer[], int length){
int i;
for(i=0;i<length;i++)
	printf("%02X ",(unsigned char)buffer[i]);
}

typedef struct virus virus;
 
struct virus {
    unsigned short length;
    char name[16];
    char *signature;
};
typedef struct link link;
 
struct link {
    virus *v;
    link *next;
};
void detect_virus(char *buffer, link *virus_list, unsigned int size){
   	int i=0;
   	for(;i<size;i++){
   		link * temp=virus_list;
   		while(temp!=NULL){
    		if(temp->v->length+i<=size)
   				if(memcmp(temp->v->signature,buffer+i,temp->v->length)==0){
   					printf("%s %d\n","The starting byte location in the suspected file is: ",i);
   					printf("%s %s\n","The virus name is: ",temp->v->name);
   					printf("%s %d\n\n","The size of the virus signature is: ",temp->v->length);
    			}
    		temp=temp->next;
    	}
   	}
}
/* Print the data of every link in list. Each item followed by a newline character. */
void list_print(link *virus_list){
	while(virus_list){
		virus tmp_virus=*virus_list->v;
			printf("Virus name: %s\n",tmp_virus.name);
			printf("Virus size: %d\nSignature:\n",tmp_virus.length);
			PrintHex(tmp_virus.signature,tmp_virus.length);
			printf("\n\n");
		virus_list=virus_list->next;
	}
}
/* Add a new link with the given data to the list 
   (either at the end or the beginning, depending on what your TA tells you),
   and return a pointer to the list (i.e., the first link in the list).
   If the list is null - create a new entry and return a pointer to the entry. */ 
link* list_append(link* virus_list, virus* data){
		struct link *new_link=malloc (sizeof (struct link));
		new_link->v=data;
		new_link->next=NULL;
		if(!virus_list)
			virus_list=new_link;
		else{
			struct link *curr=virus_list;
			struct link  *the_next=virus_list->next;
			while(the_next){
				the_next=the_next->next;
				curr=curr->next;
			}
			the_next=new_link;
			curr->next=the_next;
		}
		return virus_list;
}

 /* Free the memory allocated by the list. */
void list_free(link *virus_list){
	link * temp=virus_list;

	while(temp!=NULL){
		temp=temp->next;
		free(virus_list->v->signature);
		free(virus_list->v);
		free(virus_list);
		virus_list=temp;
	}
} 

 int main(int argc, char **argv) {
struct link *virus_list=NULL;
int signature_size;
char name_type[1];
char full_size[2];
char buffer[10000];
int buffered_size;
FILE *fp=fopen(argv[1],"r");
FILE *mainf=fopen(argv[2],"r+");
buffered_size=fread(buffer,1,10000,mainf);
int name_limited=0;
char number_notation[1];
fread(number_notation,1,1,fp);
fread(name_type,1,1,fp);
name_limited=0;
if(name_type[0]==0)
	name_limited=1;
while(fread(full_size,1,2,fp)>0){
	if(number_notation[0]==0)
		 signature_size=(full_size[0]+full_size[1]*100)-18;
	
	else
		 signature_size=(full_size[1]+full_size[0]*100)-18;
	
		    virus* vir= malloc (sizeof(virus));
		    vir->signature=malloc(signature_size);
	vir->length=signature_size;
	if(name_limited==1)
		fread(vir->name,1,16,fp);	
	else{
		int i=0;
		char c;
		do{
		c=fgetc(fp);
		vir->name[i]=c;	
		i++;
		}
		while(c!='\0');
	}
	fread(vir->signature,1,signature_size,fp);
	virus_list=list_append(virus_list,vir);
}

detect_virus(buffer, virus_list, buffered_size);
list_free(virus_list);
fclose(fp);
fclose(mainf);

return 0;
}