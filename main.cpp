#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// генерация случайного числа от min до max
int GetRandomInt(int min, int max)
{
    // инициализация генератора (по примеру с лекции)
    auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937 RandomGenerator(seed);
    uniform_int_distribution<int> dist(min, max);
    return dist(RandomGenerator);
}

// линейный поиск - просто пробегаем по всем элементам
int LinearSearch(const vector<int>& arr, int value)
{
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] == value)
        {
            return i; // если совпали(нашли), то вернули индекс
        }
    }
    return -1; // не нашли
}

//место для сортировок

int main()
{
    const int N = 100000;  // размер массива
    const int M = 10000;   // сколько раз ищем

    // 1 задача: сортировка + линейный поиск
    cout << "1 step: sorting + linear search" << endl;

    vector<int> sortedArray;
    sortedArray.reserve(N);

    // создаём массив
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        sortedArray.push_back(GetRandomInt(0, 1000000));
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Creating massiv: " << duration.count() << " ms" << endl;


    //место для вызова сортировки

    cout << "Sortirovka: " << duration.count() << " ms" << endl;


    // ищем элементы линейным поиском
    start = chrono::high_resolution_clock::now();

    int foundCount = 0;
    for (int i = 0; i < M; i++)
    {
        int searchValue = GetRandomInt(0, 1000000);
        int index = LinearSearch(sortedArray, searchValue);
        if (index != -1)
        {
            foundCount++;
        }
    }

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Linear search (" << M << " times): " << duration.count() << " ms" << endl;
    cout << "Found: " << foundCount << " elements" << endl;

    return 0;
}