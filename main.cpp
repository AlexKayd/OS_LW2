#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

// Фильтры эрозии и наращивания
void erosion(vector<vector<double>>& matrix, int K);
void dilation(vector<vector<double>>& matrix, int K);

// Функция печати итоговых матриц
void print(const vector<vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}



int main(int argc, char* argv[]) {
    
    cout << "Максимально возможное количество потоков: " << thread::hardware_concurrency() << "\n";
    
    if ( atoi(argv[1]) > 12 || atoi(argv[1]) <= 0){
        cout << "Заданное число потоков не может быть использовано." << "\n";
        return 1;
    }

    const int Max = atoi(argv[1]);
    int rows, cols;
    string filename;
    int K;
    
    cout << "Введите имя файла: ";
    cin >> filename;
    cout << "Введите количество раз для наложения фильтров: ";
    cin >> K;
    cout << "\n";

// Начало отсчета времени
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

    ifstream input(filename);
    if (!input.is_open()) {
        cout << "Ошибка открытия файла. " << "\n";
        return 1;
    }
    
    input >> rows >> cols;
    vector<vector<double>> matrix(rows, vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            input >> matrix[i][j];
        }
    }

    input.close();

    vector<vector<double>> erosion_matrix = matrix;
    vector<vector<double>> increase_matrix = matrix;

    cout << "ID потока main.cpp: " << this_thread::get_id() << "\n";

// Работа единственного потока
    if (Max == 1){
        erosion(erosion_matrix, K);
        dilation(increase_matrix, K);
    }

// Работа двух потоков одновременно
    if (Max == 2){
        thread erosion_thread(erosion, ref(erosion_matrix), K);
        dilation(increase_matrix, K);

        erosion_thread.join();
    }

// Работа трех потоков оновременно
    if (Max > 2){
        thread erosion_thread(erosion, ref(erosion_matrix), K);
        thread increase_thread(dilation, ref(increase_matrix), K);

        erosion_thread.join();
        increase_thread.join();
    }

    cout << "Матрица после наложения " << K << " раз фильтра эрозии: " << "\n";
    print(erosion_matrix);

    cout << "Матрица после наложения " << K << " раз фильтра наращивания: " << "\n";
    print(increase_matrix);

// Конец отсчета времени
    chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
    chrono::microseconds duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    
    cout << "Время работы программы: " << duration.count() << " микросекунд. " << "\n";

    return 0;
}