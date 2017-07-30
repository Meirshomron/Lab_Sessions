#include <stdio.h>
#include <string.h>
 
char encrypt(char c){
	if(c>=' '&&c<='~')
		c+=3;
return c;
}

char decrypt(char c){
	if(c>=' '&&c<='~')
		c-=3;
return c;
}

char xprt(char c){
	if(c!=0)
		printf("0x%X\n",c);
return c;
}

char cprt(char c){
	if(c>=' '&&c<='~')
		printf("%c\n",c);
	else
		printf("%c\n",'.');
return c;
}

char my_get(char c){
return fgetc(stdin);
}

char quit(char c){
	exit(c);
}

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}

char* map(char *array, int array_length, char (*f) (char)){
	char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
	int i;
	for(i=0;i<array_length;i++){
		mapped_array[i]=f(array[i]);
	}
return mapped_array;
}
 
typedef struct fun_desc {
  char *name;
  char (*fun)(char);
}fun_desc;

int main(int argc, char **argv){
char carray[5]="";
struct fun_desc menu[] = { { "Censor", &censor }, { "Encrypt", &encrypt }, { "Decrypt", &decrypt }, 
						   { "Print hex", &xprt } , { "Print string", &cprt }, { "Get string", &my_get },
						   { "Quit", &quit },{ NULL, NULL } };
int i;
int j;
int range=sizeof(menu)-121;
int c;
char * str;
while(1){
	printf("%s\n","Please choose a function:" );
	for(i=0;i<range;i++){
		printf("%d) %s\n",i,menu[i].name);
	}
	printf("%s", "Option: ");
	c=(getc(stdin)-'0');
	if(c<=range-1&&c>=0)
		printf("%s\n","Within bounds" );
	else{
		printf("%s\n","Not within bounds" );
		quit(0);
	}
	getc(stdin);
	str=map(carray,5,menu[c].fun);
	for(j=0;j<5;j++){
		if(str[j]=='\0')
			break;
		carray[j]=str[j];
	}
	printf("%s\n\n","DONE.");
}
return 0;
}