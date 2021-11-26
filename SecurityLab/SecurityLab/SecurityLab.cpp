#include <iostream>
#include <fstream>
#include <sstream>
#include "GeneticAlgorithm.hpp"
#include "./Substitution/GeneticAlgorithm.hpp"

int FromHex(const std::string& s) { return strtoul(s.c_str(), NULL, 16); }

int main()
{
    srand((unsigned)time(NULL));
    std::ifstream myfile3;
    myfile3.open("files/file3.txt");
    std::string text3((std::istreambuf_iterator<char>(myfile3)),
        std::istreambuf_iterator<char>());
    GeneticAlgorithm alg1(text3);
    for (int i = 1; i < 30; i++)
    {
        std::cout << alg1.IndexOfCoincidence(alg1.SelectSubstring(text3,i)) << std::endl;
    }
    alg1.StartCalculating();

    std::ifstream myfile4;
    myfile4.open("files/file4.txt");
    std::string text4((std::istreambuf_iterator<char>(myfile4)),
        std::istreambuf_iterator<char>());
    substitution::GeneticAlgorithm alg2(text4,1);
    alg2.StartCalculating();

    std::ifstream myfile5;
    myfile5.open("files/file5.txt");
    std::string text5((std::istreambuf_iterator<char>(myfile5)),
        std::istreambuf_iterator<char>());
    substitution::GeneticAlgorithm alg3(text5, 4);
    alg3.StartCalculating(100, 10000);
}
