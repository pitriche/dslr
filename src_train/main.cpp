/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 15:14:08 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/04 20:08:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <cmath>
#include <climits>

#include "DataPack.hpp"

#define SEED				460
#define TRAIN_TEST_RATIO	0.5f
#define LEARNING_RATE		0.1f
#define TRAIN_CYCLES		70000

#define EARLYSTOP_CHECKS	(TRAIN_CYCLES / 10) // set to 1 to disable early stopping

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

// #############################################################################

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

static void					theta_to_file(std::array<Tuple, 4> theta_types)
{
		std::ofstream	ofs;

		ofs.open("theta", std::fstream::out);
		if (!ofs.is_open())
		{
			std::cout << "I hate your guts" << std::endl;
			exit(0);
		}
		for (unsigned type = 0; type < 4; ++type)
		{
			for (const real_t &weight : theta_types[type])
				ofs << weight << '\n';
			if (type < 3)
				ofs << '\n';
		}
		ofs.close();
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
	std::array<Tuple, 4>	theta_types;
	std::array<Tuple, 4>	theta_types_min;
	real_t					cost_;
	real_t					min_cost;
	unsigned				epoch;

	for (unsigned type = 0; type < 4; ++type) // for each type (hogwarts houses)
	{
		std::cout << "Type "<< type << ":" << std::endl;

		// initialize the weights
		// theta_types[type].fill(0);
		for (real_t &weight : theta_types[type])
			weight = 1.0f - ((real_t)std::rand() / (real_t)INT_MAX) * 2;
		
		min_cost = INFINITY;
		for (epoch = 0; epoch < TRAIN_CYCLES; ++epoch)
		{
			if (epoch % (TRAIN_CYCLES / EARLYSTOP_CHECKS) == 0)	// earlystopping
			{
				cost_ = cost(test, theta_types[type], (real_t)type);
				if (cost_ < min_cost)
				{
					min_cost = cost_;
					theta_types_min[type] = theta_types[type];
				}
				else
				{
					std::cout << "Early stopping !" << std::endl;
					theta_types[type] = theta_types_min[type];
					break ;
				}
			}

			if (epoch % (TRAIN_CYCLES / 10) == 0)	// display
				std::cout << "Epoch "<< epoch << ", cost is:\t\t" <<
				cost(test, theta_types[type], (real_t)type) << std::endl;

			train_cycle(train, theta_types[type], (real_t)type);
		}
		std::cout << "Final epoch "<< epoch << ", cost is:\t" <<
		cost(test, theta_types[type], (real_t)type) << std::endl << std::endl;
	}
	// accuracy rating, pass the entire dataset as tot and uncomment classify
	// unsigned misses = 0;
	// for (auto element : tot.dataset)
	// {
	// 	misses += 1 - (classify(element, theta_types) == element[0]);
	// }
	// std::cout << "missed " << misses << std::endl;
	// real_t acc;
	// acc = 100 - (misses / (real_t)tot.dataset.size()) * 100;
	// std::cout << "accuracy score : " << acc << std::endl;
	theta_to_file(theta_types);
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
	try { train.parse(ifs, false); }
	catch (std::exception &e) { return (0); }
	train.correct();
	train.normalize();
	test = train.split(1 - TRAIN_TEST_RATIO);
	std::cout << "train set size: " << train.dataset.size() << std::endl;
	std::cout << "test set size: " << test.dataset.size() << std::endl;
	train_linreg(train, test);
	return (0);
}
