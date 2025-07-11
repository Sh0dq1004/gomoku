#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>

using std::string, std::vector, std::exp;

class Weight{
    string name;
    vector<vector<double>> vec;
    public:
    Weight(std::string p):name(p){ load(); }

    Weight(std::string p,int h,int w):name(p){
        // Using He initializer
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        std::normal_distribution<> dist(0.0, 1.0);
        double c{sqrt(w)/sqrt(2)};
        for (int i=0; i<h; i++){
            vector<double> v;
            for (int j=0; j<w; j++) v.push_back(dist(engine)/c);
            vec.push_back(v);
        }
    }

    Weight(int h, int w, double val=0.0){
        vec = vector<vector<double>> (h, vector<double>(w, val));
    }

    void load(){
        std::ifstream f("weights/"+name+".txt");
        for (string line; std::getline(f, line);){
            std::stringstream ss; ss << line;
            std::vector<double> v;
            for (string e; ss >> e;) v.push_back(std::stod(e));
            vec.push_back(v);
        }
    }

    void save(){
        std::ofstream f("weights/"+name+".txt");
        for (const std::vector<double>& v : vec){
            for (const double& e:v) f << e << ' ';
            f<<"\n";
        }
    }

    vector<double>& operator[](int n) { return vec[n]; }
    vector<double> operator[](int n) const { return vec[n]; }

    int height() const{ return vec.size(); }
    int weight() const{ return vec[0].size(); }

    template<typename T>
    vector<vector<double>> operator*(const vector<vector<T>>& v) const{
        vector<vector<double>> result(height(),vector<double>(v[0].size(),0));
        for (int i=0; i < height(); i++) for (int j=0; j < v[0].size(); j++) for (int k=0; k < weight(); k++) result[i][j] += vec[i][k]*v[k][j];
        return result;
    }


    template<typename T>
    std::pair<vector<double>,vector<vector<double>>> operator*(const vector<T>& v) const{
        vector<double> result(height(),0);
        for (int i=0; i < height(); i++) for(int j=0; j < weight(); j++) result[i] += vec[i][j]*v[j];
        vector<vector<double>> diff(height(), vector<double>(weight()));
        for (int i=0; i < height(); i++) for (int j=0; j < weight(); j++) diff[i][j]=v[j];
        return {result, diff};
    }

    friend std::ostream& operator<< (std::ostream& os, const Weight& w){
        for (int i=0; i < w.height(); i++){
            for (int j=0; j < w.weight(); j++) os << w[i][j] << " ";
            os << std::endl;
        }
        return os;
    }
};

std::pair<vector<double>,vector<double>> relu(vector<double> vec){
    vector<double> result(vec.size());
    vector<double> diff(vec.size());
    for (int i=0; i < vec.size(); i++){
        if (vec[i] >= 0) {
            result[i]=vec[i];
            diff[i]=1;
        }else {
            result[i]=0;
            diff[i]=0;
        }
    }
    return {result, diff};
}

template<typename T>
std::pair<vector<T>,vector<T>> softmax(const vector<T>& vec){
    vector<T> exp_vec;
    T sum{};
    for (T x : vec){
        T e{exp(x)};
        sum += e;
        exp_vec.push_back(e);
    }


    vector<T> result;
    vector<T> diff;
    for (int i = 0; i < vec.size(); i++){
        result.push_back(exp_vec[i] / sum);
        diff.push_back(exp_vec[i] * (sum - exp_vec[i]) / (sum * sum));
    }
    return {result, diff};
}



template <typename T>
void addGap(vector<T>& vec){
    vec.push_back(static_cast<T>(1));
}

std::ostream& operator<< (std::ostream& os, const vector<double>& v){
    for (double e : v) os << e << " ";
    os << std::endl;
    return os;
}

std::ostream& operator<< (std::ostream& os, const vector<vector<double>>& v){
    for (const vector<double>& e : v) os << e;
    return os;
}
