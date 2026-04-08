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

// Бинарный поиск (пункт 4), шаблонная версия
template<typename T>
int BinarySearch(const vector<T>& arr, const T& value)
{
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == value)
            return mid;
        else if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

/*  Быстрая сортировка (Quick Sort), шаблонная функция (по пункту 3)
    Операции, необходимые для сортировки:
    1. Сравнение (operator< (arr[i] < pivot)) — определяет порядок элементов
    2. Обмен (swap(arr[i], arr[j])) — перемещает элементы на нужные позиции
    */
template<typename T>
void QuickSort(vector<T>& arr, int left, int right)
{
    if (left >= right) return;
    T pivot = arr[(left + right) / 2];
    int i = left, j = right;
    while (i <= j)
    {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j)
        {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    QuickSort(arr, left, j);
    QuickSort(arr, i, right);
}

// Insertion Sort
template <typename T>
void InsertionSort(vector<T>& arr)
{
    // Операции: сравнение (operator>) и присваивание для сдвига элементов
    for (size_t i = 1; i < arr.size(); ++i)
    {
        T key = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && arr[j] > key)  // сравнение
        {
            arr[j + 1] = arr[j];  // сдвиг вправо (присваивание)
            --j;
        }
        arr[j + 1] = key;  // вставка на место (присваивание)
    }
}


// слияние двух отсортированных частей (для Merge Sort)
template <typename T>
void Merge(vector<T>& arr, int left, int mid, int right)
{
    vector<T> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<T> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    // Сравнение элементов и копирование меньшего
    while (i < leftArr.size() && j < rightArr.size())
    {
        if (leftArr[i] <= rightArr[j])  // сравнение
            arr[k++] = leftArr[i++];     // копирование
        else
            arr[k++] = rightArr[j++];
    }
    // Копирование остатков
    while (i < leftArr.size()) arr[k++] = leftArr[i++];
    while (j < rightArr.size()) arr[k++] = rightArr[j++];
}

template <typename T>
void MergeSortRecursive(vector<T>& arr, int left, int right)
{
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    MergeSortRecursive(arr, left, mid);
    MergeSortRecursive(arr, mid + 1, right);
    Merge(arr, left, mid, right);  // слияние с операциями сравнения и копирования
}

// Merge Sort
template <typename T>
void MergeSort(vector<T>& arr)
{
    if (!arr.empty())
        MergeSortRecursive(arr, 0, static_cast<int>(arr.size()) - 1);
}

template<typename T>
void bubbleSort(vector<T>& arr) {
    size_t n = arr.size();
    if (n <= 1) return;
    
    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        // Last i elements are already in place
        for (size_t j = 0; j < n - i - 1; ++j) {
            // Requires operator> for comparison
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no swaps occurred - array is sorted
        if (!swapped) break;
    }
}


template<typename T>
void selectionSort(vector<T>& arr) {
    size_t n = arr.size();
    if (n <= 1) return;
    
    for (size_t i = 0; i < n - 1; ++i) {
        size_t minIndex = i;
        // Find minimum element in unsorted part
        for (size_t j = i + 1; j < n; ++j) {
            // Requires operator< for comparison
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap if a smaller element was found
        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
        }
    }
}

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
