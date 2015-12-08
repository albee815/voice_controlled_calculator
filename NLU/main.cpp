#include<iostream>
#include <fstream>
#include"Tokenizer.h"
#include"Translator.h"
#include"Calculator.h"

using namespace std;

int main(int argc, char** argv){
	string input;
	string errorInfo;
	string valid;
	string res;
	ifstream inf (argv[1]);
	if(inf.is_open()){
		getline(inf,input);
	}
	else{
		cout<<"unable to open file"<<endl;
		return -1;
	}
	inf.close();
	Tokenizer tok;
	Translator tra;
	Calculator calc;
	vector<int> result = tok.token(input);
	Expression expression = tra.translate(result);
	if(tra.isProblem()){
		valid = "false";
		errorInfo = "translator can not understand the utterance";
	}
	else if(!calc.validate(expression.vD)){
		valid = "false";
		errorInfo = "calculator can not understand the expression";
	}
	else{
		double ans = calc.cal(expression.vD);
		if(calc.dividedByZero){
			valid = "false";
			errorInfo = "divided by zero detected";
		}
		else{
			valid = "true";
			stringstream ss;
			ss<<ans;
			ss>>res;
		}
	}

	ofstream outf (argv[2]);
	if(outf.is_open()){
		outf<<"{\"valid\":"+valid;
		if(expression.exp.length()>0)
			outf<<",\"expression\":\""+expression.exp+"\"";
		if(errorInfo.length()>0)
			outf<<",\"errorInfo\":\""+errorInfo+"\"";
		if(res.length()>0)
			outf<<",\"result\":"+res+"}";
		else
			outf<<"}";
	}
	else{
		cout<<"unable to open file"<<endl;
		return -1;
	}

	outf.close();
	return 0;
}
