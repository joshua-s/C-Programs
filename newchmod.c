#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct files {
  int readPerm;
  char name[100];
  char permissions[10];
}; // creates the file struct

struct files nfiles[100]; // initialize a struct of nfiles

int readPerm(char permissions[]) {

  if (permissions[0] != '_') {
    printf("When inputting permissions first thing in the string should be an "
           "underscore");
    return -1;
  }

  for (int i = 0; i < 10; i++) {
    permissions[i] = tolower(permissions[i]);
  }

  if (strlen(permissions) < 10) {
    printf("Incorrect format try again. \n");
    return -1;
  }

  // assigns user values
  int user = 0;
  switch (permissions[1]) {
  case 'r':
    user += 4;
    break;
  case '_':
    user += 0;
    break;
  default:
    printf("Either _ or r should be put into the first value of string index "
           "to assign a permissions. \n");
    return -1;
  }
  switch (permissions[2]) {
  case 'w':
    user += 2;
    break;
  case '_':
    user += 0;
    break;
  default:
    printf("Either _ or w should be put into the second value of string index "
           "to assign a permissions. \n");
    return -1;
  }
  switch (permissions[3]) {
  case 'x':
    user += 1;
    break;
  case '_':
    user += 0;
    break;
  default:
    printf("Either _ or x should be put into the third value of string index "
           "to assign a permissions. \n");
    return -1;
  }

  // assigns group values
  int group = 0;
  switch (permissions[4]) {
  case 'r':
    group += 4;
    break;
  case '_':
    group += 0;
    break;
  default:
    printf("Either _ or r should be put into the fourth value of string index "
           "to assign a permissions. \n");
    return -1;
  }
  switch (permissions[5]) {
  case 'w':
    group += 2;
    break;
  case '_':
    group += 0;
    break;
  default:
    printf("Either _ or w should be put into the fifth value of string index "
           "to assign a permissions. \n");
    return -1;
  }
  switch (permissions[6]) {
  case 'x':
    group += 1;
    break;
  case '_':
    group += 0;
    break;
  default:
    printf("Either _ or x should be put into the sixth value of string index "
           "to assign a permissions. \n");
    return -1;
  }

  // assign others values
  int other = 0;
  switch (permissions[7]) {
  case 'r':
    other += 4;
    break;
  case '_':
    other += 0;
    break;
  default:
    printf("Either _ or r should be put into the seventh value of string index "
           "to assign a permissions. \n");
    return -1;
  }
  switch (permissions[8]) {
  case 'w':
    other += 2;
    break;
  case '_':
    other += 0;
    break;
  default:
    printf("Either _ or w should be put into the eigth value of string index "
           "to assign a permissions. \n");
    return -1;
  }
  switch (permissions[9]) {
  case 'x':
    other += 1;
    break;
  case '_':
    other += 0;
    break;
  default:
    printf("Either _ or x should be put into the ninth value of string index "
           "to assign a permissions. \n");
    return -1;
  }

  // checks if user has read permissions if given execute as well
  if (permissions[3] == 'x' && permissions[1] != 'r') {
    printf("Error, user should also have read permissions if given excecute "
           "permissions. \n");
    return -1;
  }

  // checks if user has read permissions if given write as well
  if (permissions[2] == 'w' && permissions[1] != 'r') {
    printf("Error, group should also have read permissions if given write "
           "permissions.");
    return -1;
  }

  // checks if group has read permissions if given write as well
  if (permissions[5] == 'w' && permissions[4] != 'r') {
    printf("Error, others should also have read permissions if given write "
           "permissions.");
    return -1;
  }

  // checks if others have read permissions if given write as well
  if (permissions[8] == 'w' && permissions[7] != 'r') {
    printf("Error, others should also have read permissions if given write "
           "permissions.");
    return -1;
  }

  // checks for no permissions given
  if (strcmp(permissions, "__________") == 0) {
    printf("No permissions for no groups are allowed, try again.");
    return -1;
  }

  // checks for 777 and 666 file permissions
  if (strcmp(permissions, "_rwxrwxrwx") == 0 ||
      strcmp(permissions, "_rw_rw_rw_") == 0) {
    printf(
        "Error, these permissions can be given but can cause a security risk.");
  }

  // puts the values into three digits which assign permissions
  char perm2Num[4];
  sprintf(perm2Num, "%i%i%i", user, group, other);
  return atoi(perm2Num);
}

// creation of file or changing permissions
void searchFiles(struct files madeFiles) {
  FILE *result;
  char perm2Num[5];
  char command[100];
  strcpy(command, "ls -al ");
  strcat(command, madeFiles.name);
  sprintf(perm2Num, "%04d", madeFiles.readPerm);

  // goes through files
  result = fopen(madeFiles.name, "r");

  if (result == 0) {
    printf("No previous file was found. Creating new file...\n");
    result = fopen(madeFiles.name, "w");
    if (result == 0) {
      printf("No permissions are given to create this file. This part will be "
             "skipped. \n");
    } else { // otherwise
      fclose(result);
      printf("This file is created. permissions will be changed now... \n");
      if (chmod(madeFiles.name, strtol(perm2Num, 0, 8)) < 0) {
        printf("CHMOD ERROR");
      } else {
        printf("permissions are now changed. \n");
        printf("Every action is done for the file(s) at this point.");
        system(command);
      }
    }
  } else { // if the file already exists
    system(command);
    printf("This file already exists. permissions will be checked. \n");
    result = fopen(madeFiles.name, "w");
    if (result == 0) {
      printf("No permissions to be changed. \n");
    } else { // has permissions, now updates the permissions
      fclose(result);
      printf("Chainging the permissions now. \n");
      if (chmod(madeFiles.name, strtol(perm2Num, 0, 8)) < 0) {
        printf("Error with CHMOD");
      } else {
        printf("permissions are changed and all actions completed for this "
               "file. \n");
        system(command);
      }
    }
  }
}

int main(int argv, char **args) {
	system("clear");
	if (argv < 2) { // checks that a number of files has actuallly been entred
                  // into the command line
    printf("Wrong amount of files. You should enter at least 1. \n");
    return -1;
  }
  for (int index = 0; index < atoi(args[1]);
       index++) { // loops through the amount of files entered
    printf("\nEntered %i file(s) to be created/modified. \n", index + 1);
    printf("\nEnter file name: ");
    scanf("%s", nfiles[index].name);
    printf("\nPlease enter the permissions as _rwxrwxrwx: ");
    scanf("%s", nfiles[index].permissions);
    nfiles[index].readPerm = readPerm(nfiles[index].permissions);
    while (nfiles[index].readPerm == -1) {
      printf("\nPlease enter the permissions as _rwxrwxrwx: ");
      scanf("%s", nfiles[index].name);
      nfiles[index].readPerm = readPerm(nfiles[index].permissions);
    }
  }
  for (int index = 0; index < atoi(args[1]); index++) {
    printf("\nYou entered %i file(s) \n", index + 1, nfiles[index].name);
    printf("\nThese are the entered permissions %s\n", nfiles[index].permissions);
    printf("\nThese are the searched permissions: %.03d\n",
           nfiles[index].readPerm);
    searchFiles(nfiles[index]);
    printf("\n");
  }
  printf("\n!DONE!\n");
}//program is finished
//_rw_r__r__
