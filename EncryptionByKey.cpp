// EncryptionByKey.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Encrypter.h"
#include <chrono>
#include <limits>

int main()
{
    std::cout << "Hello World!\n";
    std::cout << "Practuchna #2, encrypter/decipher(decrypter), that save the result to the file and load data from the file" << std::endl;
    std::cout << "Made by Drochak Stanislave" << std::endl;
    
    Encrypter enc;
    
    int mode;

    enc.loadFile(); // Load the file with which to work with

    do {
        std::cout << "Load/set encrypting/decrypting key(max lenght is 100): ";
        std::cin >> enc;
    } while (enc.checkKey());

    do { // Choose to use multi threads or single thread
        std::cout << "Chose the method of encrypting/decrypting: " << std::endl;
        std::cout << "Multi threads - 0, single thread - 1: ";
        std::cin >> mode;

        if (mode < 0 || mode > 1 || std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        else
        {
            enc.setThreadMode(mode);
            break;
        }
    } while (true);

    if (enc.getThreadMode() == 1) {
        auto start = std::chrono::high_resolution_clock::now();
        enc.startWork();
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Elapsed time single thread: " << diff << std::endl;
    }
    else {
        auto start = std::chrono::high_resolution_clock::now();
        enc.multiThreadStartWork();
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Elapsed time multi threads: " << diff << std::endl;
    }
    
    enc.saveToFile();
    std::cout << "Data saved to file, called: " << enc.getFileName() << std::endl;
    
    system("pause");
    return 0;
}
