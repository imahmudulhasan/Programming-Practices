#include<stdio.h>
int main()
{   int n;
    do{
       printf("Enter an number : \n");
       scanf("%d",&n);

       if(n % 7 ==0){
        break;
       }
    }
    while(1);
    
    printf(" \nThank You");

    return 0;
}