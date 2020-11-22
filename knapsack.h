//
// Created by Hugo Ziviani on 11/21/20. Using by reference:
// https://www.geeksforgeeks.org/implementation-of-0-1-knapsack-using-branch-and-bound/?ref=rp

#ifndef KNAPSACK_BOUND_KNAPSACK_H
#define KNAPSACK_BOUND_KNAPSACK_H

#include <queue>
#include <fstream>

using namespace std;

// Structure for Item which store weight and corresponding
// prof of Item
struct Item
{
    int id;
    float weight;
    int prof;
};


// Nó para a confecção da arvore de decisao para a poda
struct Node
{
    // level  --> Level of node in decision tree (or index
    //             in arr[]
    // weight --> Profit of nodes on path from root to this
    //            node (including this node)
    // bound ---> Upper makeBound of maximum weight in subtree
    //            of this node/
    int level, profit, bound;
    float weight;
};
void printItems(vector<Item> items){
    for (auto it : items){
        cout << "ID:" << it.id
             << " Lucro: " << it.prof
             << " Peso: " << it.weight << endl;
    }
}

// funcao parametro para ordenar o vetor de valores pelo peso
bool compareItemsByWeight(Item a, Item b){
        double reason1, reason2;
        reason2 = (double)b.prof / b.weight;
        reason1 = (double)a.prof / a.weight;
    return reason1 > reason2;
}

// Returns makeBound of weight in subtree rooted with u.
// This function mainly uses Greedy solution to find
// an upper makeBound on maximum weight.
int makeBound(Node u, int n, int W, vector<Item> arr)
{
    // if weight overcomes the knapsack capacity, return
    // 0 as expected makeBound
    if (u.weight >= W)
        return 0;

    // initialize makeBound on weight by current weight
    int profit_bound = u.profit;

    // start including items from index 1 more to current
    // item index
    int j = u.level + 1;
    int totweight = u.weight;

    // checking index condition and knapsack capacity
    // condition
    while ((j < n) && (totweight + arr[j].weight <= W))
    {
        totweight    += arr[j].weight;
        profit_bound += arr[j].prof;
        j++;
    }

    // If k is not n, include last item partially for
    // upper makeBound on weight
    if (j < n)
        profit_bound += (W - totweight) * arr[j].prof /
                        arr[j].weight;

    return profit_bound;
}

// Returns maximum weight we can get with capacity W
vector<Item> knapsack(int W, vector<Item> &items, int n, int** conflictMatrixReference){
    vector<Item> bagSolution;

// TODO: se nao funcionar mudar o +n
//    sort(items.begin(), items.end()+n, compareItemsByWeight);
    sort(items.begin(), items.end(), compareItemsByWeight);

    // fila para processar os elementos
    queue<Node> queueToProcess;
    Node root, node;
    // nó do início
    root.level = -1;
    root.profit = 0;
    root.weight = 0;
    queueToProcess.push(root);
    // salva o limite inferior do lucro (maximo lucro até o momento).
    // separa os itens para processamento de cada nó
    int maxProfit = 0;
    while (!queueToProcess.empty()){
        root = queueToProcess.front(); // tira o primeiro nó p/ processar
        queueToProcess.pop();
        if (root.level == -1) // se é raiz, inicia com zero
            node.level = 0;
        //se nao tem nada no proximo level continua
        //TODO: talvez procurar aqui se existe conflito
        // criar uma bag solution, que vai guardando os nós/chaves que sao utilizados
        if (root.level == n - 1)
            continue;

        // se não for o ultimo nó, vamos para o próximo nível
        // para computar o lucro dos nós-filhos
        node.level = root.level + 1;

        // Pega o atual item, seu lucro, e atualiza para ser a nova raiz da busca.
        node.weight = root.weight + items[node.level].weight;
        node.profit = root.profit + items[node.level].prof;


        //Se o lucro for menor que a capacidade e maior que o lucro anterior, atualiza o lucro maximo
        if (node.weight <= W && node.profit > maxProfit){
            maxProfit = node.profit;
            bagSolution.push_back(items[node.level]);
        }




        // pega o maior valor resultante da poda para decidir
        // e ver se adiciona ou nao na fila de processos
        node.bound = makeBound(node, n, W, items);

        // se o resultado da função for maior que o lucro, só coloca na fila por consideracao
        if (node.bound > maxProfit)
            queueToProcess.push(node);

        // faça a mesma operacao, sem alocar o item na mochila
        node.weight = root.weight;
        node.profit = root.profit;
        node.bound = makeBound(node, n, W, items);
        if (node.bound > maxProfit){
            queueToProcess.push(node);


        }

    }
    cout<<"MaxProfit: "<<maxProfit<<endl;
    return bagSolution;
}

int** alocateConflictMatrix(int sizeMatrix)
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
    auto height = sizeMatrix; //matriz quadrada
    auto width = sizeMatrix;
    for (int h = 0; h < height; h++){
        for (int w = 0; w < width; w++){
            cout<<conflictsMatrix[h][w];
        }
        cout<<endl;
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
        auto conflictsMatrix = alocateConflictMatrix(numberOfItens);

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
            if (it != items.end()){ it->prof = weight;}
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
