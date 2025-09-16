#include<stdio.h>
int main() {
int n;
printf("Enter an number: \n");
scanf("%d",&n);
// int i=0;
// while(i<=n)
// {printf("%d\n",i);
//     i++;
// }
// for(int i=0; i<=n; i++)
// {
//     printf("%d\n",i);
// }
int i=n;
do{
    printf("%d\n",i);
    i--;
    }
while(1<=i);
return 0;
}