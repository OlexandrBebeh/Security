#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <bitset>
#include <random>
#include <set>
#include <iostream>

struct less_than_key
{
    inline bool operator() (const std::pair<double, std::vector<std::bitset<8>>>& struct1,
        const std::pair<double, std::vector<std::bitset<8>>>& struct2)
    {
        return (struct1.first > struct2.first);
    }
};

class GeneticAlgorithm
{
    const int BYTE_LENGTH = 8;
    const int KEY_LENGTH = 3;
    const int POPULATION_AMOUNT = 24;
    const int POPULATION_TO_REPRODUCE = 12;

    using byte = std::bitset<8>;

    std::vector< std::vector<byte>> population;
    std::string text;

    std::map<char, double> indexies_of_frequency;

public:
    GeneticAlgorithm(std::string str) : text(str)
    {
        //char ch = 'A';
        //double frequency[] = { .082, .015, .028, .043, .127, .022, .020, .061, .070, .002, .008, .040, .024, .067, .075, .019, .001, .060, .063, .091, .028, .010, .023, .001, .020, .001 };
        //for (int i = 0; i < 26; i++)
        //{
        //    indexies_of_frequency[ch] = frequency[i];
        //    ch++;
        //}
    };

    std::string SelectSubstring(const std::string& str, int separator)
    {
        std::string result = "";
        for (int letter = 0; letter < str.length(); letter++)
        {
            if (!(letter % separator))
            {
                result.push_back(str[letter]);
            }
        }
        return result;
    }

    double IndexOfCoincidence(const std::string& str)
    {
        std::map<char, int> temp_letters;

        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] >= 'A' && str[i] <= 'Z')
                temp_letters[str[i] - ('A' - 'a')] += 1;

            if (str[i] >= 'a' && str[i] <= 'z')
                temp_letters[str[i]] += 1;
        }

        double index_of_coincidence = 0.0;
        double length = 0.0;
        for (const auto& pair : temp_letters)
        {
            length += pair.second;
            index_of_coincidence += pair.second * (pair.second - 1.0);
        }
        index_of_coincidence /= (length * (length - 1));

        return index_of_coincidence;
    }

    void GenerateRandomPopulation(int amount)
    {
        for (int i = 0; i < amount; i++)
        {
            std::vector<byte> s;

            for (int j = 0; j < KEY_LENGTH; j++)
            {
                int rand_char = std::rand() % 256;

                s.push_back(rand_char);
            }

            population.push_back(s);
        }
    }

    std::string XORText(std::string key)
    {
        std::string result = "";
        for (int i = 0; i < text.length(); i++)
        {
            result.push_back(text[i] ^ key[i % key.size()]);
        }

        return result;
    }

    double FitnesFunction(std::vector<byte> key)
    {
        std::string key_in_string = "";

        for (int i = 0; i < key.size(); i++)
        {
            key_in_string.push_back(static_cast<char>(key[i].to_ulong()));
        }
        auto text_to_compare = XORText(key_in_string);

        double known_symbols = 0.0;

        for (int i = 0; i < text.length(); i++)
        {
            if (text_to_compare[i] >= 'A' && text_to_compare[i] <= 'Z')
                known_symbols++;
            else if (text_to_compare[i] >= 'a' && text_to_compare[i] <= 'z')
                known_symbols++;
            else if (text_to_compare[i] == ' ')
                known_symbols++;
        }

        double percent_of_engkish_text = known_symbols / text.length();

        return percent_of_engkish_text;
    }

    auto SelectionPhase()
    {
        std::vector<std::pair<double, std::vector<byte>>> individs_with_record;

        for (const auto& p : population)
        {
            individs_with_record.emplace_back(FitnesFunction(p), p);
        }

        std::sort(individs_with_record.begin(), individs_with_record.end(), less_than_key());

        return individs_with_record;
    }

    std::vector<byte> CrossoverParrents(const std::vector<byte>& parent1, const std::vector<byte>& parent2)
    {
        if (parent1.size() != parent2.size())
            throw std::exception{ "Logic error" };
        std::vector<byte> child = parent1;

        int separate_bit = rand() % BYTE_LENGTH + BYTE_LENGTH;

        for (int i = separate_bit; i < KEY_LENGTH * 8; i++)
        {
            child[i / BYTE_LENGTH][i % BYTE_LENGTH] = parent2[i / BYTE_LENGTH][i % BYTE_LENGTH];
        }

        return child;
    }

    void CrossoverPhase(std::vector<std::pair<double, std::vector<byte>>> individs_with_record)
    {
        population.clear();
        for (int i = 0; i < POPULATION_TO_REPRODUCE; i++)
        {
            population.push_back(individs_with_record[i].second);
        }

        for (int i = 0; i < POPULATION_TO_REPRODUCE - 1; i += 2)
        {
            population.push_back(CrossoverParrents(population[i], population[i + 1]));
            population.push_back(CrossoverParrents(population[i + 1], population[i]));
        }
    }

    void MakeMutation(std::vector<byte>& individ)
    {
        int change_bit = rand() % (KEY_LENGTH * 8);

        int bit = individ[change_bit / 8][(change_bit % 8)];

        individ[change_bit / 8][(change_bit % 8)] = !bit;
    }

    void MutationPhase()
    {
        for (int i = POPULATION_TO_REPRODUCE; i < population.size(); i++)
        {
            if (rand() % 2 == 0)
            {
                MakeMutation(population[i]);
            }
        }
    }

    void StartCalculating()
    {
        GenerateRandomPopulation(10000);
        int counter = 0;
        while (true)
        {
            auto sorted_individs = SelectionPhase();
            CrossoverPhase(sorted_individs);
            MutationPhase();
            counter++;
            if (counter == 1000)
            {
                break;
            }
        }

        for (auto p : population)
        {
            std::string key_in_string = "";

            for (int i = 0; i < p.size(); i++)
            {
                key_in_string.push_back(static_cast<char>(p[i].to_ulong()));
            }
            std::cout << key_in_string << std::endl;

            std::cout << XORText(key_in_string) << std::endl;
        }
        std::cout << " " << std::endl;
    }
};