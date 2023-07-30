#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <secret_word>\n", argv[0]);
    return 1;
  }

  char *secretWord = argv[1];
  int fd;
  char *myfifo = "/home/admsu.montclair.edu/sepulvedaj1/groucho.fifo";
  char arr1[80], guess[200];

  for (int index = 0; secretWord[index]; index++) {
    secretWord[index] = toupper(secretWord[index]);
  }

  while (1) {
    fd = open(myfifo, O_RDONLY);
    read(fd, arr1, sizeof(arr1));
    close(fd);

    printf("\n Contestant: %s\n", arr1);

    if (strcmp(arr1, secretWord) == 0) {
      strcpy(guess, "\nHooray for Captain Spaulding the African Exlporer...did "
                    "someone call me Schnorer...hooray, hooray, hooray!!!\nYou "
                    "just said the secret word and you have won $100!!!!\n");
    } else {
      strcpy(guess, "Sorry that's not the secret word");
    }

    fd = open(myfifo, O_WRONLY);
    write(fd, guess, strlen(guess) + 1);
    close(fd);

    if (strcmp(arr1, secretWord) == 0) {
      break;
    }
  }

  return 0;
}
