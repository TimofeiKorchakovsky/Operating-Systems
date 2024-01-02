#include <iostream>
#include <string>
#include <windows.h>
#include <tchar.h>
#include<vector>
#include<thread>
#include <mutex>
#include <exception>
using namespace std;
int* arr;
int arraySize;
vector<bool> arrOfActives;
int amountOfThreads;
int AoT;
condition_variable condition;
mutex mt;
int numbToEnd = -1;
bool consistInVector(vector<int> vec, int a) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == a) {
            return true;
        }
    }
    return false;
}
bool checkAllThreadsStop() {
    for (int i = 0; i < AoT; i++) {
        if (arrOfActives[i] == true) {
            return false;
        }
    }
    return true;
}
void Marker(int x) {
    srand(x);
    vector<int> marked;
    while (true) {
        unique_lock<mutex> ul(mt);
        if (numbToEnd == x) {
            arrOfActives[x] = false;
            for (int i = 0; i < marked.size(); i++) {
                arr[marked[i]] = 0;
            }
            amountOfThreads--;
            condition.notify_all();
            ul.unlock();
            return;
        }


        int ch = rand();
        ch %= arraySize;


        if (arr[ch] == 0) {
            Sleep(5);
            arr[ch] = x + 1;
            if (!consistInVector(marked, ch)) {
                marked.push_back(ch);
            }
            Sleep(5);
        }
        else {
            arrOfActives[x] = false;
            cout << "Marker №" << x + 1 << '\n';
            cout << "Количество помеченых элементы: ";
            cout << marked.size();
            cout << '\n';
            condition.notify_all();
            condition.wait(ul, [=]() {return arrOfActives[x] == true; });
        }
        ul.unlock();
    }
}


int main(void) {

    setlocale(LC_ALL, "rus");
    cout << "Введите размер массива: ";
    cin >> arraySize;
    cout << '\n';
    arr = new int[arraySize];
    for (int i = 0; i < arraySize; i++) {
        arr[i] = 0;
    }
    cout << "Введите количество потоков maker: ";
    cin >> amountOfThreads;
    cout << '\n';
    AoT = amountOfThreads;

    vector<thread> Threads;
    for (int i = 0; i < amountOfThreads; i++) {
        Threads.push_back(thread(&Marker, i));
        arrOfActives.push_back(true);
    }
    for (int i = 0; i < amountOfThreads; i++) {
        Threads[i].detach();

    }
    vector<int> endedThreads;
    while (amountOfThreads > 0) {
        unique_lock<mutex> ul(mt);
        condition.wait(ul, [=]() {return checkAllThreadsStop(); });

        cout << "arr: ";
        for (int i = 0; i < arraySize; i++) {
            cout << arr[i] << " ";
        }
        cout << endl << "Номер потока для завершения: " << endl;
        int closingThread;
        cin >> closingThread;
        closingThread--;
        endedThreads.push_back(closingThread);
        arrOfActives[closingThread] = true;
        numbToEnd = closingThread;
        condition.notify_all();
        condition.wait(ul, [=]() {return checkAllThreadsStop(); });
        for (int i = 0; i < AoT; i++) {
            if (!consistInVector(endedThreads, i)) {
                arrOfActives[i] = true;
            }
        }
        condition.notify_all();
        ul.unlock();

    }


    return 0;
}