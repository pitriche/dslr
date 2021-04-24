/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataPack.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:04:13 by pitriche          #+#    #+#             */
/*   Updated: 2021/04/24 22:14:11 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>


typedef	float	real_t;	// real type to use everywhere

#define TUPLE_SIZE 15
typedef std::array<real_t, TUPLE_SIZE>	Tuple;

struct DataPack
{
	public:
		std::vector<Tuple>	dataset;


		DataPack(void) { }
		~DataPack(void) { }

		// parse input stream to gather the data
		void					parse(std::istream &is);
		
		// correct missing data
		void					correct(void);

		// return a copy of the data, normalized
		std::vector<Tuple>		normallize(void);

		// return the data randomly separated in batches
		void	batch(std::vector<std::vector<Tuple> > &batches, unsigned
			batch_size) const;
};

// #############################################################################

static Tuple	_parse_example(std::string &line)
{
	Tuple				tup;
	std::stringstream	sst;
	std::string			tmp;
	unsigned			element;

	sst.str(line);
	element = 0;
	while (sst.good())
	{
		tmp.clear();
		std::getline(sst, tmp, ',');
		if (element == 1)
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
		if (element == 5)
			tup[1] = tmp == "Right" ? (real_t)1.0f : (real_t)0.0f;
		else if (element >= 6)
		{
			if (!tmp.empty())
				tup[element - 4] = (real_t)std::stold(tmp);
			else
				tup[element - 4] = NAN;
		}
		element++;
	}
	if (element != 19)
		throw std::logic_error("Invalid example");
	return (tup);
}

void	DataPack::parse(std::istream &is)
{
	std::string			tmp;

	std::getline(is, tmp);
	while (is.good())
	{
		std::getline(is, tmp);
		if (tmp.empty()) // skip empty lines including the last one
			continue ;
		try { this->dataset.push_back(_parse_example(tmp)); }
		catch (std::exception &e)
		{
			std::cerr << "Parsing failed: " << e.what() << std::endl;
			throw e;
		}

	}
	std::cout << "There are " << dataset.size() << " elements in dataset" <<
	std::endl;
}

void	DataPack::correct(void)
{
	Tuple		type[4]; // storing the amount of this type's data at index 0
	unsigned	type_id;

	for (unsigned i = 0; i < TUPLE_SIZE; ++i)
	{
		type[0][i] = static_cast<real_t>(0);
		type[1][i] = static_cast<real_t>(0);
		type[2][i] = static_cast<real_t>(0);
		type[3][i] = static_cast<real_t>(0);
	}
	for (auto &example : this->dataset)
	{
		type_id = (unsigned)example[0];
		type[type_id][0]++;
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			if (!std::isnan(example[feature]))
				type[type_id][feature] += example[feature];
	}
	for (unsigned i = 1; i < TUPLE_SIZE; ++i)
	{
		type[0][i] /= type[0][0];
		type[1][i] /= type[1][0];
		type[2][i] /= type[2][0];
		type[3][i] /= type[3][0];
	}
	for (auto &example : this->dataset)
	{
		type_id = (unsigned)example[0];
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			if (std::isnan(example[feature]))
				example[feature] = type[type_id][feature];
	}
	for (unsigned i = 1; i < TUPLE_SIZE; ++i)
	{
		std::cout << "feature: " << i << std::endl;
		std::cout << "type 0>" << type[0][i] << "\t";
		std::cout << "type 1>" << type[1][i] << "\t";
		std::cout << "type 2>" << type[2][i] << "\t";
		std::cout << "type 3>" << type[3][i] << std::endl << std::endl;
	}
}

std::vector<Tuple>	DataPack::normallize(void)
{
	std::vector<Tuple>	norm;
	Tuple				min;
	Tuple				max;

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
	norm = this->dataset;
	for (auto &example : norm)
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
		{
			example[feature] -= min[feature];
			example[feature] /= max[feature] - min[feature];
		}
	
	return (norm);
}
