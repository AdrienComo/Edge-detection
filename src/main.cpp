#include <chrono>
#include <string>
#include <iostream>

#include "Image.h"

// Number of default parameters + 1
const int PARAMETERS = 8;

// Default parameters
std::string input   = "../img/lena.jpg";
int blur    = 5;
int kernel   = 2;
int t = 100;
int direction = 1;
int lowValue = 40;
int highValue = 90;

void getElements(int argc, char** argv){
    if(argc == PARAMETERS){
        input     = argv[1];
        blur      = std::stoi(argv[2]);
        if(blur < 0 || blur > 5){
            std::cout<<"Blur kernel size wrong. Must be between 0 (no blur) and 5 (max kernel blurring size)."<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        kernel = std::stoi(argv[3]);
        if(kernel < 0  || kernel > 4){
            std::cout<<"Kernel choice incorrect."<<std::endl;
            std::cout<<"Kernel 0 : Canny."<<std::endl;
            std::cout<<"Kernel 1 : Prewitt."<<std::endl;
            std::cout<<"Kernel 2 : Kirsch."<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        t = std::stoi(argv[4]);
        direction = std::stoi(argv[5]);
        lowValue = std::stoi(argv[6]);
        highValue = std::stoi(argv[7]);
    }else if(argc == 1){
        std::cout<<"Using default parameters as no arguments."<<std::endl;
    }else{
        std::cout<<"Wrong number of arguments should be 0 or : "<<PARAMETERS-1<<" is : " << argc <<std::endl;
        std::cout<<"Should be : "<< PARAMETERS-1<<std::endl;
    }
}

int main(int argc, char** argv) {
    getElements(argc, argv);

    // Kernel
    auto start = std::chrono::high_resolution_clock::now();
    {
        Image i(input, blur, kernel, t, lowValue, highValue);
        i.init();
        if(direction == 0)
            i.execute_bi();
        else
            i.execute_multi();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Total kernel time: " << totalTime.count() << " ms"<<std::endl;

    return 0;
}
