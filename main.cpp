#include <iostream>
#include <list>
#include "knapsack.h"
#include "later.h"
int main()
{

    list<string> paths =
            {{"n50c36dc0.64.knpc"},
            {"n100c29dc0.60.knpc"},
            {"n150c50dc0.36.knpc"},
            {"n200c27dc0.42.knpc"},
            {"n250c49dc0.63.knpc"},
            {"n300c37dc0.30.knpc"},
            {"n350c33dc0.39.knpc"},
            {"n400c31dc0.68.knpc"},
            {"n450c20dc0.69.knpc"},
            {"n500c22dc0.46.knpc"},
            {"n550c14dc0.39.knpc"},
            {"n600c19dc0.58.knpc"},
            {"n650c18dc0.56.knpc"},
            {"n700c49dc0.63.knpc"},
            {"n750c46dc0.38.knpc"},
            {"n800c27dc0.56.knpc"},
            {"n850c42dc0.32.knpc"},
            {"n900c25dc0.40.knpc"},
            {"n950c50dc0.51.knpc"},
            {"n1000c11dc0.62.knpc"}};

    for(auto pathToRun : paths){

        readFileAndFeedAlgorithm(pathToRun);
    }



    return 0;
}