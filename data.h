#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <error.h>
#include <sys/ioctl.h>
#include <time.h>

char *data =
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "1234567890!@#$%^&*()_+=-?><"
  ":;{}[]/*-+$#!$#$@#$$#@!$@!$#\n";
