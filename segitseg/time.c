/*** Example of time handling ***/
#include<stdio.h>
#include<time.h>
#include<unistd.h>

char *Months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
char *Days[7]    = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

int main(){
  time_t T1;
  int T2, T3;
  struct tm *T4;

  T2 = time(&T1);
  printf("The present time is: %d\n",T2);
  printf("This means: %s",ctime(&T1));

  T4 = localtime(&T1);
  printf("So the local time is: %d %s %d  ",(*T4).tm_year+1900,Months[(*T4).tm_mon],(*T4).tm_mday);
  printf("%d:%d:%d\n",(*T4).tm_hour,(*T4).tm_min,(*T4).tm_sec);
  printf(" It is the %dth day of the year.\n",(*T4).tm_yday);
  printf(" It is %s.\n",Days[(*T4).tm_wday]);

  sleep(2);        // 2.0 seconds
  usleep(1500000); // 1.5 seconds

  T3 = time(&T1);
  printf("It took: %ds\n",T3-T2);
  return 0;
  }
