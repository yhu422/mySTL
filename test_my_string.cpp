//
// Created by Yile Hu on 9/19/24.
//

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <cassert>
#include <algorithm>
#include "my_string.h"
#include "my_vector.h"
int main() {
    my_string s(5,'a');
    assert(s[5]=='\0');
    std::string str;
    std::cout << str.capacity() << std::endl;
    std::cout << str.size() << std::endl;
    printf("%p\n", str.data());
    std::cout << sizeof(str) << std::endl;
    std::vector<int> v;
    my_vector<int> v2;
    v.reserve(50);
    v2.reserve(50);
    printf("%ld %ld\n",v.capacity(), v2.capacity());
    v.reserve(70);
    v2.reserve(70);
    printf("%ld %ld\n",v.capacity(), v2.capacity());
    v.push_back(10);
}

