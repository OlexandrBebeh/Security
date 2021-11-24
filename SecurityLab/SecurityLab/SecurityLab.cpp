#include <iostream>
#include <fstream>
#include <sstream>
#include "GeneticAlgorithm.hpp"
#include "./Substitution/GeneticAlgorithm.hpp"

int FromHex(const std::string& s) { return strtoul(s.c_str(), NULL, 16); }

int main()
{
    //srand((unsigned)time(NULL));
    //std::ifstream myfile3;
    //myfile3.open("file3.txt");
    //std::string text3((std::istreambuf_iterator<char>(myfile3)),
    //    std::istreambuf_iterator<char>());
    //GeneticAlgorithmXor alg(text3);
    //for (int i = 1; i < 30; i++)
    //{
    //    std::cout << alg.IndexOfCoincidence(alg.SelectSubstring(text3,i)) << std::endl;
    //}
    //alg.StartCalculating();

    std::ifstream myfile4;
    myfile4.open("files/file4.txt");
    std::string text4((std::istreambuf_iterator<char>(myfile4)),
        std::istreambuf_iterator<char>());
    substitution::GeneticAlgorithm alg(text4);
    alg.StartCalculating();
}
