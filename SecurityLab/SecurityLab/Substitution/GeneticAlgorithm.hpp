#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <set>
#include "../Ngrams/NgramsFrequency.hpp"

namespace substitution
{
    struct less_than_key
    {
        inline bool operator() (const std::pair<double, std::vector<std::string>>& struct1,
            const std::pair<double, std::vector<std::string>>& struct2)
        {
            return (struct1.first > struct2.first);
        }
    };

    class GeneticAlgorithm
    {
        const int KEY_LENGTH = 26;
        const int POPULATION_TO_REPRODUCE = 24;
        const int POPULATION_AMOUNT = 48;
        int alphabet_length = 1;

        std::vector<std::vector<std::string>> population;
        std::string text;
        std::string classic_alphabet;
        NgramsFrequency ngrams;

        std::map< std::vector<std::string>, double> founded;

        void FixKey(std::string& s)
        {
            s += classic_alphabet;

            auto last = s.end();

            for (auto first = s.begin(); first != last; ++first)
            {
                last = std::remove(std::next(first), last, *first);
            }

            s.erase(last, s.end());
        }

    public:
        GeneticAlgorithm(std::string str, int l) : text(str), alphabet_length(l)
        {
            char ch = 'A';
            for (int i = 0; i < KEY_LENGTH; i++)
            {
                classic_alphabet.push_back(ch);
                ch++;
            }
        };

        void GenerateRandomPopulation(int amount)
        {
            for (int i = 0; i < amount; i++)
            {
                std::vector<std::string> vs;
                for (int j = 0; j < alphabet_length; j++)
                {
                    std::string s = classic_alphabet;

                    std::random_shuffle(s.begin(), s.end());

                    vs.push_back(s);
                }

                population.push_back(vs);
            }
        }

        std::string SubstituteText(const std::string& text_to_deciper, const std::vector<std::string>& key)
        {
            std::vector<std::map<char, char>> sustitute;
            for (int i = 0; i < key.size(); i++)
            {
                sustitute.push_back(std::map<char, char>());
                for (int j = 0; j < classic_alphabet.length(); j++)
                {
                    sustitute[i][key[i][j]] = classic_alphabet[j];
                }
            }

            std::string result;
            for (int i = 0; i < text_to_deciper.length(); i++)
            {
                result.push_back(sustitute[i % key.size()][text_to_deciper[i]]);
            }
            return result;
        }

        double FitnesFunction(std::vector<std::string> key)
        {
            auto it = founded.find(key);
            if (it != founded.end())
            {
                return it->second;
            }
            auto text_to_compare = SubstituteText(text, key);
            double record = ngrams.Calculate(text_to_compare);
            founded[key] = record;
            return record;
        }

        auto SelectionPhase()
        {
            std::vector<std::pair<double, std::vector<std::string>>> individs_with_record;

            for (const auto& p : population)
            {
                individs_with_record.emplace_back(FitnesFunction(p), p);
            }

            std::sort(individs_with_record.begin(), individs_with_record.end(), less_than_key());

            return individs_with_record;
        }

        std::vector<std::string> CrossoverParrents(const std::vector<std::string>& parent1, const std::vector<std::string>& parent2)
        {
            if (parent1.size() != parent2.size())
                throw std::exception{ "Logic error" };

            auto child = parent1;

            for (int i = 0; i < parent1.size(); i++)
            {
                for (int j = rand() % KEY_LENGTH; j < KEY_LENGTH; j++)
                {
                    child[i][j] = parent2[i][j];
                }
                FixKey(child[i]);
            }

            return child;
        }

        void CrossoverPhase(const std::vector<std::pair<double, std::vector< std::string>>>& individs_with_record)
        {
            population.clear();;

            for (int i = 0; i < POPULATION_AMOUNT; i++)
            {
                population.push_back(individs_with_record[i].second);
            }

            for (int i = 0; i < alphabet_length; i++)
            {
                for (int j = 0; j < population.size(); j++)
                {
                    for (int k = j + 1; k < population.size(); k++)
                    {
                        if (population[j][i] == population[k][i])
                        {
                            MakeMutation(population[k][i]);
                        }
                    }
                }
            }

            for (int i = 0; i < POPULATION_TO_REPRODUCE - 1; i += 2)
            {
                population.push_back(CrossoverParrents(population[0], population[POPULATION_TO_REPRODUCE - i]));
                population.push_back(CrossoverParrents(population[i + 1], population[0]));
            }
        }

        void MakeMutation(std::string& individ)
        {
            int letter1 = rand() % KEY_LENGTH;
            int letter2 = rand() % KEY_LENGTH;

            if (letter1 == letter2)
            {
                letter1++;
            }

            std::swap(individ[letter1], individ[letter2]);
        }

        void MutationPhase()
        {
            for (int i = POPULATION_TO_REPRODUCE; i < population.size(); i++)
            {
                if (rand() % 2 == 0)
                {
                    int r = rand() % 10;
                    for (int j = 0; j < r; j++)
                    {
                        for (auto& p : population[i])
                            MakeMutation(p);
                    }
                }
            }
        }

        void StartCalculating(int start_population = 100, int count = 1000)
        {
            GenerateRandomPopulation(start_population);
            int counter = 0;
            double prev = 0.0;
            int flag = 0;
            while (true)
            {
                auto sorted_individs = SelectionPhase();
                CrossoverPhase(sorted_individs);
                MutationPhase();
                counter++;
                if (counter == count)
                {
                    break;
                }
                if (counter % 2000 == 0)
                {
                    founded.clear();
                }
                std::cout << counter;
                std::cout << "||||||";
                std::cout << sorted_individs.begin()->first;
                std::cout << std::endl;
            }

            for (int i = 0; i < 6; i++)
            {
                for (auto s : population[i])
                    std::cout << s << std::endl;

                std::cout << SubstituteText(text, population[i]) << std::endl;
            }
        }
    };
}
