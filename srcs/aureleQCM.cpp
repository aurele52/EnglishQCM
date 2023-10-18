#include "../headers/aureleQCM.hpp"
#include "../headers/main.hpp"
#include "../headers/picojson.h"
#include "../config.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <sys/wait.h>
#include <dirent.h>
#include <stdio.h>
#include <curl/curl.h>
	

using namespace std;

enum setting_type {
    CLEAR,
    KEY,
    DEFAULT_LANG
};

typedef struct {
    string key;
    string default_lang;
} settings_struct;

bool setup_curl(CURL **curl) {
    *curl = curl_easy_init();

    if (curl == nullptr) {
        cerr << "Error: failed to set up curl" << endl;
        return false;
    }

    return true;
}

void cleanup_curl(CURL **curl) {
    cout << "\r";

    curl_easy_cleanup(*curl);
}

size_t curl_on_receive(char *ptr, size_t size, size_t nmemb, void *stream) {
    vector<char> *recv_buffer = (vector<char>*)stream;
    const size_t sizes = size * nmemb;
    recv_buffer->insert(recv_buffer->end(), (char*)ptr, (char*)ptr + sizes);
    return sizes;
}

bool connect_curl(CURL **curl, string url, string post_data, string &res_string) {
    vector<char> res_data;
    const char *post_data_c = post_data.c_str();
    const char *url_c = url.c_str();

    curl_easy_setopt(*curl, CURLOPT_URL, url_c);
    curl_easy_setopt(*curl, CURLOPT_POST, 1);
    curl_easy_setopt(*curl, CURLOPT_POSTFIELDS, post_data_c);
    curl_easy_setopt(*curl, CURLOPT_POSTFIELDSIZE, strlen(post_data_c));
    curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYPEER, 1);
    curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, curl_on_receive);
    curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &res_data);

    CURLcode res = curl_easy_perform(*curl);
    if (res != CURLE_OK) {
        cerr << "Error: curl_easy_perform failed: " << curl_easy_strerror(res) << endl;
        cleanup_curl(curl);
        return false;
    }

    cleanup_curl(curl);

    res_string = string(res_data.data());

    return true;
}

int aureleQCM::translate(string &eng, string &fr)
{
	std::string	source_lang_code = "EN";
	std::string	target_lang_code = "FR";

	std::string str;
	if (eng == "fr")
	{
		str = fr;
		eng = "";
		source_lang_code = "FR";
		target_lang_code = "EN";
	}
	else
	{
		str = eng;
		source_lang_code = "EN";
		target_lang_code = "FR";
	}
    CURL *curl;

    if (!setup_curl(&curl)) {
        cleanup_curl(&curl);
        return 1;
    }
	string get_data;
	string post_data;

	post_data = "";
	post_data = post_data + "auth_key=" + API_KEY + "&text=" + str + "&target_lang=" + target_lang_code;
	if (source_lang_code != "")
		post_data += "&source_lang=" + source_lang_code;
	std::cout << "waiting..." << flush;
	if (!connect_curl(&curl, "https://api-free.deepl.com/v2/translate", post_data, get_data)) {
		cout << "\r" << string(8, ' ');
		cout << "\r";
		cerr << "Error" << endl;
		cleanup_curl(&curl);
		return false;
	}
	cout << "\r" << string(8, ' ');
	cout << "\r";
	cout << get_data << std::endl;
	picojson::value v;
	string err = picojson::parse(v, get_data);
	if (!err.empty()) {
		cerr << "Error: picojson error - " << err << endl;
		return false;
	}

	std::string translated_text = "";
	picojson::object& v_obj = v.get<picojson::object>();
	picojson::array& v_array = v_obj["translations"].get<picojson::array>();
	for (auto v_element : v_array) {
		picojson::object& v_element_obj = v_element.get<picojson::object>();
		translated_text += v_element_obj["text"].get<string>();
	}
	if (eng.empty())
		eng = translated_text;
	else
		fr = translated_text;
	return 0;
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
					std::cout << "level + 1" << std::endl;
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
