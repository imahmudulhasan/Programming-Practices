#include<stdio.h>
int main(){

    int num[3];
     printf("Enter Numbers \n");
    // scanf("%d %d",&num[0],&num[1]);
    // printf("Enter Two Numbers %d %d", num[0],num[1]);
    for(int i=0; i<=2; i++){
        
        scanf("%d",&num[i]);
        if(num[i] %2 == 0){
            printf("this is an even number\n");
        }
         else{
            printf("this is not an even number\n");
        }
        


    }
   


    return 0;
}