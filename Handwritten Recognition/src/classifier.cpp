//
// Created by 杨航一 on 3/15/18.
//

#include "classifier.h"
#include "training.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "math.h"


vector<pair<double, double>> getPairFromModel() {
    vector<pair<double, double>> model_pair;
    ifstream input("../data/traininglabels");
    if (!input.is_open()) {
        cout << "file cannot be opened" << endl;
        exit(1);
    } else {
        string pair;
        while (!input.eof()) {
            getline(input, pair);
            double first = stod(pair.substr(0, pair.find(" ")));
            double second = stod(pair.substr(pair.find(" ") + 1));
            model_pair.push_back(make_pair(first, second));
        }
    }
    return model_pair;
};

vector<vector<vector<pair<double, double>>>> loadModelFile() {
    vector<vector<vector<pair<double, double>>>> model = InitializeVector();
    vector<pair<double, double>> pair_from_model = getPairFromModel();
    int p = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 28; j++) {
            for (int k = 0; k < 28, p < 7840; k++, p++) {
                model[j][k][i] = pair_from_model[p];
            }
        }
    }
    return model;
};

