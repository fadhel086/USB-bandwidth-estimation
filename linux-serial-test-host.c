#include "data.h"

#define BUF_SZ 1024 * 1024

int loop_end = 101;
int loop;
char buf[BUF_SZ] = {0};

int baud_rate = 9600;
int fd;

unsigned long diff_ms(const struct timespec *t1, const struct timespec *t2)
{
  struct timespec diff;

  diff.tv_sec = t2->tv_sec - t1->tv_sec;
  diff.tv_nsec = t2->tv_nsec - t1->tv_nsec;

  return ((diff.tv_sec * 1000000000) + (diff.tv_nsec));
}


void build_data(int i)
{
  bzero(buf, BUF_SZ);
  while(i) {
    strcat(buf, data);
    --i;
  } 
}

int write_data(char *buf, int size)
{

  char *ptr = buf;
  int write_ret = 0, outq = 0;
  do {
    if (write_ret < 0) {
      // Some error, just return for time being and coninue with loop
      return write_ret;
    }
    ptr += write_ret;
    size -= write_ret;
    
  } while ((write_ret = write(fd, ptr, size)) < size);

  // Wait till the above write has been fully complete, that is out queue is empty
  ioctl(fd, TIOCOUTQ, &outq);
  while(outq != 0x0){
    ioctl(fd, TIOCOUTQ, &outq);
  }
  
  return write_ret;
}

void waittime(int data_size)
{
  struct timespec delay = {0, 0};
  data_size *= 10; // payload has start_bit, 8-bit data, stop_bit

  float time_req = (float )data_size / baud_rate;
  time_req *= 1000000000; // convert to nano seconds

  // Sleep for above nano secs plus one second to clear the previous transmission
  delay.tv_sec = 1;
  delay.tv_nsec = time_req;
  nanosleep(&delay, NULL);
}

/*
 * Below converts bytes / nano-sec ----> MegaBytes / second
 */
float calc_datarate(long size, long time)
{
  float data_rate = (float)(size * 1000) / time;
  return  data_rate;
}

void write_loop()
{
  int outq, i, ret;
  unsigned long nsec;
  struct timespec before_tx, after_tx;
  
  for (loop = 1; loop < loop_end; ++loop)
    {
      ioctl(fd, TIOCOUTQ, &outq);
      if (outq == 0x0) {
	// You may send the data, first create the data
	build_data(loop);
	if (strlen(buf) >= BUF_SZ) {
	  error(1, errno, "Exceeded BUF size. Stopping experiment");
	}

	// Now get ready to send data, Always better to sleep before doing anything
	sleep(1);
	clock_gettime(CLOCK_MONOTONIC, &before_tx);
	ret = write_data(buf, strlen(buf));
	clock_gettime(CLOCK_MONOTONIC, &after_tx);
	nsec = diff_ms(&before_tx, &after_tx);
	printf("**********************************************************************\n");
	printf("[loop:%d]Wrote %d bytes in %ld nano secs.\n"
               "<data_rate:%.2fMBps>\n",
	                              loop, ret, nsec, calc_datarate(ret, nsec));
      } else {
	// calculate wait for previous data, and wait for that much of time + 'dt' and retry loop
	waittime(strlen(buf));
	--loop;
      }// end of outq else == 0x0
    } // for loop end
}

int main() { 
  struct termios terms;

  fd = open("/dev/ttyACM0", O_RDWR);
  if (fd < 0) {
    error(1, errno, "Failed opening /dev/ttyACM0");
  }

  bzero(&terms, sizeof(terms));
  
  if(tcgetattr(fd, &terms)) {
    error(1, 0, "Failed in getting ttyXXX settings");
  }
  
  // Let's continously write out the data
  write_loop();
  return 0;
}
