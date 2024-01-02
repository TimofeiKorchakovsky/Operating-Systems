#include <iostream>
#include <vector>
#include <Windows.h>
#include <thread>
#include <algorithm>
CRITICAL_SECTION cs;
void min_max_thread(const std::vector<int>& arr, int& min, int& max) {
    min = arr[0];
    max = 0;
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        if (arr[i] > max)
        {
            max = arr[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(7));
    EnterCriticalSection(&cs);
    std::cout << "Minimal element: " << min << std::endl;
    std::cout << "Maximum element: " << max << std::endl;
    LeaveCriticalSection(&cs);
}

void average_thread(const std::vector<int>& arr, double& avg) {
    int sum = 0;
    for (int num : arr)
    {
        sum += num;
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
    avg = (double)sum / arr.size();
    EnterCriticalSection(&cs);
    std::cout << "Average value: " << avg << std::endl;
    LeaveCriticalSection(&cs);
}

int main() {
    int size;
    std::cout << "Enter the dimension of the array: ";
    std::cin >> size;

    if (size == 0)
    {
        std::cout << "Wrong size";
        return 0;
    }
    std::vector<int> arr(size);
    std::cout << "Enter the array elements: ";
    for (int& num : arr) {
        std::cin >> num;
    }
    if (size == 1)
    {
        std::cout << "Average value: " << arr[0] << std::endl;
        std::cout << "Minimal element: " << arr[0] << std::endl;
        std::cout << "Maximum element: " << arr[0] << std::endl;
        std::cout << "Changed array: " << arr[0];
        return 0;
    }
    InitializeCriticalSection(&cs);
    int max_element;
    int min_element;
    double avg;
    
    std::thread min_max_thread_obj(min_max_thread, arr, std::ref(min_element),std::ref(max_element));
    std::thread average_thread_obj(average_thread, arr,std::ref(avg));

    min_max_thread_obj.join();
    average_thread_obj.join();

    std::replace(arr.begin(), arr.end(), max_element, (int)avg);
    std::replace(arr.begin(), arr.end(), min_element, (int)avg);

    std::cout << "Changed array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    DeleteCriticalSection(&cs);
    return 0;
}
