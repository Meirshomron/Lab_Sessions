#include "util.h"
#define	SYS_CREAT	8
#define	SYS_OPEN	5																															
#define	SYS_WRITE	4																															
#define	SYS_READ	3																															
#define STDERR		2
#define STDOUT		1
#define STDIN		0


void print_debug(int j,int g,int fw_id,int read_from_id){
    	char* num;
    	char enter[1]="\n";
    	if(j>0){
            system_call(SYS_WRITE,STDERR,"Id:8",4);
            system_call(SYS_WRITE,STDERR,enter,1);
            num=itoa(fw_id);
            system_call(SYS_WRITE,STDERR,num,1);
            system_call(SYS_WRITE,STDERR,enter,1);
        }
        else{
        	system_call(SYS_WRITE,STDERR,"stdout",6);
        	system_call(SYS_WRITE,STDERR,enter,1);  
        }

        if(g>0){
            system_call(SYS_WRITE,STDERR,"Id:5",4);
            system_call(SYS_WRITE,STDERR,enter,1);
            num=itoa(read_from_id);
            system_call(SYS_WRITE,STDERR,num,1);
            system_call(SYS_WRITE,STDERR,enter,1);  
        }
        else{
        	system_call(SYS_WRITE,STDERR,"stdin",6);
        	system_call(SYS_WRITE,STDERR,enter,1);  
        }
    }



int main(int argc,char *argv[]){
int fw_id=1;
int c;
char enter[1]="\n";
int i;
int read_from_id=0;
char error[]="The file does not open\n";
char byte[1];
int is_d=0;
int j=0;
int g=0;
for(i=1;i<argc;i++ ){
    if(strcmp(argv[i],"-o")==0){
        fw_id=system_call(SYS_CREAT,argv[i+1],0666);
		j=i+1;
	    }
    if(strcmp(argv[i],"-i")==0){
    	read_from_id=system_call(SYS_OPEN,argv[i+1],0,0777);
    	g=i+1;
	}

    if(strcmp(argv[i],"-d")==0)
        is_d=1;
}
    if(read_from_id<0){
        system_call(SYS_WRITE,1,error,23);
        return -1;
    }

    if(is_d==1)
    	print_debug(j,g,fw_id,read_from_id);
    
    while((c=system_call(SYS_READ,read_from_id,byte,1))!=0){
        if(byte[0]>64&&byte[0]<91)
            byte[0]+=32;

        system_call(SYS_WRITE,fw_id,byte,1);
    }
    if(read_from_id!=0)
    system_call(SYS_WRITE,fw_id,enter,1);

    return 0;
}


