/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 15:14:08 by pitriche          #+#    #+#             */
/*   Updated: 2021/04/24 22:15:00 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <cmath>

#include "DataPack.hpp"

#define SEED				460
#define TRAIN_TEST_RATIO	0.7f
#define LEARNING_RATE		0.05f
#define TRAIN_CYCLES		650000

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

static inline real_t	cost(const DataPack &test, const Tuple &theta,
	real_t type)
{
	real_t res;

	res = 0;
	for (const Tuple &example : test.dataset)
	{
		if (example[0] == type)
			res += (real_t)log(predict(example, theta));
		else
			res += (real_t)log(1 - predict(example, theta));
	}
	return (-res / (real_t)test.dataset.size());
}

// #############################################################################

static void	train_cycle(const DataPack &train_batch, Tuple &theta,
	const real_t type)
{
	Tuple	derivatives;
	real_t	is_type;
	real_t	difference;

	derivatives.fill(0);
	for (const Tuple &example : train_batch.dataset)
	{
		is_type = example[0] == type ? 1 : 0;
		difference = predict(example, theta) - is_type;
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
			derivatives[feature] += difference * example[feature];
	}
	for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
		theta[feature] -= derivatives[feature] * LEARNING_RATE /
		(real_t)train_batch.dataset.size();
}

static void	train_linreg(const DataPack &train, const DataPack &test)
{
	std::array<Tuple, 4> theta_types;

	for (unsigned type = 0; type < 4; ++type) // for each type (hogwarts houses)
	{
		std::cout << "Type "<< type << ":" << std::endl;
		theta_types[type].fill(0);
		for (unsigned epoch = 0; epoch < TRAIN_CYCLES; ++epoch)
		{
			if (epoch % (TRAIN_CYCLES / 10) == 0)
				std::cout << "Epoch "<< epoch << ", cost is " <<
				cost(test, theta_types[type], (real_t)type) << std::endl;
			train_cycle(train, theta_types[type], (real_t)type);
		}
		std::cout << "Final epoch "<< TRAIN_CYCLES << ", cost is " <<
		cost(test, theta_types[type], (real_t)type) << std::endl << std::endl;
	}
}

// #############################################################################

int			main(int ac, char **av)
{
	std::srand(SEED);

	DataPack 		train;
	DataPack 		test;
	std::ifstream	ifs;
	std::string		filename;

	if (ac < 2)
	{
		std::cout << "Default file: dataset_train.csv" << std::endl;
		filename = "dataset_train.csv";
	}
	else
		filename = std::string(av[1]);
	ifs.open(filename);
	if (!ifs.is_open())
	{
		std::cout << "Impossible to open file" << std::endl;
		return (0);
	}
	try { train.parse(ifs); }
	catch (std::exception &e) { return (0); }
	train.correct();
	train.normalize();
	test = train.split(1 - TRAIN_TEST_RATIO);
	std::cout << "train set size: " << train.dataset.size() << std::endl;
	std::cout << "test set size: " << test.dataset.size() << std::endl;
	train_linreg(train, test);
	return (0);
}
