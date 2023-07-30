#include <stdio.h>

/*
 *Joshua Sepulveda
 */
//the following variables account for the melting temperatures of the following metals

double gld = 1064.58;
double irn = 1535.00;
double cpr = 1084.60;
double led = 327.60;
double znc = 419.73;
char ans;
double ans1;

void main(){
	printf("Enter a temperature to be tested ");
	scanf("%lf", &ans1);
	printf("Is this temperature in Fahrenheit or Celsius (Enter f or c)? ");
	scanf("%s", &ans);
	if (ans == 'c'){
		double tempk = ans1 + 273;
		if(tempk < 0){
			printf("The temperature is less than absolute zero there might be an input error\n");
		}	
		printf("The temperature in C is %.2lf, in F is %.2lf and K is %.2lf", ans1, ((ans1 * (1.8)) + 32), (ans1 + 273));
	}
	else if(ans == 'f'){
		double conv = (ans1 * (1.8)) + 32;
		double tempk = ans1 + 273;	
		if(tempk < 0){
			printf("The temeprature is less than aboslute zero there might be an input error\n");
		}
		printf("The temperature in F is %.2lf, in C is %.2lf and K is %.2lf", conv, ans1, (conv + 273)); 
	}
	if(ans1 == gld){
	       printf("\nGold should be melted at this temperature");
	}
	else{
		printf("\nGold should melt by raising the temperature %.2lf degrees", (gld - ans1));
	}
	if (ans1 == irn){
		printf("\nIron should be melted at this temperature");
	}
	else{
		printf("\nIron should melt by raising the temperature %.2lf degrees", (irn - ans1));
	}
	if(ans1 == cpr){
		printf("\nCopper should be melted at this temperature");
	}	
	else{
		printf("\nCopper should melt by raising the temperature %.2lf degrees", (cpr - ans1));
	}
	if(ans1 == led){
		printf("\nLead should be melted at this temperature");
	}
	else{
		printf("\nLead should melt by raising the temperature %.2lf degrees", (led - ans1));
	}
	if(ans == znc){
		printf("\nZinc should be melted at this temperature");
	}
	else{
		printf("\nZinc should melt by raising the temperature %.2lf degrees", (znc - ans1));
	}
	return;
}
