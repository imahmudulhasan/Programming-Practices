#include<stdio.h>
int main(){
    int n,sum = 0;
    printf("Enter an number: \n");
    scanf("%d",&n);
    for(int i=1, j=n; i<=n && j>=1; i++,j--){
        sum += i;
        printf("the sum is : %d\n",sum);
        printf("%d\n",j);  
    }
    
    // for(int i=n; i>=1; i--){
    //     printf("%d\n",i);
    // }

    return 0;
}