


struct Stock{
    char name[50];
    float price;
    int number_available;
};

struct Stock stockMarket[3];

struct Buyer{
    char name[50];
    float budget;
    int stocks_owned[3];
};

void buyStock(struct Buyer *buyer, int stockIndex, int quantity) {
    if (stockIndex < 0 || stockIndex >= 3) {
        printf("Invalid stock index.\n");
        return;
    }
    
    if (quantity > stockMarket[stockIndex].number_available) {
        printf("Not enough stocks available.\n");
        return;
    }
    
    float totalCost = stockMarket[stockIndex].price * quantity;
    
    if (totalCost > buyer->budget) {
        printf("Insufficient budget.\n");
        return;
    }
    
    // Update buyer's budget and stocks owned
    buyer->budget -= totalCost;
    buyer->stocks_owned[stockIndex] += quantity;
    
    // Update stock market
    stockMarket[stockIndex].number_available -= quantity;
    
    printf("%s bought %d shares of %s for $%.2f. Remaining budget: $%.2f\n", 
           buyer->name, quantity, stockMarket[stockIndex].name, totalCost, buyer->budget);
}