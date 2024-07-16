#include <iostream>
#include <ctime>

// using namespace std;

std::ostream &Log(){
    std::cout << "For Debug | timestamp: " << (long long )time(nullptr) << " | "; 
    return std::cout;
}