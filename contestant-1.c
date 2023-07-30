#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
char userInput[80];
void signalCc(int signal) {
  static int controlCnumber = 0;
  controlCnumber++;
  if (controlCnumber == 3) {
    printf("\n Control C has been entered: %d times. EXITING NOW",
           controlCnumber);
    exit(1);
  }
}
void signalCz(int signal) {
  static int controlZnumber = 0;
  controlZnumber++;
  if (controlZnumber == 3) {
    printf("\n Control Z has been entered: %d times. EXITING NOW",
           controlZnumber);
    exit(1);
  }
}
int main() {
  int fd1;
  char *myfifo = "/home/admsu.montclair.edu/sepulvedaj1/groucho.fifo";
  mkfifo(myfifo, 0666);
  char str1[80], str2[80];
  signal(SIGINT, signalCc);
  signal(SIGTSTP, signalCz);
  while (1) {
    printf("\n Please Enter A String that is 80 characters long: \n");
    fgets(userInput, 80, stdin);
    userInput[strcspn(userInput, "\n")] = 0;
    for (int i = 0; userInput[i]; i++) {
      userInput[i] = toupper(userInput[i]);
    }
    fd1 = open(myfifo, O_WRONLY);
    write(fd1, userInput, strlen(userInput) + 1);
    close(fd1);
    fd1 = open(myfifo, O_RDONLY);
    read(fd1, str1, sizeof(str1));

    close(fd1);
    printf("Groucho says: %s\n ", str1);
  }
  return 0;
}
