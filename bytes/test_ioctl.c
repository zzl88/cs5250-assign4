#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/ioctl.h>  
//needed for IO things. Attention that this is different from kernel mode 
int lcd; 
struct dev_msg_t {
  char data[100];
};
#define LCD_IOC_TYPE  'k' 
#define LCD_IOC_HELLO _IO(LCD_IOC_TYPE,   1) 
#define LCD_IOC_WRITE _IOW(LCD_IOC_TYPE, 2, struct dev_msg_t)
#define LCD_IOC_READ _IOR(LCD_IOC_TYPE, 3, struct dev_msg_t)

void test()
{ 
  int k, i, sum; 
  char s[3]; 
  struct dev_msg_t dev_msg;
  memset(dev_msg.data, 0, sizeof(dev_msg.data));
  memset(s, '2', sizeof(s)); 
  printf("test begin!\n"); 

  k = write(lcd, s, sizeof(s)); 
  printf("written = %d\n", k); 
  // k = ioctl(lcd, LCD_IOC_HELLO); 
  // printf("result = %d\n", k); 

  strcpy(dev_msg.data, "ioctl_write");
  k = ioctl(lcd, LCD_IOC_WRITE, &dev_msg);
  printf("write result = %d\n", k);

  memset(dev_msg.data, 0, sizeof(dev_msg.data));
  printf("cleara msg = %s\n", dev_msg.data);
  k = ioctl(lcd, LCD_IOC_READ, &dev_msg);
  printf("read result = %d msg = %s\n", k, dev_msg.data);
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
