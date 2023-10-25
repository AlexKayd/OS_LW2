#include <vector>
#include <iostream>
#include <thread>

using namespace std;

void dilation(vector<vector<double>>& matrix, int K){

    cout << "ID потока increase.cpp: " << this_thread::get_id() << "\n";

    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int k = 0; k < K; k++){

        vector<vector<double>> new_matrix(rows, vector<double>(cols));

        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){

                double max_value = matrix[i][j];

                for (int ii = -1; ii <= 1; ii++){
                    for (int jj = -1; jj <= 1; jj++){
                        
                        if (i + ii >= 0 && i + ii < rows && j + jj >= 0 && j + jj < cols){
                            max_value = max(max_value, matrix[i + ii][j + jj]);
                        }
                    }
                }
                new_matrix[i][j] = max_value;
            }
        }
        matrix = new_matrix;
    }
}