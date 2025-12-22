//
// Created by skill on 11/15/2025.
//
#include <iostream>
#include "client/client.hpp"
int main()
    {
    while (true) {
        client c = client("localhost", 8080);
    }
    std::cout << "Hello World!\n";
    return 0;


  }