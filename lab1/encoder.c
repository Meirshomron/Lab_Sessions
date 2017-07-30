#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  
int c;
int isKey=0;
int i;
int sum;
int writeTo=0;
char sign;
char output[30];
char *from="stdin";
char * key;
FILE *fp;
FILE *fw;

for(i=1;i<argc&&isKey==0;i++ ){
    key=argv[i];
    sign=key[0];
    sum=key[1]-'0';
    if((sign=='+'||sign=='-')&&(sum<=9&&sum>=-9))
       isKey=1;    
}
for(i=1;i<argc;i++){
    if(strcmp(argv[i],"-i")==0){
    	fp=fopen(argv[i+1],"r");
    	from="fp";
  	}
}
for(i=1;i<argc;i++){
  	if(strcmp(argv[i],"-o")==0){
  	  fputs("Please enter name of file: ",stdout);
  	  fgets(output,30,stdin);
  	  output[strlen(output)-1]='\0';
  	  fw=fopen(output,"w");
  	  writeTo=1;
    }
}
if(strcmp(from,"fp")==0){
  if(fp==NULL){
    perror("FILE cannot be opened for reading");
    return -1;
  }
    while((c=fgetc(fp))!=EOF){
        if(c>='A'&&c<='Z')
            c+='a'-'A' ;
           if(c>='a'&&c<='z'){
            if(isKey==1){
            if(sign=='+')
              c+=sum;
            if(sign=='-')
              c-=sum;
            if(c<'a')
              c+='z'-'a'+1;
            if(c>'z')
              c-='z'-'a'+1;
           }
       }
       if(writeTo==0)
        fputc(c,stdout);
    else
      putc(c,fw);

    }
        printf("%c",'\n');

    if(writeTo==1)
      fclose(fw);
fclose(fp);
return 0;
}
    while((c=fgetc(stdin))!=EOF){
        if(c>='A'&&c<='Z')
            c+='a'-'A' ;
           if(c>='a'&&c<='z'){
            if(isKey==1){
            if(sign=='+')
              c+=sum;
            if(sign=='-')
              c-=sum;
            if(c<'a')
              c+='z'-'a'+1;
            if(c>'z')
              c-='z'-'a'+1;
           }
       }
       if(writeTo==0)
        fputc(c,stdout);
        else
        putc(c,fw);


    }
if(writeTo==1)
   fclose(fw);
return 0;
}