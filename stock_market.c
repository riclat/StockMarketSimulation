#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct Stock{
    pthread_mutex_t mutex;
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
   
    pthread_mutex_lock(&stockMarket[stockIndex].mutex);

    if (stockIndex < 0 || stockIndex >= 3) {
        printf("Invalid stock index.\n");
        pthread_mutex_unlock(&stockMarket[stockIndex].mutex);
        return;
    }
    
    if (quantity > stockMarket[stockIndex].number_available) {
        printf("Not enough stocks available.\n");
        pthread_mutex_unlock(&stockMarket[stockIndex].mutex);
        return;
    }
    
    float totalCost = stockMarket[stockIndex].price * quantity;
    
    if (totalCost > buyer->budget) {
        printf("Insufficient budget.\n");
        pthread_mutex_unlock(&stockMarket[stockIndex].mutex);
        return;
    }
    

    buyer->budget -= totalCost;
    buyer->stocks_owned[stockIndex] += quantity;
    
    stockMarket[stockIndex].number_available -= quantity;
    stockMarket[stockIndex].price *= 1.05;
    
    printf("%s bought %d shares of %s for $%.2f. Remaining budget: $%.2f\n
           Shares left: %d, New price: $%.2f\n", 
           buyer->name, quantity, stockMarket[stockIndex].name, totalCost, buyer->budget, 
           stockMarket[stockIndex].number_available, stockMarket[stockIndex].price);

    pthread_mutex_unlock(&stockMarket[stockIndex].mutex);
}


void sellStock(struct Buyer *buyer,int stockIndex,int quantity){

     if (stockIndex<0 || stockIndex>2){
        printf("Wrong stock number! It must be between 0 and 2");
        return;
    }


    pthread_mutex_lock(&stockMarket[stockIndex].mutex);
    if(buyer->stocks_owned[stockIndex]<quantity){
        printf("There is not %d stocks of %s to sell!",quantity,stockMarket[stockIndex].name);
        pthread_mutex_unlock(&stockMarket[stockIndex].mutex);
        return;
    }

   

    int total_price=stockMarket[stockIndex].price*quantity;

    buyer->stocks_owned[stockIndex]-=quantity;
    buyer->budget+=total_price;
    stockMarket[stockIndex].number_available+=quantity;
    stockMarket[stockIndex].price*=0.95;


    printf("Buyer %s sold %d stocks of %s for %.2f$.\nQuantity left of %s:%d, New price of %s:%.2f\n",
            buyer->name,quantity,stockMarket[stockIndex].name,total_price,
            stockMarket[stockIndex].name,stockMarket[stockIndex].number_available,
            stockMarket[stockIndex].name,stockMarket[stockIndex].price);

    pthread_mutex_unlock(&stockMarket[stockIndex].mutex);

}


void* buyer_job(void* arg){

    struct Buyer* buyer=(struct Buyer*)arg;

    while(1){
        int stockIndex=rand()%3;
        int quantity=(rand()%5)+1;
        
        if(rand()%2==0){
            buyStock(buyer, stockIndex, quantity);
        }
        else{
            sellStock(buyer,stockIndex,quantity);
        }
        sleep(2);
    }
    return NULL;
}


void* inflation(void* arg){

    while(1){
        for (int i = 0; i < 3; i++) {
            pthread_mutex_lock(&stockMarket[i].mutex);
            stockMarket[i].price *= 1.1;
            pthread_mutex_unlock(&stockMarket[i].mutex);
        }
        printf("INFLACIJA DELUJE: Cene svih akcija povecane su za  10%.\n");
        sleep(10);
    }

    return NULL;
}