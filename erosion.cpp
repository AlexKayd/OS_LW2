#include <vector>
#include <iostream>
#include <thread>

using namespace std;

void erosion(vector<vector<double>>& matrix, int K){

    cout << "ID потока erosion.cpp: " << this_thread::get_id() << "\n";
    
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int k = 0; k < K; k++){

        vector<vector<double>> new_matrix(rows, vector<double>(cols));

        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){

                double min_value = matrix[i][j];

                for (int ii = -1; ii <= 1; ii++){
                    for (int jj = -1; jj <= 1; jj++){

                        if (i + ii >= 0 && i + ii < rows && j + jj >= 0 && j + jj < cols){
                            min_value = min(min_value, matrix[i + ii][j + jj]);
                        }
                    }
                }
                new_matrix[i][j] = min_value;
            }
        }
        matrix = new_matrix;
    }
}