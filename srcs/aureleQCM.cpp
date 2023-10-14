#include "../headers/aureleQCM.hpp"
#include "../headers/main.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <cstring>
#include <dirent.h>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <vector>
#include "../headers/api.hpp"

std::string	aureleQCM::api(std::string word)
{
	// POST /v2/translate HTTP/2
	// lol += "{\"text\":[\"hello\"],\"target_lang\":\"FR\"}\r\n";
	// std::string lol = "POST /v2/translate HTTP/2\r\n";
	try
	{
		http::Request request{"http://api-free.deepl.com/v2"};
		const auto response = request.send("GET");
		// http::Request request{"http://api-free.deepl.com/v2/translate"};
		// const std::string body = "{\"text\": [\"hello\"], \"target_lang\": \"FR\"}";
		// const auto response = request.send("POST", body, {
		// 	{"Content-Type", "application/json"}, {"Authorization" , "DeepL-Auth-Key ''"},{"User-Agent","aureleQCM/1.2.3"}
		// });
		std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; // print the result
	}
	catch (const std::exception& e)
	{
		std::cerr << "Request failed, error: " << e.what() << '\n';
	}
	return ("");
}

void	aureleQCM::addWord( std::string eng, std::string fr, std::string hint)
{
	word lol(eng,fr,hint);

	for (std::vector<word>::iterator it = list.begin(); it != list.end();it++)
	{
		if ((*it).eng == eng)
			return;
	}
	list.push_back(lol);
}

void	aureleQCM::start( int number )
{
	std::string test;
	std::random_shuffle(list.begin(), list.end());
	for (std::vector<word>::iterator it = list.begin(); it != list.end();it++)
	{
		if ((*it).level == number || number == -1)
		{
			std::cout << (*it).eng << " : " << std::endl;
			std::getline(std::cin, test);
			if (std::cin.eof())
				return ;
			if ((*it).fr == test)
			{
				std::cout << "ok" << std::endl;
				(*it).level++;
			}
			else if (test == "h")
				std::cout << (*it).hint << std::endl;
			else
			{
				std::cout << "is it same as : " << (*it).fr << "?" << std::endl;
				std::getline(std::cin, test);
				if (std::cin.eof())
					return ;
				if (test == "ok" || test == "o" || test == "y" || test == "yes")
				{
					std::cout << "level add" << std::endl;
					(*it).level++;
				}

			}
		}
	}
}

aureleQCM::~aureleQCM( void )
{
	std::ofstream out;

	out.open("word.txt", std::ios::out);
	if (!out.is_open())
		std::cout << "sad" << std::endl;
	for (std::vector<word>::iterator it = list.begin(); it != list.end();it++)
		out << (*it).fr << ";" << (*it).eng << ";" << (*it).hint << ";" << (*it).level << std::endl;

}

aureleQCM::aureleQCM( void )
{
	std::ifstream in;
	std::string str;
	std::string tmp;
	std::istringstream parseline;
	word	word("","","");

	in.open("word.txt", std::ios::in);
	if ( in.is_open() )
	{
		in >> str;
		while (in.good())
		{
			parseline.clear();
			parseline.str(str);
			getline(parseline, tmp, ';');
			word.fr = tmp;
			getline(parseline, tmp, ';');
			word.eng = tmp;
			getline(parseline, tmp, ';');
			word.hint = tmp;
			parseline >> word.level;
			list.push_back(word);
			in >> str;
		}
	}
	return;

}

aureleQCM::aureleQCM( aureleQCM const & src )
{

	std::cout << "Copy Constructor Called" << std::endl;
	*this = src;

	return;

}

aureleQCM & aureleQCM::operator=( aureleQCM const & rhs )
{

	std::cout << "Assignment operator Called" << std::endl;

	//	if ( this != &rhs )
	//	{
	//	 = rhs.get();
	//	}
	(void)rhs;
	return (*this);
}

std::ostream & operator<<( std::ostream & o, aureleQCM const & i )
{

	//	o << i.get()
	(void)i;
	return (o);

}
