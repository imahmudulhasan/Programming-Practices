#include<stdio.h>
int main()
{   int n;
    do{
       printf("Enter an even number : \n");
       scanf("%d",&n);

       if(n%2!=0){
        break;
       }
    }
    while(1);
    
    printf("Not an even number \nThank You");

    return 0;
}