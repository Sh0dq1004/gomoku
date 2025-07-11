#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "weight.hpp"

class Board{
    int map[15][15]{};
    int turn{1};
    bool game_end{false};
    public:
        void action(size_t x, size_t y){
            if (turn%2==1) map[y][x]=1;
            else map[y][x]=2;
            turn++;
        }

        int get_map_val(size_t y, size_t x) const{ return map[y][x]; }
        bool game_end_get()const{ return game_end;}
        void game_end_set(){ game_end = true;}
        std::vector<int> map2vec() const{
            std::vector<int> vec;
            for (int i = 0; i < 15; i++) for (int j=0; j < 15; j++) vec.push_back(map[i][j]);
            return vec;
        }

        friend std::ostream& operator<<(std::ostream& out, const Board& b){
            for (int i{0}; i < 15; i++){
                for (int j{0}; j < 15; j++) out << b.map[i][j];
                out << "\n";
            }
            return out;
        }
};

class Player{
    int first_or_second;
    Weight w1, w2, w3;

    public:
        Player(int f_or_s):
            first_or_second{f_or_s},
            w1("weight1_"+std::to_string(f_or_s), 300, 226),
            w2("weight2_"+std::to_string(f_or_s), 300, 301),
            w3("weight3_"+std::to_string(f_or_s), 225, 301){}
        
        void random_action(Board& b){
            size_t x{static_cast<size_t>(rand()%15)}, y{static_cast<size_t>(rand()%15)};
            if (rule_check(b,x,y)==0) random_action(b);
            else if (rule_check(b,x,y)==1) b.action(x,y);
            else{
                b.action(x,y);
                b.game_end_set();
                std::cout << "PLAYER(" << first_or_second << ") WIN!!\n";
            }
        }

        void neural_net_action(Board& b){
            std::vector<int> input{b.map2vec()};

            addGap(input);
            auto [u1, d_weight_1] = w1 * input;
            auto [h1, d_relu_1] = relu(u1);

            addGap(h1);
            auto [u2, d_weight_2] = w2 * h1;
            auto [h2, d_relu_2] = relu(u2);

            addGap(h2);
            auto [u3, d_weight_3] = w3 * h2;
            auto [output, d_softmax] = softmax(u3);

            //outputをmapni直して最も大きいところを次の一手として考えさせる。
        }

        int rule_check(const Board& b, size_t x, size_t y) const{
            if (first_or_second==1){
                int san_counter{0},yon_counter{0},naga_counter{0};
                for (int yc{-1};yc<2;yc++){
                    if (y+yc<0 || 14<y+yc) continue;
                    for (int xc{-1};xc<2;xc++){
                        if (x+xc<0 || 14<x+xc) continue;
                        bool space_exist{false};
                        int ren_counter{1};
                        for (size_t d{1};d<=5;d++){
                            if (x+xc*d<0 || 14<x+xc*d || y+yc*d<0 || 14<y+yc*d) break;
                            if (b.get_map_val(y+yc*d,x+xc*d)==1){
                                ren_counter++;
                            } else if (b.get_map_val(y+yc*d,x+xc*d)==0){
                                if (space_exist) break;
                                else space_exist=true;
                            } else break;
                        }
                        if (ren_counter==3){
                            san_counter++;
                            if (san_counter>=2) return 0;
                        }else if (ren_counter==4){
                            yon_counter++;
                            if (yon_counter>=2) return 0;
                        }else if (ren_counter==5 && !space_exist) return 2;
                        else if (ren_counter>4){
                            naga_counter++;
                            if (ren_counter>=2) return 0;
                        }
                        
                    }
                }
            }
            return 1;
        }
};
