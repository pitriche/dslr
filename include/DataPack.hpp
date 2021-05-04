/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataPack.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:04:13 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/04 16:37:02 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>


typedef	float real_t;	// real type to use everywhere

#define TUPLE_SIZE 9
typedef std::array<real_t, TUPLE_SIZE>	Tuple;

struct DataPack
{
	public:
		std::vector<Tuple>	dataset;


		DataPack(void) { }
		DataPack(std::vector<Tuple> dataset_) : dataset(dataset_) { }
		~DataPack(void) { }

		// parse input stream to gather the data
		void		parse(std::istream &is, bool is_test);
		

		// correct missing data
		void		purge(void);		// by purging it
		// by mean substitution
		void		correct(void);		// from data type mean
		void		test_correct(void); // from general mean

		// normalize the data
		void		normalize(void);

		// split the data in 2 sets randomly
		DataPack	split(double ratio);

		DataPack &operator=(const DataPack &rhs)
		{
			this->dataset = rhs.dataset;
			return (*this);
		}
};

// #############################################################################

static Tuple	_parse_example(std::string &line, bool is_test)
{
	Tuple				tup;
	std::stringstream	sst;
	std::string			tmp;
	unsigned			element;
	unsigned			feature_tup;

	sst.str(line);
	element = 0;
	feature_tup = 1;
	while (sst.good())
	{
		tmp.clear();
		std::getline(sst, tmp, ',');
		if (element == 0 )
			tup[0] = (real_t)std::stold(tmp);
		if (element == 1 && !is_test)
		{
			if (tmp == "Gryffindor")
				tup[0] = 0.0f;
			else if (tmp == "Slytherin")
				tup[0] = 1.0f;
			else if (tmp == "Hufflepuff")
				tup[0] = 2.0f;
			else if (tmp == "Ravenclaw")
				tup[0] = 3.0f;
			else
				throw std::logic_error("Invalid house : >" + tmp + "<");
		}
		else if (element >= 7 && element != 9 && element != 15 &&
			element != 16 && element != 17)	// select only useful features
											// 15 and 17 can be added
		{
			if (!tmp.empty())
				tup[feature_tup++] = (real_t)std::stold(tmp);
			else
				tup[feature_tup++] = NAN;
		}
		element++;
	}
	if (element != 19)
		throw std::logic_error("Invalid example");
	return (tup);
}

void	DataPack::parse(std::istream &is, bool is_test)
{
	std::string			tmp;

	std::getline(is, tmp);
	while (is.good())
	{
		std::getline(is, tmp);
		if (tmp.empty()) // skip empty lines including the last one
			continue ;
		try { this->dataset.push_back(_parse_example(tmp, is_test)); }
		catch (std::exception &e)
		{
			std::cerr << "Parsing failed: " << e.what() << std::endl;
			throw e;
		}
	}
	std::cout << "There are " << dataset.size() << " elements in dataset" <<
	std::endl;
}

// #############################################################################

void	DataPack::purge(void) // don't make me use this please
{
	auto 		it = this->dataset.begin();
	unsigned	purged;
	
	purged = 0;
	while (it != this->dataset.end())
	{
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			if (std::isnan((*it)[feature]))
			{
				this->dataset.erase(it);
				++purged;
				--it;
				break ;
			}
		++it;
	}
	std::cout << "Purged " << purged << std::endl;
}


void	DataPack::test_correct(void)
{
	Tuple		sum;
	Tuple		number;	// number of each features not NaN

	sum.fill(0.0f);
	number.fill(0.0f);
	for (auto &example : this->dataset)
	{
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			if (!std::isnan(example[feature]))
			{
				number[feature]++;
				sum[feature] += example[feature];
			}
	}
	for (unsigned i = 1; i < TUPLE_SIZE; ++i)
		sum[i] /= number[i];
	for (auto &example : this->dataset)
	{
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			if (std::isnan(example[feature]))
				example[feature] = sum[feature];
	}
}

void	DataPack::correct(void)
{
	Tuple		sum[4];
	Tuple		number[4];	// number of each features not NaN
	unsigned	type_id;

	for (unsigned i = 0; i < 4; ++i) // zero fill the arrays
	{
		sum[i].fill(0.0);
		number[i].fill(0.0);
	}
	for (auto &example : this->dataset)
	{
		type_id = (unsigned)example[0];
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			if (!std::isnan(example[feature]))
			{
				number[type_id][feature]++;
				sum[type_id][feature] += example[feature];
			}
	}
	for (unsigned i = 1; i < TUPLE_SIZE; ++i)
	{
		sum[0][i] /= number[0][i];
		sum[1][i] /= number[1][i];
		sum[2][i] /= number[2][i];
		sum[3][i] /= number[3][i];
	}
	for (auto &example : this->dataset)
	{
		type_id = (unsigned)example[0];
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			if (std::isnan(example[feature]))
				example[feature] = sum[type_id][feature];
	}
}

// #############################################################################

void	DataPack::normalize(void)
{
	Tuple		min;
	Tuple		max;

	for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
	{
		min[feature] = INFINITY;
		max[feature] = -INFINITY;
	}
	for (auto &example : this->dataset)
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
		{
			if (example[feature] < min[feature])
				min[feature] = example[feature];
			if (example[feature] > max[feature])
				max[feature] = example[feature];
		}
	for (auto &example : this->dataset)
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
		{
			example[feature] -= min[feature];
			example[feature] /= max[feature] - min[feature];
		}
}

// #############################################################################

DataPack	DataPack::split(double ratio)
{
	DataPack		pack;
	unsigned		pack_size;
	size_t			swap_id;

	pack_size = static_cast<unsigned>(ratio *
		static_cast<double>(this->dataset.size()));
	for (unsigned i = 0; i < pack_size; ++i)
	{
		swap_id = (size_t)std::rand() % this->dataset.size();
		pack.dataset.push_back(this->dataset[swap_id]);
		this->dataset.erase(this->dataset.begin() + (long)swap_id);
	}
	return (pack);
}
