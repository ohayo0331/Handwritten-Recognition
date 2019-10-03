//
// Created by 杨航一 on 3/11/18.
//

#include "training.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<vector<vector<pair<double, double>>>> InitializeVector() {
    //initialized the 3d vector
    vector<vector<vector<pair<double, double>>>> image;
    for (int i = 0; i < 28; ++i) {
        vector<vector<pair<double, double>>> row;
        for (int j = 0; j < 28; ++j) {
            vector<pair<double, double>> column;
            for (int k = 0; k < 10; ++k) {
                pair<double, double> feature = make_pair(0.0, 0.0);
                column.push_back(feature);
            }
            row.push_back(column);
        }
        image.push_back(row);
    }
    return image;
}

vector<int> getLabels() {
    vector<int> training_labels;
    ifstream input("../data/traininglabels");
    if (!input.is_open()) {
        cout << "file cannot be opened" << endl;
        exit(1);
    } else {
        char label[1];
        while (!input.eof()) {
            input.getline(label, 2);
            training_labels.push_back(label[0] - '0');
        }
    }
    return training_labels;
}

vector<int> getTotalOccurrence() {
    vector<int> total_occurrence(10);
    vector<int> labels = getLabels();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < labels.size(); j++) {
            if (i == labels[j]) {
                total_occurrence[i]++;
            }
        }
    }
    return total_occurrence;
}

vector<vector<vector<pair<double, double>>>> calculateP(
        vector<vector<vector<pair<double, double>>>> possibility_data, vector<vector<vector<int>>> images) {
    vector<int> total_occurrence = getTotalOccurrence();
    vector<int> labels = getLabels();
    for (int i = 0; i < 5000; i++) {
        for (int j = 0; j < 28; j++) {
            for (int k = 0; k < 28; k++) {
                if (images[i][j][k] == 0) {
                    possibility_data[j][k][labels[i]].first++;
                } else {
                    possibility_data[j][k][labels[i]].second++;
                }
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 28; j++) {
            for (int k = 0; k < 28; k++) {
                    possibility_data[j][k][i].first
                            = (0.1 + possibility_data[j][k][i].first) / (0.2 + total_occurrence[i]);
                    possibility_data[j][k][i].second
                            = (0.1 + possibility_data[j][k][i].second) / (0.2 + total_occurrence[i]);
            }
        }
    }

    return possibility_data;
}

vector<vector<vector<int>>> getTrainingImages() {
    vector<vector<vector<int>>> images(5000, vector<vector<int>>(28, vector<int>(28)));
    vector<int> labels = getLabels();
    ifstream input("../data/trainingimages");
    if (!input.is_open()) {
        cout << "file cannot be opened" << endl;
    } else {
            for (int i = 0; i < 5000; i++) {
                for (int j = 0; j < 28; j++) {
                    char images_line[28];
                    input.getline(images_line, 30);
                    for (int k = 0; k < 28; k++) {
                        if (images_line[k] == ' ') {
                            images[i][j][k] = 0;
                        } else {
                            images[i][j][k] = 1;
                        }
                    }
                }
            }

     }
    return images;
}

int main () {
    vector<vector<vector<int>>> images = getTrainingImages();
    vector<vector<vector<pair<double, double>>>> possibility_data = InitializeVector();
    vector<int> occurrence = getTotalOccurrence();
    vector<vector<vector<pair<double, double>>>> possibility_output = calculateP(possibility_data, images);
    ofstream output("../data/trainingoutput");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 28; j++) {
            for (int k = 0; k < 28; k++) {
                output << possibility_output[j][k][i].first << " ";
                output << possibility_output[j][k][i].second << " ";
                output << "\n";
            }
        }
    }
    return 0;
};