//
// Created by 杨航一 on 3/15/18.
//

#include "evaluation.h"
#include "classifier.h"
#include "training.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int row_e = 28;
int column_e = 28;
int class_count_e = 10;
int training_total_e = 5000;
int pair_count_e = 7840;
int test_count_e = 1000;

/**
 * this function will load the classified labels
 * @return the classified labels
 */
vector<int> getClassifiedLabels() {
    vector<int> classified_labels(test_count_e);
    ifstream input_classified("../data/classifiedlabels");
    //load the actual file to a vector
    if (!input_classified.is_open()) {
        cout << "file cannot be opened" << endl;
        exit(1);
    } else {
        string classified_label;
        int index = 0;
        while (!input_classified.eof() && index < test_count_e) {
            getline(input_classified, classified_label);
            int label = stoi(classified_label);
            classified_labels[index] = label;
            index++;
        }
    }
    return classified_labels;
}

/**
 * this function will load the test labels
 * @return the test labels
 */
vector<int> getTestLabels() {
    vector<int> test_labels(test_count_e);
    //load the expected file to a vector
    ifstream input_test("../data/testlabels");
    if (!input_test.is_open()) {
        cout << "file cannot be opened" << endl;
        exit(1);
    } else {
        string test_label;
        int index = 0;
        while (!input_test.eof() && index < test_count_e) {
            getline(input_test, test_label);
            int label = stoi(test_label);
            test_labels[index] = label;
            index++;
        }
    }
    return test_labels;
}

/**
 * this function will calculate the whole correct possibility
 * @return the possibility
 */
double correctPossibility() {
    double correct_num = 0.0;
    double correct_possibility;
    vector<int> classified_labels = getClassifiedLabels();
    vector<int> test_labels = getTestLabels();
    for (int i = 0; i < test_count_e; i++) {
        if (classified_labels[i] == test_labels[i]) {
            correct_num++;
        }
    }
        correct_possibility = correct_num / test_count_e;
        return correct_possibility;
    }

/**
 * this function will calculate the possibility for each matrix's row and column
 * @param actual the actual class
 * @param classified the classified class
 * @return the possibility
 */
double classPossibility(int actual, int classified) {
    vector<int> test_labels = getTestLabels();
    vector<int> classified_labels = getClassifiedLabels();
    double class_possibility;
    double classified_num = 0.0;
    double class_num = 0.0;
    for (int i = 0; i < test_count_e; i++) {
        if (test_labels[i] == actual) {
            class_num++;
            if (classified_labels[i] == classified) {
                classified_num++;
            }
        }
    }
    class_possibility = classified_num / class_num;
    return class_possibility;
}

/**
 * this function will form the matrix as a 2d vector
 * @return the matrix
 */
vector<vector<double>> getMatrix() {
    vector<vector<double>> matrix(vector<vector<double>>(class_count_e, vector<double>(class_count_e)));
    for (int i = 0; i < class_count_e; i++) {
        for (int j = 0; j < class_count_e; j++) {
            matrix[i][j] = classPossibility(i, j);
        }
    }
    return matrix;
}

/**
 * this function will record all the prototypical images and print them out
 */
void printPrototypicalImages() {
    vector<vector<vector<int>>> test_images = getTestImages();
    vector<vector<vector<pair<double, double>>>> model = loadModelFile();
    vector<int> classified_labels = getClassifiedLabels();
    vector<vector<int>> images_index(class_count_e);
    vector<vector<double>> images_possibility(class_count_e);
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 10; j++) {
            if (classified_labels[i] == j) {
                images_index[j].push_back(i);
                double possibility = calculatePossibility(test_images, i, j, model);
                images_possibility[j].push_back(possibility);
            }
        }
    }
    vector<vector<int>> print_images(class_count_e, vector<int>(2));
    for (int i = 0; i < class_count_e; i++) {
        vector<double>::iterator biggest = max_element(begin(images_possibility[i]), end(images_possibility[i]));
        int biggest_num = (int)distance(begin(images_possibility[i]), biggest);
        print_images[i][0] = images_index[i][biggest_num];
        vector<double>::iterator smallest = min_element(begin(images_possibility[i]), end(images_possibility[i]));
        int smallest_num = (int)distance(begin(images_possibility[i]), smallest);
        print_images[i][1] = images_index[i][smallest_num];
    }

    for (int i = 0; i < class_count_e; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < row_e; k++) {
                for (int n = 0; n < column_e; n++) {
                    cout << test_images[print_images[i][j]][k][n];
                }
                cout << "\n";
            }
            cout << "\n" << "\n";
        }
    }
}

//int main() {
//    training();
//
//    vector<vector<vector<int>>> test_images = getTestImages();
//
//    vector<vector<vector<pair<double, double>>>> model = loadModelFile();
//    finalClassified(test_images, model);
//
//    double correct_possibility = correctPossibility();
//
//    cout << correct_possibility << "\n";
//
//    vector<vector<double>> matrix = getMatrix();
//
//    for (int i = 0; i < 10; i++) {
//        for (int j = 0; j < 10; j++) {
//            cout << matrix[i][j] << " ";
//        }
//        cout << "\n";
//    }
//
//    printPrototypicalImages();
//    return 0;
//}