#include <stdio.h>

int main() {
    // String array banaisi
    char items[3][20] = {"Pizza", "Burger", "Cold Drink"};

    //price store korsi
    int prices[3] = {250, 220, 30};

    int choice, quantity;

    printf("===Food Order System===\n");
    printf("\n Main Menu  \n");
    for (int i = 0; i < 3; i++) {
        printf("%d - %s (%d tk)\n", i, items[i], prices[i]);
    }
    printf("\nEnter the item number you want to buy: ");
    scanf("%d", &choice);

    if (choice >= 0 && choice < 3) {
        printf("Enter the quantity: ");
        scanf("%d", &quantity);

        int total = prices[choice] * quantity;

        printf("\nYou ordered %d %s(s).\n", quantity, items[choice]);
        printf("Total Cost: %d tk\n", total);
    } else {
        printf("\nInvalid item number! Please try again.\n");
    }

    return 0;
}
