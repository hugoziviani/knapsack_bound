//
// Created by Hugo Ziviani on 11/21/20. Using by reference:
// https://www.geeksforgeeks.org/implementation-of-0-1-knapsack-using-branch-and-bound/?ref=rp

#ifndef KNAPSACK_BOUND_KNAPSACK_H
#define KNAPSACK_BOUND_KNAPSACK_H

#include <queue>
#include <fstream>


using namespace std;


// Structure for Item which store profit and corresponding
// weight of Item
struct Item
{
    int id;
    float profit;
    int weight;
};


// Node structure to store information of decision
// tree
struct Node
{
    // level  --> Level of node in decision tree (or index
    //             in arr[]
    // profit --> Profit of nodes on path from root to this
    //            node (including this node)
    // bound ---> Upper bound of maximum profit in subtree
    //            of this node/
    int level, profit, bound;
    float weight;
};

// Comparison function to sort Item according to
// val/profit ratio
bool cmp(Item a, Item b)
{
    double r1 = (double)a.weight / a.profit;
    double r2 = (double)b.weight / b.profit;
    return r1 > r2;
}

// Returns bound of profit in subtree rooted with u.
// This function mainly uses Greedy solution to find
// an upper bound on maximum profit.
int bound(Node u, int n, int W, Item arr[])
{
    // if profit overcomes the knapsack capacity, return
    // 0 as expected bound
    if (u.weight >= W)
        return 0;

    // initialize bound on profit by current profit
    int profit_bound = u.profit;

    // start including items from index 1 more to current
    // item index
    int j = u.level + 1;
    int totweight = u.weight;

    // checking index condition and knapsack capacity
    // condition
    while ((j < n) && (totweight + arr[j].profit <= W))
    {
        totweight    += arr[j].profit;
        profit_bound += arr[j].weight;
        j++;
    }

    // If k is not n, include last item partially for
    // upper bound on profit
    if (j < n)
        profit_bound += (W - totweight) * arr[j].weight /
                        arr[j].profit;

    return profit_bound;
}

// Returns maximum profit we can get with capacity W
int knapsack(int W, Item arr[], int n)
{
    // sorting Item on basis of weight per unit
    // profit.
    sort(arr, arr + n, cmp);

    // make a queue for traversing the node
    queue<Node> Q;
    Node u, v;

    // dummy node at starting
    u.level = -1;
    u.profit = u.weight = 0;
    Q.push(u);

    // One by one extract an item from decision tree
    // compute profit of all children of extracted item
    // and keep saving maxProfit
    int maxProfit = 0;
    while (!Q.empty())
    {
        // Dequeue a node
        u = Q.front();
        Q.pop();

        // If it is starting node, assign level 0
        if (u.level == -1)
            v.level = 0;

        // If there is nothing on next level
        if (u.level == n-1)
            continue;

        // Else if not last node, then increment level,
        // and compute profit of children nodes.
        v.level = u.level + 1;

        // Taking current level's item add current
        // level's profit and weight to node u's
        // profit and weight
        v.weight = u.weight + arr[v.level].profit;
        v.profit = u.profit + arr[v.level].weight;

        // If cumulated profit is less than W and
        // profit is greater than previous profit,
        // update maxprofit
        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;

        // Get the upper bound on profit to decide
        // whether to add v to Q or not.
        v.bound = bound(v, n, W, arr);

        // If bound weight is greater than profit,
        // then only push into queue for further
        // consideration
        if (v.bound > maxProfit)
            Q.push(v);

        // Do the same thing,  but Without taking
        // the item in knapsack
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, W, arr);
        if (v.bound > maxProfit)
            Q.push(v);
    }

    return maxProfit;
}

int** createConflictMatrix(int sizeMatrix)
{
    unsigned height = sizeMatrix;
    unsigned width = sizeMatrix;
    auto conflictsMatrix = new int*[height];
    for (int h = 0; h < height; h++){
        conflictsMatrix[h] = new int[width];
        for (int w = 0; w < width; w++){
            conflictsMatrix[h][w] = 0;
        }
    }
    return conflictsMatrix;
}

void printConflictMatrix(int ** conflictsMatrix, int sizeMatrix){
    auto height = sizeMatrix;
    auto width = sizeMatrix;
    for (int h = 0; h < height; h++){
        for (int w = 0; w < width; w++){
            cout<<conflictsMatrix[h][w];
        }
        cout<<endl;
    }
}
void printItems(vector<Item> items){
    for (auto it : items){
        cout<<"ID:"<<it.id
            <<" Peso: "<<it.weight
            <<" Lucro: "<<it.profit<<endl;
    }
}

void readFileAndFeedAlgorithm(){
    const int MAX = 100;
    char buff[MAX];
    ifstream fin("/Users/hz/CLionProjects/knapsack_bound/input/n50c36dc0.64.knpc");

    int capacity;
    int numberOfItens;
    vector<Item> items;
    int itemsControl = 0;
    while(fin){ // leitura do arquivo linha a linha
        fin.getline(buff, MAX);
        capacity = stoi(buff);
        fin.getline(buff, MAX);
        numberOfItens = stoi(buff);
        auto conflictsMatrix = createConflictMatrix(numberOfItens);

        while (itemsControl < numberOfItens){ // leitura dos lucros
            fin.getline(buff, MAX);
            auto profit = stof(buff);
            items.push_back({itemsControl + 1, profit, 0}); //insere o elemento no vector de itens
            itemsControl++;
        }
        itemsControl = 0;
        while (itemsControl < numberOfItens){
            fin.getline(buff, MAX);
            auto weight = stof(buff);
            const int the_key =  itemsControl+1;
            //busca pela chave correta para inserir o peso no objeto
            auto it = find_if(items.begin(), items.end(), [the_key] ( const Item& a ) { return a.id == the_key ;});
            if (it != items.end()){ it->weight = weight;}
            itemsControl++;
        }
        fin.getline(buff, MAX);
        itemsControl = 0;
        auto conflicts = stoi(buff);
        while (itemsControl < conflicts){
            // leitura dos pares de conflitos
            auto delim = " ";
            fin.getline(buff, MAX);
            int i, j;
            char * token = strtok(buff, delim);
            i = stoi(token);
            while(token != NULL) {
                j = stoi(token);
                token = strtok(NULL, delim);
            }
            conflictsMatrix[i][j] = 1; // seta 1 onde tem conflito
            itemsControl++;
        }
//        printConflictMatrix(conflictsMatrix, numberOfItens);
        printItems(items);
        break;
    }

}


#endif //KNAPSACK_BOUND_KNAPSACK_H
