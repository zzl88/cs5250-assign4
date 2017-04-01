#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/ioctl.h>  
//needed for IO things. Attention that this is different from kernel mode 
int lcd; 
#define LCD_IOC_TYPE  'k' 
#define LCD_IOC_HELLO _IO(LCD_IOC_TYPE,   1) 

void test()
{ 
  int k, i, sum; 
  char s[3]; 

  memset(s, '2', sizeof(s)); 
  printf("test begin!\n"); 

  k = write(lcd, s, sizeof(s)); 
  printf("written = %d\n", k); 
  k = ioctl(lcd, LCD_IOC_HELLO); 
  printf("result = %d\n", k); 

} 

int main(int argc, char **argv) 
{ 
  lcd = open("/dev/lcd", O_RDWR); 
  if (lcd == -1) { 
    perror("unable to open lcd"); 
    exit(EXIT_FAILURE); 
  } 


  test(); 

  close(lcd); 
  return 0; 
}
