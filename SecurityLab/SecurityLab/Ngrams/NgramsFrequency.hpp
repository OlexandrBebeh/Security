#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <math.h>  

class NgramsFrequency
{
public:
	std::map<std::string, double> quadgrams_frequency;
	std::map<std::string, double> trigrams_frequency;
	std::map<std::string, double> bigrams_frequency;
	std::map<std::string, double> monograms_frequency;

	double quadgrams_floor;
	double trigrams_floor;
	double bigrams_floor;
	double monograms_floor;

	const double quadgrams_influence = 0.4;
	const double trigrams_influence = 0.3;
	const double bigrams_influence = 0.2;
	const double monograms_influence = 0.1;

	NgramsFrequency()
	{
		quadgrams_floor = ReadFromTxT(quadgrams_frequency, "./resources/english_quadgrams.txt");
		trigrams_floor = ReadFromTxT(trigrams_frequency, "./resources/english_trigrams.txt");
		bigrams_floor = ReadFromTxT(bigrams_frequency, "./resources/english_bigrams.txt");
		monograms_floor = ReadFromTxT(monograms_frequency, "./resources/english_monograms.txt");
	}

	double ReadFromTxT(std::map<std::string, double>& ngrams_frequency, std::string file_name)
	{
		std::ifstream file(file_name);
		if (file.is_open())
		{
			double result = 0.0;
			std::string line;
			while (std::getline(file, line))
			{
				int pos = line.find(' ');
				auto ngram = line.substr(0, pos);
				line.erase(0, pos + 1);
				auto amount = std::stod(line);
				result += amount;
				ngrams_frequency[ngram] = amount;
			}
			file.close();

			for (const auto& pair : ngrams_frequency)
			{
				ngrams_frequency[pair.first] = std::log10(pair.second / result);
			}

			return  std::log10(0.01 / result);
		}
		return 0.0;
	}

	double Calculate(const std::string& text)
	{
		return  CalculateNgramsScore(text, quadgrams_frequency, quadgrams_floor) * quadgrams_influence +
			CalculateNgramsScore(text, trigrams_frequency, trigrams_floor) * trigrams_influence +
			CalculateNgramsScore(text, bigrams_frequency, bigrams_floor) * bigrams_influence +
			CalculateNgramsScore(text, monograms_frequency, monograms_floor) * monograms_influence;
	}

	static double CalculateNgramsScore(const std::string& text,
		const std::map<std::string, double>& ngrams_frequency,
		double floor)
	{
		if (ngrams_frequency.empty())
		{
			throw std::exception{ "Logic error" };
		}
		int len = ngrams_frequency.begin()->first.length();
		double result = 0.0;
		for (int i = 0; i < text.length() - len - 1; i++)
		{
			auto substr = text.substr(i, len);

			auto it = ngrams_frequency.find(substr);

			if (it != ngrams_frequency.end())
			{
				result += it->second;
			}
			else
			{
				result += floor;
			}
		}
		return result;
	}

};