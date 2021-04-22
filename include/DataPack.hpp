/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataPack.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:04:13 by pitriche          #+#    #+#             */
/*   Updated: 2021/04/22 21:28:59 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <vector>
#include <array>

#define TUPLE_SIZE 5

typedef std::array<float, TUPLE_SIZE> Tuple;

class DataPack
{
	public:
		DataPack(void) { }
		~DataPack(void) { }

		void	parse(std::istream &is);
		void	batch(std::vector<std::vector<Tuple> > &batches, unsigned
			batch_size) const;

	private:
		std::vector<Tuple>	dataset;
};

void	DataPack::parse(std::istream &is)
{
	std::string			tmp;
	std::stringstream	sst;

	std::getline(is, tmp);
	std::getline(is, tmp);
	sst.str(tmp);
	while (sst.good())
	{
		std::getline(sst, tmp, ',');
		std::cout <<">"<< tmp <<"<"<<std::endl;
	}
	
}
