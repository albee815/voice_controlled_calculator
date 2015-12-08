#pragma once
#include<vector>
#include<string>
#include"define.h"

using namespace std;

class Calculator{
public:
	Calculator(){dividedByZero = false;};
	double cal(vector<double>& vD);
	bool validate(vector<double>& vD);
	bool dividedByZero;
};

bool Calculator::validate(vector<double>& vD){
	vector<double> temp;
	for(int i=0;i<vD.size();i++){
		if(vD[i]!=BLANK)
			temp.push_back(vD[i]);
	}
	for(int i=0;i<temp.size();i++){
		if(temp[i]>=0){
			if(i>0&&(temp[i-1]==RIGHTSIGN||temp[i-1]>=0))
				return false;
			if(i<temp.size()-1&&(temp[i+1]==LEFTSIGN||temp[i+1]>=0))
				return false;
		}
		else if(temp[i]==PLUSSIGN||temp[i]==MINUSSIGN||temp[i]==TIMESSIGN||temp[i]==DIVIDESIGN){
			if(i>0&&temp[i-1]<0&&temp[i-1]!=RIGHTSIGN)
				return false;
			if(i<temp.size()-1&&temp[i+1]<0&&temp[i+1]!=LEFTSIGN)
				return false;
		}
		else if(i<temp.size()-1&&temp[i]==RIGHTSIGN&&temp[i+1]==LEFTSIGN)
			return false;
	}
	return true;
}

double Calculator::cal(vector<double>& vD){
	//mid-order to post-order
	vector<double> stack;
	vector<double> post;
	for(int i=0;i<vD.size();i++){
		if(vD[i]>=0)
			post.push_back(vD[i]);
		else if(vD[i]==LEFTSIGN)
			stack.push_back(LEFTSIGN);
		else if(vD[i]==RIGHTSIGN){
			while(stack.back()!=LEFTSIGN){
				post.push_back(stack.back());
				stack.pop_back();
			}
			stack.pop_back();
		}
		else if(vD[i]==PLUSSIGN||vD[i]==MINUSSIGN){
			while(!stack.empty()&&stack.back()!=LEFTSIGN){
				post.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(vD[i]);
		}
		else if(vD[i]==TIMESSIGN||vD[i]==DIVIDESIGN){
			while(!stack.empty()&&stack.back()!=LEFTSIGN&&stack.back()!=PLUSSIGN&&stack.back()!=MINUSSIGN){
				post.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(vD[i]);
		}
	}
	while(!stack.empty()){
		post.push_back(stack.back());
		stack.pop_back();
	}

	//compute the post-order expression
	for(int i=0;i<post.size();i++){
		if(post[i]>=0)
			stack.push_back(post[i]);
		else {
			double b = stack.back();
			stack.pop_back();
			double a = stack.back();
			stack.pop_back();
			if(post[i]==PLUSSIGN)
				stack.push_back(a+b);
			if(post[i]==MINUSSIGN)
				stack.push_back(a-b);
			if(post[i]==TIMESSIGN)
				stack.push_back(a*b);
			if(post[i]==DIVIDESIGN){
				if(b==0){
					dividedByZero = true;
					return 0;
				}
				stack.push_back(a/b);
			}
		}
	}
	return stack[0];
}