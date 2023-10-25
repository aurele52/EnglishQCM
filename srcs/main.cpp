#include "../headers/main.hpp"
#include <boost/algorithm/string/predicate.hpp>


int	main(int argc, char** argv)
{
	aureleQCM	qcm;

	std::string test;
	std::string eng;
	std::string fr;
	std::string hint;
	std::string number;

	while (test != "EXIT")
	{
		std::cout << "Enter an instruction among ADD, START and EXIT" << std::endl;
		std::getline(std::cin, test);
		if (std::cin.eof())
			return (0);
		if (test == "ADD")
		{
			while (eng.length() == 0)
			{
				std::cout << "eng:";
				std::getline(std::cin, eng);
				if (std::cin.eof())
					return (0);
			}
			if (eng == "fr")
			{
				while (fr.length() == 0)
				{
					std::cout << "fr: ";
					std::getline(std::cin, fr);
					if (std::cin.eof())
						return (0);
				}
			}
			qcm.translate(eng, fr);
			std::string rep;
			while (rep.length() == 0)
			{
				std::cout << "The french traduction of " << eng << "is " << fr << std::endl;
				std::getline(std::cin, rep);
				if (std::cin.eof())
					return (0);
			}
			if (boost::iequals(rep, "n") || boost::iequals(rep, "non"))
			{
				while (eng.length() == 0)
				{
					std::cout << "eng:";
					std::getline(std::cin, eng);
					if (std::cin.eof())
						return (0);
				}
				while (fr.length() == 0)
				{
					std::cout << "fr:";
					std::getline(std::cin, fr);
					if (std::cin.eof())
						return (0);
				}
			}
			while (hint.length() == 0)
			{
				std::cout << "hint:";
				std::getline(std::cin, hint);
				if (std::cin.eof())
					return (0);
			}
			qcm.addWord(eng, fr, hint);
			eng.clear();
			fr.clear();
			hint.clear();
		}
		else if (test == "START")
		{
			number.clear();
			while (number.length() == 0)
			{
				std::cout << "Level:";
				std::getline(std::cin, number);
				if (std::cin.eof())
					return (0);
			}
			qcm.start(number[0] - '0');
		}
		else if (test != "EXIT")
			std::cout << "instruction must be ADD or START or EXIT" << std::endl;
	}
}
