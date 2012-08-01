
#include <libcdsBasics.h>
#include <cppUtils.h>

namespace cds_utils
{

	uint transform(const string & s) {
		stringstream ss;
		ss << s;
		uint ret;
		ss >> ret;
		return ret;
	}

	void tokenize(string str, vector<string> &tokens, char delim) {
		string::size_type last_pos = 0;
		string::size_type pos = str.find_first_of(delim);
		while(pos!=string::npos) {
			tokens.push_back(str.substr(last_pos,pos-last_pos));
			last_pos = pos+1;
			if(last_pos >= str.length()) break;
			pos = str.find_first_of(delim,pos+1);
		}
		if(last_pos<str.length())
			tokens.push_back(str.substr(last_pos));
	}

};
