#include <iostream>
# include "knapsack.h"

int main()
{
    int W = 10;   // Weight of knapsack
    vector<Item> arr = {{0,2, 40},
                        {1,3.14, 50},
                        {2,1.98, 100},
                        {3,5, 95},
                        {4,3, 30}};
//    int n = sizeof(arr) / sizeof(arr[0]);
    int n = arr.size();
    auto s = knapsack(W, arr, n, nullptr);

    auto sumW = 0.0;
    auto sumP = 0.0;

    for(auto v:s){
        sumW+= v.weight;
        sumP+= v.prof;
    }
    cout<<"Soma do peso: "<<sumW<<endl;
    cout<<"Soma do lucro: "<<sumP<<endl;
    printItems(s);

//    readFileAndFeedAlgorithm();

//    auto m = alocateConflictMatrix(50);
//    printConflictMatrix(m, 50);

    return 0;
}