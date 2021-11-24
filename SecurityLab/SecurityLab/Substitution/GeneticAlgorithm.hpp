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
        inline bool operator() (const std::pair<double, std::string>& struct1,
            const std::pair<double, std::string>& struct2)
        {
            return (struct1.first > struct2.first);
        }
    };

    class GeneticAlgorithm
    {
        const int KEY_LENGTH = 26;
        const int POPULATION_TO_REPRODUCE = 12;
        const int POPULATION_AMOUNT = 24;
        int ALPHABET_LENGTH = 26;

        std::vector<std::string> population;
        std::string text;
        std::string classic_alphabet;

        NgramsFrequency ngrams;

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
        GeneticAlgorithm(std::string str) : text(str)
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
                std::string s = classic_alphabet;

                std::random_shuffle(s.begin(), s.end());

                population.push_back(s);
            }
        }

        std::string SubstituteText(const std::string& text_to_deciper, const std::string& key)
        {
            if (key.length() != classic_alphabet.length())
            {
                throw std::exception{ "Logic error" };
            }

            std::map<char, char> sustitute;
            for (int i = 0; i < classic_alphabet.length(); i++)
            {
                sustitute[key[i]] = classic_alphabet[i];
            }

            std::string result;
            for (const auto& ch : text_to_deciper)
            {
                result.push_back(sustitute[ch]);
            }
            return result;
        }

        double FitnesFunction(std::string key)
        {
            auto text_to_compare = SubstituteText(text, key);

            return ngrams.Calculate(text_to_compare);
        }

        auto SelectionPhase()
        {
            std::vector<std::pair<double, std::string>> individs_with_record;

            for (const auto& p : population)
            {
                individs_with_record.emplace_back(FitnesFunction(p), p);
            }

            std::sort(individs_with_record.begin(), individs_with_record.end(), less_than_key());

            return individs_with_record;
        }

        std::string CrossoverParrents(const std::string& parent1, const std::string& parent2)
        {
            if (parent1.size() != parent2.size())
                throw std::exception{ "Logic error" };
            auto child = parent1;

            for (int i = rand() % (KEY_LENGTH / 2) + KEY_LENGTH / 4; i < KEY_LENGTH; i++)
            {
                child[i] = parent2[i];
            }
            FixKey(child);
            return child;
        }

        void CrossoverPhase(const std::vector<std::pair<double, std::string>>& individs_with_record)
        {
            population.clear();;

            for (int i = 0; i < POPULATION_AMOUNT; i++)
            {
                population.push_back(individs_with_record[i].second);
            }

            for (int i = 0; i < POPULATION_TO_REPRODUCE - 1; i += 2)
            {
                population.push_back(CrossoverParrents(population[i], population[i + 1]));
                population.push_back(CrossoverParrents(population[i + 1], population[i]));
            }

        }

        void MakeMutation(std::string& individ)
        {
            int letter1 = rand() % KEY_LENGTH;
            int letter2 = rand() % KEY_LENGTH;

            std::swap(individ[letter1], individ[letter2]);
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
            GenerateRandomPopulation(100);
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
                std::cout << sorted_individs.begin()->first;
                std::cout << ":";
                std::cout << sorted_individs.begin()->second << std::endl;
            }

            for (auto p : population)
            {
                std::cout << p << std::endl;

                std::cout << SubstituteText(text, p) << std::endl;
            }
        }
    };
}
