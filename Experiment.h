//
// Created by Shewa on 06/12/2019.
//

#ifndef LAB_02_EXPERIMENT_H
#define LAB_02_EXPERIMENT_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <chrono>
#include <cstdlib>
#include <fstream>

class Note {
public:
    std::string method;
    int number;
    int buffer_size;
    int result;
    Note (std::string method, int number, int buffer_size, int result);
    void print(std::ofstream &out);
};

class Experiment {
private:
    const int loop_amount = 1000;
    int *buffer = nullptr;
    std::vector<int> buffer_sizes;
    size_t Amount = 0;
    std::vector<Note> Notes;
public:
    Experiment(int min, int max);
    void Calculation();
    void Init();
    void WarmUp();
    void Forward();
    void Backward();
    void Random();
    void print(std::ofstream &out);
    void GraphBuild();
};


#endif //LAB_02_EXPERIMENT_H
