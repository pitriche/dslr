/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 15:14:08 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/04 16:37:34 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <cmath>

#include "DataPack.hpp"

// #############################################################################

static inline real_t	sigmoid(real_t n)
{ return (1 / static_cast<real_t>(1 + pow(static_cast<real_t>(M_E), -n))); }

static inline real_t	predict(const Tuple &input, const Tuple &theta)
{
	real_t res;

	res = 0;
	for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
		res += input[feature] * theta[feature];
	return (sigmoid(res));
}

static real_t			classify(const Tuple &input,
	std::array<Tuple, 4> theta_types)
{
	std::array<real_t, 4> results;

	for (unsigned type = 0; type < 4; ++type)
		results[type] = predict(input, theta_types[type]);
	return ((real_t)(std::max_element(results.begin(), results.end()) - results.begin()));
}

// #############################################################################

static std::array<Tuple, 4>	theta_from_file(void)
{
	std::array<Tuple, 4>	theta_types;
	std::ifstream			ifs;

	for (unsigned type = 0; type < 4; ++type)
		theta_types[type].fill(0.0f);
	ifs.open("theta", std::fstream::out);
	if (!ifs.is_open())
	{
		std::cout << "Cannot open theta, weights initialized at 0" << std::endl;
		return (theta_types);
	}
	try
	{
		for (unsigned type = 0; type < 4; ++type)
			for (real_t &weight : theta_types[type])
				ifs >> weight;
	}
	catch (std::exception &e) // probably useless
	{
		std::cout << "Stop messing with my files" << std::endl;
		exit(0);
	}
	ifs.close();
	return (theta_types);
}

// #############################################################################

static void	execute(std::ostream &os, const DataPack &test)
{
	std::array<Tuple, 4>		theta_types;
	std::array<std::string, 4>	houses;

	houses[0] = "Gryffindor";
	houses[1] = "Slytherin";
	houses[2] = "Hufflepuff";
	houses[3] = "Ravenclaw";
	theta_types = theta_from_file();
	os << "Index,Hogwarts House" << std::endl;
	for(unsigned element = 0; element < test.dataset.size(); ++element)
		os << test.dataset[element][0] << ',' <<
		houses[(unsigned)classify(test.dataset[element], theta_types)] << std::endl;
}

// #############################################################################

int			main(int ac, char **av)
{
	DataPack 		test;
	std::ifstream	ifs;
	std::ofstream	ofs;
	std::string		filename;

	if (ac < 2)
	{
		std::cout << "Default file: dataset_test.csv" << std::endl;
		filename = "dataset_test.csv";
	}
	else
		filename = std::string(av[1]);
	ifs.open(filename);
	if (!ifs.is_open())
	{
		std::cout << "Impossible to open file" << std::endl;
		return (0);
	}
	try { test.parse(ifs, true); }
	catch (std::exception &e) { return (0); }
	//test.purge();
	test.test_correct();
	test.normalize();
	std::cout << "test set size: " << test.dataset.size() << std::endl;

	ofs.open("houses.csv", std::fstream::out);
	if (!ofs.is_open())
	{
		std::cout << "There is no curse in Elvish, Entish, or the tongues of Me"
		"n for this treachery." << std::endl;
		exit(0);
	}
	execute(ofs, test);
	ofs.close();
	return (0);
}
