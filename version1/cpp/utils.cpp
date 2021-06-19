#include"../include/utils.h"





std::string & util::ltrim(std::string & str){
		if(str.empty())
				return str;

		str.erase(0, str.find_first_not_of(" \t"));
		return str;

}

std::string &util::rtrim(std::string & str){
		if(str.empty())
				return str;

		str.erase(str.find_last_not_of(" \t") +1);

		return str;
}


std::string& util::trim(std::string&str){
		if(str.empty())
				return str;

		utile::ltrim(str);
		utile::rtrim(str);
		return str;
}


