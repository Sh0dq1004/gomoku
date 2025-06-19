#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>

class Weight{
    std::string name;
    int height,width;
    double* vecs;
    public:
    Weight(std::string path,int w,int h): name(path),height(h),width(w){
        vecs=new double[h][w];
        if (!load()){
            std::random_device seed_gen;
            std::default_random_engine engine(seed_gen());
            std::normal_distribution<> dist(0.0, 1.0);
            double c{sqrt(w)/sqrt(2)};
            for (int i=0; i<height;i++){
                for (int j=0; j<width;j++) vecs[i][j]=dist(engine)/c;
            }
        }
    }

    bool load(){
        std::ifstream f("weights/"+name+".txt");
        for (int i{0}; i<height; i++){
            std::string line; std::getline(f,line); double v[width];
            std::stringstream ss; ss << line;
            for (int j{0}; j<width; j++){
                std::string e; ss>>e; v[j]=std::stod(e);
            }
            vecs[i]=v;
        }
        return std::size(vecs)!=0
    }

    void save(){
        std::ofstream f("weights/"+name+".txt");
        for (const std::vector<double>& v:vecs){
            for (const double& e:v) f << e << ' ';
            f<<"\n";
        }
    }
    
    /*
    std::vector<std::vector<double>> operator*(const std::vector<std::vector<double>>& v) const{
        std::vector<std::vector<double>> result;
        for (int i=0;i<height;i++){
            double x;
            for (int j=0;j<width;j++) x+=vecs[i][j]*v[j][0];
            result.push_back(std::vector<double>{x});
        }
        return result;
    }

    std::vector<std::vector<double>> operator*(const Weight& w) const{
        std::vector<std::vector<double>> result;
        for (int i=0;i<height;i++){
            double x;
            for (int j=0;j<width;j++) x+=vecs[i][j]*v[j][0];
            result.push_back(std::vector<double>{x});
        }
        return result;
    }
    */
};