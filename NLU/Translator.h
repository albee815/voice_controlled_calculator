#pragma once
#include<vector>
#include<string>
#include<sstream>
#include"define.h"

using namespace std;

struct Expression{
	Expression(string s, vector<double> v){exp=s;vD=v;};
	string exp;
	vector<double> vD;
};

class Translator{
public:
	Translator();
	Expression translate(vector<int>& input);
	bool isProblem();
private:
	bool problem;
	bool finished(vector<double>& input);
	bool identical(vector<double>& temp, vector<double>& vD);
};

Translator::Translator(){
	problem = false;
}

Expression Translator::translate(vector<int>& input){
	string exp;
	//step one: interpret numbers
	vector<double> vD;
	int i = 0;
	bool flag = false;
	while(i<input.size()){
		if(input[i]<-3){
			vD.push_back(input[i]);
			i++;
		}
		else{
			double num = input[i++];
			if(num<0){
				problem = true;
				vD.clear();
				return Expression(exp,vD);
			}
			double factor = 0.1;
			bool decimal = false;
			while(i<input.size()&&input[i]>=-3){
				if(input[i]==HUNDRED)
					num *= 100;
				else if(input[i]==POINT)
					decimal = true;
				else if(input[i]==AND){
					if(input[i-1]!=HUNDRED){
						i++;
						flag = true;
						break;
					}
				}
				else{
					if(!decimal){
						if(input[i-1]>=0&&input[i-1]<20){
							problem = true;
							vD.clear();
							return Expression(exp,vD);
						}
						if(input[i-1]>=20&&(input[i]==0||input[i]>=10)){
							problem = true;
							vD.clear();
							return Expression(exp,vD);
						}
						num += input[i];
					}
					else{
						num += input[i]*factor;
						factor /= 10;
					}
				}
				i++;
			}
			vD.push_back(num);
			if(flag){
				vD.push_back(AND);
				flag = false;
			}
		}
	}

	//debug: show the result of step 1
	/*for(int i=0;i<vD.size();i++)
		cout<<vD[i]<<" ";
	cout<<endl;*/
	
	if(vD.size()<1){
		problem = true;
		return Expression(exp,vD);
	}

	//step 2: interpret ordered partterns: plus, times, multipied, substract,minus and divide
	for(int i=1;i<vD.size()-1;i++){
		if(vD[i]==PLUS) vD[i] = PLUSSIGN;
		else if(vD[i]==TIMES||vD[i]==MULTIPLIED) vD[i] = TIMESSIGN;
		else if(vD[i]==SUBSTRACT||vD[i]==MINUS) vD[i] = MINUSSIGN;
		else if(vD[i]==DIVIDED) vD[i] = DIVIDESIGN;
	}

	//step 3: interpret order logical patterns: then, after, result of, answer of
	for(int i=0;i<vD.size();i++){
		if(vD[i]==THEN||vD[i]==AFTER){
				vD[i] = RIGHTSIGN;
				vector<double>::iterator it = vD.begin();
				vD.insert(it,LEFTSIGN);
		}
	}
	for(int i=0;i<vD.size();i++){
		if(vD[i]==RESULT||vD[i]==ANSWER){
			vD[i] = LEFTSIGN;
			int j = i+1;
			bool found = false;
			for(;j<vD.size();j++){
				if(vD[j]==AND||vD[j]==RIGHTSIGN){
					vector<double>::iterator it = vD.begin();
					vD.insert(it+j,RIGHTSIGN);
					found = true;
					break;
				}
			}
			if(!found){
				vD.push_back(RIGHTSIGN);
			}
		}
	}
	

	//step 4: interpret reordered patterns: add, sum and product
	while(!finished(vD)){
		vector<double> temp;
		for(int i=0;i<vD.size();i++)
			temp.push_back(vD[i]);
		for(int i=0;i<vD.size();i++){
			if(vD[i]==SUM||vD[i]==ADD||vD[i]==PRODUCT){
				int j=i+1;
				int insideBracket = 0;
				vector<int> andPos;
				bool next = false;
				int foundFarthest = -1;
				for(;j<=vD.size();j++){
					if(insideBracket<=0&&andPos.size()>0&&(j==vD.size()||vD[j]==PLUSSIGN||vD[j]==TIMESSIGN||vD[j]==MINUSSIGN||vD[j]==DIVIDESIGN)){
						foundFarthest = j;
						if(foundFarthest==vD.size()||insideBracket<0)
							break;
						else{
							int countANDLeft = 0;
							for(int k=j;k<vD.size();k++)
								if(vD[k]==AND)
									countANDLeft++;
							if(countANDLeft>0)
								continue;
							else
								break;
						}
					}
					if(j==vD.size()){
						problem = true;
						vD.clear();
						return Expression(exp,vD);
					}
					if(vD[j]==SUM||vD[j]==ADD||vD[j]==PRODUCT){
						break;
					}
					if(vD[j]==AND)
						andPos.push_back(j);
					if(vD[j]==LEFTSIGN)
						insideBracket++;
					if(vD[j]==RIGHTSIGN)
						insideBracket--;
				}
				if(foundFarthest>=0){
					int replace = vD[i]==PRODUCT ? TIMESSIGN : PLUSSIGN;
					vD[i] = LEFTSIGN;
					for(int k=0;k<andPos.size();k++)
						vD[andPos[k]] = replace;
					if(foundFarthest==vD.size()){
						vD.push_back(RIGHTSIGN);
					}
					else{
						vector<double>::iterator it = vD.begin();
						vD.insert(it+foundFarthest,RIGHTSIGN);
					}
					//if deal with PRODUCT, some additional brackets are needed
					if(replace==TIMESSIGN){
						int leftPlace = i;
						for(int k=0;k<andPos.size();k++){
							vector<double>::iterator it = vD.begin();
							vD.insert(it+leftPlace,LEFTSIGN);
							it = vD.begin();
							vD.insert(it+andPos[k]+1+k*2,RIGHTSIGN);
							leftPlace = andPos[k]+3+k*2;
						}
					}
					next = true;
				}
				if(next)
					break;
			}
		}
		if(identical(temp,vD)){
			problem = true;
			vD.clear();
			return Expression(exp,vD);
		}
	}

	//step 5: simplify the expression, remove the repeated brackets
	vector<int> stack;
	vector<int> pos;
	for(int i=0;i<vD.size();i++){
		if(vD[i]==LEFTSIGN){
			stack.push_back(i);
			pos.push_back(-2);
		}
		else if(vD[i]==RIGHTSIGN){
			int p = stack.back();
			stack.pop_back();
			pos.push_back(p);
		}
		else{
			pos.push_back(-2);
		}
	}
	for(int i=1;i<pos.size();i++){
		if(pos[i]+1==pos[i-1]){
			vD[i] = BLANK;
			vD[pos[i]] = BLANK;
		}
	}

	//step 6: translate the expression into human readable form:
	for(int i=0;i<vD.size();i++){
		stringstream ss;
		if(vD[i]==PLUSSIGN) exp.append("+");
		else if(vD[i]==MINUSSIGN) exp.append("-");
		else if(vD[i]==TIMESSIGN) exp.append("*");
		else if(vD[i]==DIVIDESIGN) exp.append("/");
		else if(vD[i]==LEFTSIGN && vD[i+2]!=RIGHTSIGN) exp.append("(");
		else if(vD[i]==RIGHTSIGN && vD[i-2]!=LEFTSIGN) exp.append(")");
		else if(vD[i]>=0){
			ss<<vD[i];
			exp.append(ss.str());
			ss.clear();
			ss.str();
		}
	}

	//debug: output the expression
	//cout<<exp<<endl;

	return Expression(exp,vD);
}

bool Translator::finished(vector<double>& input){
	for(int i=0;i<input.size();i++)
		if(input[i]<0&&input[i]>-100)
			return false;
	return true;
}

bool Translator::identical(vector<double>& temp, vector<double>& vD){
	if(temp.size()!=vD.size())
		return false;
	for(int i=0;i<vD.size();i++){
		if(vD[i]!=temp[i])
			return false;
	}
	return true;
}

bool Translator::isProblem(){
	return problem;
}