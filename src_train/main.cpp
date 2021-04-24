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
#include "DataPack.hpp"

int	main(int ac, char **av)
{
	DataPack 		dp;
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
	try { dp.parse(ifs); }
	catch (std::exception &e) { return (0); }
	// dp.correct();
	dp.dataset = dp.normallize();
	dp.correct();
	return (0);
}
