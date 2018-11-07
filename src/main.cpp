
#include "../include/Restaurant.h"
#include <iostream>

using namespace std;

Restaurant* backup = nullptr;

int main(int argc, char** argv){
std::string test="#number of tables\n \n6\n#tables description\n6,4,4,14,10,10\n#Menu\nBeer,ALC,50\nSalad,VEG,40";
 Restaurant restaurant(test);

    std::string test2="open 1 Alice,chp Bob,alc";
    restaurant.splitStringComma(test2);


/*
    if(argc!=2){
        std::cout << "usage: rest <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Restaurant rest(configurationFile);
    rest.start();
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }
     */
    return 0;
}