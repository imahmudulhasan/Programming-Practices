#include<stdio.h>
int main() {
  int myNumbers[10];
  // int arrlen = sizeof(myNumbers) / sizeof(myNumbers[0]);
  int i;
  printf("Enter 0-5 Array :\n");
  for(int i=0; i<5; i++){
  scanf("%d",&myNumbers[i]);
  }
  printf("You entered: \n");
  for (i = 0; i <5; i++) {
    printf("%d\n", myNumbers[i]);
  }
  
  return 0;
}