#include <iostream>
#include "my_vector.h"
#include <vector>
#include <fcntl.h>
#include <cassert>
#include <algorithm>

int main() {
    my_vector<int> arr1;
    my_vector<int> arr2 = {1, 2, 3, 4, 5};
    my_vector<int> arr3{6, 7, 8, 9, 10};
    my_vector<int> arr4(5, 10);
    my_vector<int> arr5 = arr4;
    my_vector<int> arr6;
    arr6 = {1, 2, 3};
    std::vector<int> v2(1, 5);
    std::vector<int> v1{1, 2, 3};
    std::vector<int> v = std::vector<int>();
    v1.erase(v1.begin(), v1.end());
    int i = 100;
    arr2.insert(arr2.begin() + 1, {4, 5, 6, 7, 8});
    for (auto it = arr2.begin(); it != arr2.end(); it++) {
        printf("%d ", *it);
    }
    printf("\n");
    assert(arr2.erase(arr2.begin(), arr2.end()) == arr2.end());
    for (auto it = arr2.begin(); it != arr2.end(); it++) {
        printf("%d ", *it);
    }
    printf("\n");
    printf("%ld, %ld\n", arr2.size(), arr2.capacity());

    my_vector<int> arr7;
    assert(arr7.begin() == arr7.end());
    return 0;
}
