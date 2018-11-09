
#include "../include/Restaurant.h"
#include <iostream>

using namespace std;

Restaurant* backup = nullptr;

int main(int argc, char** argv){

    if(argc!=2){
        std::cout << "usage: rest <config_path>" << std::endl;
        return 0;
    }
    //string configurationFile = argv[1];
    string configurationFile="#Config file 2\n#number of tables\n6\n#tables description\n6,4,4,14,10,10\n#Menu\nButterbeer,ALC,50\nFire Whisky,ALC,60\nHoney Wine,ALC,80\nWater,BVG,20\nPumpkin Juice,BVG,40\nPumpkin Cake,VEG,50\nSpicy meat,SPC,300";
    Restaurant rest(configurationFile);
    rest.start();
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }

    return 0;
}