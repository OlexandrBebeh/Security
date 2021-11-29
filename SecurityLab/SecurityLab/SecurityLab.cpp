#include <iostream>
#include <fstream>
#include <sstream>
#include "GeneticAlgorithm.hpp"
#include "./Substitution/GeneticAlgorithm.hpp"

int FromHex(const std::string& s) { return strtoul(s.c_str(), NULL, 16); }

void XORDeciper(std::string text)
{
    std::ofstream file;
    file.open("files/file.txt");
    char ch = '\0';
    for (int i = 0; i < 256; i++)
    {
        for (int i = 0; i < text.length() - 1; i += 2)
        {
            file << (char)(FromHex(text.substr(i,2)) ^ ch);
        }
        file << "\n=====================================================================================\n";
        ch++;
    }
}

int main()
{
    std::ifstream myfile1;
    myfile1.open("files/file1.txt");
    std::string text1((std::istreambuf_iterator<char>(myfile1)),
        std::istreambuf_iterator<char>());
    XORDeciper(text1);

    srand((unsigned)time(NULL));

    //std::ifstream myfile3;
    //myfile3.open("files/file3.txt");
    //std::string text3((std::istreambuf_iterator<char>(myfile3)),
    //    std::istreambuf_iterator<char>());
    //GeneticAlgorithm alg1(text3);
    //for (int i = 1; i < 30; i++)
    //{
    //    std::cout << alg1.IndexOfCoincidence(alg1.SelectSubstring(text3,i)) << std::endl;
    //}
    //alg1.StartCalculating();

    //std::ifstream myfile4;
    //myfile4.open("files/file4.txt");
    //std::string text4((std::istreambuf_iterator<char>(myfile4)),
    //    std::istreambuf_iterator<char>());
    //substitution::GeneticAlgorithm alg2(text4,1);
    //alg2.StartCalculating();

    //std::ifstream myfile5;
    //myfile5.open("files/file5.txt");
    //std::string text5((std::istreambuf_iterator<char>(myfile5)),
    //    std::istreambuf_iterator<char>());
    //substitution::GeneticAlgorithm alg3(text5, 4);
    //alg3.StartCalculating(100, 10000);
}
