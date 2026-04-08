#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// Генерация случайного числа в диапазоне [min, max]
int GetRandomInt(int min, int max)
{
    auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937 RandomGenerator(seed);
    uniform_int_distribution<int> dist(min, max);
    return dist(RandomGenerator);
}

// Линейный поиск: возвращает индекс или -1, если не найдено
int LinearSearch(const vector<int>& arr, int value)
{
    for (size_t i = 0; i < arr.size(); i++)
        if (arr[i] == value) return static_cast<int>(i);
    return -1;
}

// ============================================================================
// 1. INSERTION SORT (твоя реализация)
// Операции: сравнение (operator>) и присваивание (=) для сдвига и вставки
// ============================================================================
template <typename T>
void InsertionSort(vector<T>& arr)
{
    for (size_t i = 1; i < arr.size(); ++i)
    {
        T key = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// ============================================================================
// 2. MERGE SORT (твоя реализация)
// Операции: сравнение (operator<=) и присваивание (=) при слиянии частей
// ============================================================================
template <typename T>
void Merge(vector<T>& arr, int left, int mid, int right)
{
    vector<T> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<T> R(arr.begin() + mid + 1, arr.begin() + right + 1);
    
    int i = 0, j = 0, k = left;
    while (i < (int)L.size() && j < (int)R.size())
    {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < (int)L.size()) arr[k++] = L[i++];
    while (j < (int)R.size()) arr[k++] = R[j++];
}

template <typename T>
void MergeSortRecursive(vector<T>& arr, int left, int right)
{
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    MergeSortRecursive(arr, left, mid);
    MergeSortRecursive(arr, mid + 1, right);
    Merge(arr, left, mid, right);
}

template <typename T>
void MergeSort(vector<T>& arr)
{
    if (!arr.empty())
        MergeSortRecursive(arr, 0, static_cast<int>(arr.size()) - 1);
}

// ============================================================================
// 3. QUICK SORT
// Операции: сравнение (operator<, operator>) и обмен (swap) элементов
// ============================================================================
template <typename T>
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

// ============================================================================
// 4. BUBBLE SORT
// Операции: сравнение (operator>) и обмен (swap) соседних элементов
// ============================================================================
template <typename T>
void BubbleSort(vector<T>& arr)
{
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i)
    {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// ============================================================================
// 5. SELECTION SORT
// Операции: сравнение (operator<) для поиска минимума и обмен (swap)
// ============================================================================
template <typename T>
void SelectionSort(vector<T>& arr)
{
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i)
    {
        size_t minIdx = i;
        for (size_t j = i + 1; j < n; ++j)
        {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        if (minIdx != i)
            swap(arr[i], arr[minIdx]);
    }
}

// ============================================================================
// 6. STD::SORT (обёртка для единообразия вызова)
// Использует интроспективную сортировку (гибрид Quick+Heap+Insertion)
// ============================================================================
template <typename T>
void StdSort(vector<T>& arr)
{
    sort(arr.begin(), arr.end());
}

// ============================================================================
// MAIN: тест всех 6 сортировок с замером времени
// ============================================================================
int main()
{
    const int N = 30000;
    const int M = 10000;

    cout << "=== Sorting Algorithms Benchmark ===" << endl << endl;

    // Вспомогательная лямбда для замера времени
    auto measure = [](auto func) -> long long {
        auto start = chrono::high_resolution_clock::now();
        func();
        auto stop = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    };

    // 1. Insertion Sort
    cout << "1. Insertion Sort" << endl;
    {
        vector<int> arr(N);
        for (int& x : arr) x = GetRandomInt(0, 1000000);
        long long t = measure([&]() { InsertionSort(arr); });
        cout << "Sorting: " << t << " ms" << endl;
        
        int found = 0;
        for (int i = 0; i < M; i++)
            if (LinearSearch(arr, GetRandomInt(0, 1000000)) != -1) found++;
        cout << "Linear search (" << M << " times): " << measure([&]() {
            for (int i = 0; i < M; i++) LinearSearch(arr, GetRandomInt(0, 1000000));
        }) << " ms" << endl;
        cout << "Found: " << found << " elements" << endl << endl;
    }

    // 2. Merge Sort
    cout << "2. Merge Sort" << endl;
    {
        vector<int> arr(N);
        for (int& x : arr) x = GetRandomInt(0, 1000000);
        long long t = measure([&]() { MergeSort(arr); });
        cout << "Sorting: " << t << " ms" << endl;
        
        int found = 0;
        for (int i = 0; i < M; i++)
            if (LinearSearch(arr, GetRandomInt(0, 1000000)) != -1) found++;
        cout << "Linear search (" << M << " times): " << measure([&]() {
            for (int i = 0; i < M; i++) LinearSearch(arr, GetRandomInt(0, 1000000));
        }) << " ms" << endl;
        cout << "Found: " << found << " elements" << endl << endl;
    }

    // 3. Quick Sort
    cout << "3. Quick Sort" << endl;
    {
        vector<int> arr(N);
        for (int& x : arr) x = GetRandomInt(0, 1000000);
        long long t = measure([&]() { QuickSort(arr, 0, (int)arr.size() - 1); });
        cout << "Sorting: " << t << " ms" << endl;
        
        int found = 0;
        for (int i = 0; i < M; i++)
            if (LinearSearch(arr, GetRandomInt(0, 1000000)) != -1) found++;
        cout << "Linear search (" << M << " times): " << measure([&]() {
            for (int i = 0; i < M; i++) LinearSearch(arr, GetRandomInt(0, 1000000));
        }) << " ms" << endl;
        cout << "Found: " << found << " elements" << endl << endl;
    }

    // 4. Bubble Sort
    cout << "4. Bubble Sort" << endl;
    {
        vector<int> arr(N);
        for (int& x : arr) x = GetRandomInt(0, 1000000);
        long long t = measure([&]() { BubbleSort(arr); });
        cout << "Sorting: " << t << " ms" << endl;
        
        int found = 0;
        for (int i = 0; i < M; i++)
            if (LinearSearch(arr, GetRandomInt(0, 1000000)) != -1) found++;
        cout << "Linear search (" << M << " times): " << measure([&]() {
            for (int i = 0; i < M; i++) LinearSearch(arr, GetRandomInt(0, 1000000));
        }) << " ms" << endl;
        cout << "Found: " << found << " elements" << endl << endl;
    }

    // 5. Selection Sort
    cout << "5. Selection Sort" << endl;
    {
        vector<int> arr(N);
        for (int& x : arr) x = GetRandomInt(0, 1000000);
        long long t = measure([&]() { SelectionSort(arr); });
        cout << "Sorting: " << t << " ms" << endl;
        
        int found = 0;
        for (int i = 0; i < M; i++)
            if (LinearSearch(arr, GetRandomInt(0, 1000000)) != -1) found++;
        cout << "Linear search (" << M << " times): " << measure([&]() {
            for (int i = 0; i < M; i++) LinearSearch(arr, GetRandomInt(0, 1000000));
        }) << " ms" << endl;
        cout << "Found: " << found << " elements" << endl << endl;
    }

    // 6. std::sort (эталон)
    cout << "6. std::sort (reference)" << endl;
    {
        vector<int> arr(N);
        for (int& x : arr) x = GetRandomInt(0, 1000000);
        long long t = measure([&]() { StdSort(arr); });
        cout << "Sorting: " << t << " ms" << endl;
        
        int found = 0;
        for (int i = 0; i < M; i++)
            if (LinearSearch(arr, GetRandomInt(0, 1000000)) != -1) found++;
        cout << "Linear search (" << M << " times): " << measure([&]() {
            for (int i = 0; i < M; i++) LinearSearch(arr, GetRandomInt(0, 1000000));
        }) << " ms" << endl;
        cout << "Found: " << found << " elements" << endl;
    }

    return 0;
}
