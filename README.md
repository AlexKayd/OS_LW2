# Лабораторная работа №2
## Кайдалова Александра, М8О-212Б-22
## Вариант 12

#### Цель работы
Приобретение практических навыков в:
    1. Управление потоками в ОС;
    2. Обеспечение синхронизации между потоками.

#### Задание
Составить программу на языке Си(C++), обрабатывающую данные в многопоточном режиме. При  обработки использовать стандартные средства создания потоков операционной системы  (Windows/Unix). Ограничение максимального количества потоков, работающих в один момент  времени, должно быть задано ключом запуска вашей программы. Так же необходимо уметь продемонстрировать количество потоков, используемое вашей  программой с помощью стандартных средств операционной системы. В отчете привести исследование зависимости ускорения и эффективности алгоритма от входных  данных и количества потоков. Получившиеся результаты необходимо объяснить.

#### Вариант 12
Наложить K раз фильтры эрозии и наращивания на матрицу состоящую из вещественных чисел. На выходе получается 2 результирующие матрицы.

#### Main.cpp
```cpp
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
```

#### erosion.cpp
```cpp
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
```

#### increase.cpp
```cpp
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
```