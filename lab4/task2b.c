#include "util.h"
		#define SYS_WRITE 4
		#define STDOUT 1
		#define SYS_getdents 141
		#define SYS_OPEN 5
    #define STDERR 2
    #define BUF_SIZE 8192
struct linux_dirent {
  long          d_ino;
  int         d_off;
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
  int is_d=0;
  char *num;
  char enter[1]="\n";
  char name_length;
  int k, suffixLocation= -1;

  for (k=0 ; k<argc ; k++){
    if ((strcmp(argv[k],"-s")==0)){
      k+=1;
      suffixLocation=k;
    }
    if ((strcmp(argv[k],"-d")==0))
      is_d=1;  
  }
  fd = system_call(SYS_OPEN,".",0,0);
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
      int p,count=0; 
      for (p=0; d->d_name[p]!=0; p++)
        count+=1;
      int nameLength=count;

      if (argc>2 && suffixLocation != -1){
        if (d->d_name[nameLength-1]==*argv[suffixLocation]){
          if(d_type!=4){
            name_length=system_call(SYS_WRITE,STDOUT,d->d_name,d_type);
            system_call(SYS_WRITE,STDOUT,space,1);
            if(is_d==1&&d_type!=4){
              system_call(SYS_WRITE,STDERR,"length: ",8);
              num=itoa(name_length);
              system_call(SYS_WRITE,STDERR,num,1);
              system_call(SYS_WRITE,STDERR,enter,1);  
            }
          }
        }
      }
      else {
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
      bpos += d->d_reclen;
    }
  }
  return 0;
}
