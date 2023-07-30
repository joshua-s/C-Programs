#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Joshua Sepulveda
//CSIT231_SP23	

void revstr(char *strn){
	int i, len, temp;
	len = strlen(strn);
	for(i = 0; i < len/2; i++){
		temp = strn[i];
		strn[i] = strn[len - i - 1];
		strn[len - i - 1] = temp; //Fucntion reverses string on command and stores it int he temp variable. 
	}
}

char str[50];
int length;
int main(){
	char answer;
	printf("Enter a word (no more than 25 characters) ");
	scanf("%s", &str);
	length = strlen(str);
	if (length > 25){
		printf("The word is greater than 25 characters long ");
		exit(2);
	}
	else if (strlen(str) < 0){
		printf("A string wasn't entered");
		exit(1);
	}
	printf("(%s), Is this the word you entred(y/n) ", str);
	scanf("%s", &answer);
	if (answer == 'y'){
		system("clear");
		printf("The word is spelled: %s", str);
		revstr(str);
		printf("\nThe word spelled backwards is : %s", str);
		revstr(str);
		printf("\nFor those that read slowly: \n");
		for(int i = 0; i < length; i++){
			printf("%c - ", str[i]);
			sleep(1); 
		}
		printf("\nThe word entered was %i characters long ", length - 1);
		str[3] = '\0';
		printf("\nHere's a suggested abbreviation: %s \n", str);
	}
	else{
		printf("Execute the code again with correct input\n");
		exit(3);
	}
}
