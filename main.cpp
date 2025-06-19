#include <iostream>
#include <fstream>
#include "gameset.hpp"
#include "weight.hpp"

void random_test(){
    Board b;
    Player p1{1},p2{2};
    int count{1};
    while (!b.game_end_get()){
        if (count%2==1) p1.random_action(b);
        else p2.random_action(b);
        std::ofstream f("game_map/1/"+std::to_string(count)+".txt");
        if (f.is_open()){
            f<<b;f.close();
        }else{
            std::cout<<"file error\n";
        }
        count++;
    }
}


int main(){
    random_test();
}