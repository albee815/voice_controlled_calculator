#pragma once
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

class Tokenizer{
public:
	Tokenizer();
	vector<int> token(string utterance);
private:
	unordered_map<string,int> map;
};

Tokenizer::Tokenizer(){
	//numbers
	map["zero"] = 0;map["one"] = 1;map["two"] = 2;map["three"] = 3;map["four"] = 4;map["five"] = 5;
	map["six"] = 6;map["seven"] = 7;map["eight"] = 8;map["nine"] = 9;map["ten"] = 10;map["eleven"] = 11;
	map["twelve"] = 12;map["thirteen"] = 13;map["fourteen"] = 14;map["fifteen"] = 15;map["sixteen"] = 16;
	map["seventeen"] = 17;map["eighteen"] = 18;map["nineteen"] = 19;map["twenty"] = 20;map["thirty"] = 30;
	map["forty"] = 40;map["fifty"] = 50;map["sixty"] = 60;map["seventy"] = 70;map["eighty"] = 80;map["ninety"] = 90;
	//other tokens
	map["and"] = -1;map["hundred"] = -2;map["point"] = -3;map["product"] = -4;map["multiplied"] = -5;map["multiply"] = -5;
	map["times"] = -6;map["add"] = -7;map["sum"] = -8;map["plus"] = -9;map["minus"] = -10;map["substract"] = -11;map["divide"] = -12;
	map["divided"] = -12;map["after"] = -13;map["then"] = -14;map["result"] = -15;map["answer"] = -16;
}

vector<int> Tokenizer::token(string utterance){
	vector<int> result;
	int begin = 0;
	for(int i=0;i<=utterance.length();i++){
		if(i==utterance.length()||utterance[i]==' '){
			unordered_map<string,int>::iterator it = map.find(utterance.substr(begin,i-begin));
			if(it!=map.end()){
				result.push_back(it->second);
			}
			begin = i+1;
		}
	}
	return result;
}