#include <stdio.h>

long long int fact(long long int n) {
    if (n == 0 || n == 1)  // Base case
        return 1;
      long long int temp =n * fact(n - 1);  
    // return n * fact(n - 1);  // Recursive case
    return temp;
}

int main() {
    long long int n;
    printf("Enter a number to get its factorial: ");
    scanf("%lld", &n);

    long long int factN = fact(n);
    printf("%lld is the factorial\n", factN);

    return 0;
}
