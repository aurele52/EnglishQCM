#ifndef AURELEQCM_CLASS_H
# define AURELEQCM_CLASS_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>

struct word
{
	word( std::string ieng, std::string ifr, std::string ihint) : eng(ieng), fr(ifr), hint(ihint), level(0) 
	{
	}
	std::string eng;
	std::string fr;
	std::string hint;
	int	level;

};

class aureleQCM
{

	public:

		void	addWord( std::string eng, std::string fr, std::string hint);
		void	start( int number );
		std::string	api(std::string word);
		aureleQCM( void );
		aureleQCM( aureleQCM const & src );
		~aureleQCM( void );
		aureleQCM & operator=( aureleQCM const & rhs );


	private:
		std::vector<word> list;
};

std::ostream & operator<<( std::ostream & o, aureleQCM const & i );

#endif
