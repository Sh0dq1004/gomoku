#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>

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
    public:
        Player(int f_or_s): first_or_second{f_or_s}{}
        void random_action(Board& b) const{
            size_t x{static_cast<size_t>(rand()%15)}, y{static_cast<size_t>(rand()%15)};
            if (rule_check(b,x,y)==0) random_action(b);
            else if (rule_check(b,x,y)==1) b.action(x,y);
            else{
                b.action(x,y);
                b.game_end_set();
                std::cout << "PLAYER(" << first_or_second << ") WIN!!\n";
            }
        }

        int rule_check(Board b, size_t x, size_t y) const{
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