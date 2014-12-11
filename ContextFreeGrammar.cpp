//
//  ContextFreeGrammar.cpp
//  CompilerTask
//
//  Created by lms on 11/13/14.
//  Copyright (c) 2014 lms. All rights reserved.
//

#include <fstream>
#include <algorithm>
#include "ContextFreeGrammar.h"

void ContextFreeGrammar::addTerminalStr(const std::string &newTerminalStr){
    terminalStr.insert(newTerminalStr);
}

void ContextFreeGrammar::addNTerminalStr(const std::string &newNTerminaStr){
    nterminalStr.insert(newNTerminaStr);
}

std::unordered_set<std::string> ContextFreeGrammar::getProductionRhStr(const std::string &lhStr) const{
    std::unordered_set<std::string> result;

    for (auto ptr = production.at(lhStr).begin(); ptr != production.at(lhStr).end();++ptr) {
        result.insert(*ptr);
    }
    return  result;
}

std::unordered_set<std::string> ContextFreeGrammar::getFirstSet(const std::string &lhStr) const{
	std::unordered_set<std::string> FIRST;
	return FIRST;
}

std::unordered_set<std::string> ContextFreeGrammar::getFollowSet(const std::string &lhStr) const{
	std::unordered_set<std::string> FOLLOW;
	return FOLLOW;
}

bool ContextFreeGrammar::isTerminalStr(const std::string &tempStr) const{
    if (terminalStr.find(tempStr) == terminalStr.end()) {
        return false;
    }
    return true;
}

bool ContextFreeGrammar::isLeftRecursion(const std::string &lhProduction,const std::string &rhProduction) const{
	int i=0;
	while(i<lhProduction.length()){
		if(lhProduction[i] == rhProduction[i])
			i++;
		else
			return false;
	}
	if(i==lhProduction.length())
	{
		if( (rhProduction[i]!='\'') && (rhProduction[i]!='^') ) 
			return true;
		else
			return false;
	}
	else
		return false;
}

void ContextFreeGrammar::clearDirectLeftRecursion(const std::string &lhStr) {
	std::string clrProduction;
	std::string a_temp; 
	std::vector<std::string> a,b;  //用于消除左递归的中间变量

	int i;
	bool flog; //用来判断当前非终极符是否存在左递归的情况，true表示存在，否则不存在
	
	flog=false;
    for (auto ptr1 = production.at(lhStr).begin(); ptr1 != production.at(lhStr).end();++ptr1) {
		if(isLeftRecursion(lhStr,*ptr1)){
 			flog=true;
			i=(lhStr).length();
			while(i<(*ptr1).length()){
				a_temp.push_back((*ptr1)[i]);
				i++;
			}
			a.push_back(a_temp);
			a_temp.clear();
		}
		else{
			b.push_back(*ptr1);
		}	
	}

	//消除左递归
	if(flog) {
		production.at(lhStr).clear();
		for(auto ptr2 = b.begin(); ptr2 != b.end();++ptr2) {
			i=0;
			while(i<(lhStr).length()){
				(*ptr2).push_back((lhStr)[i]);
				i++;
			}
			(*ptr2).push_back('\'');
			production[lhStr].push_back(*ptr2);
		}

		i=0;
		while(i<(lhStr).length()){
			clrProduction.push_back((lhStr)[i]);
			i++;
		}
		clrProduction.push_back('\'');
		clrProduction.push_back('-');
		clrProduction.push_back('>');
		for(auto ptr2 = a.begin(); ptr2 != a.end();++ptr2) {
			i=0;
			while(i<(*ptr2).length()){
				clrProduction.push_back((*ptr2)[i]);
				i++;
			}
			i=0;
			while(i<(lhStr).length()){
				clrProduction.push_back((lhStr)[i]);
				i++;
			}
			clrProduction.push_back('\'');
			clrProduction.push_back('|');
		}
		clrProduction.push_back('$');
		getLine(clrProduction);
		clrProduction.clear();
	}
		
	a.clear();
	b.clear();
}

void ContextFreeGrammar::clearAllLeftRecuision(){
	int i,j;
	bool flog;
	std::string mterminalStr;//just one terminal or nterminal
	std::string tempStr;
	std::string a;//存储产生式右侧项中将要被替换的非终极符的前部分
	std::string b;//存储产生式右侧项中将要被替换的非终极符的后部分


	std::vector<std::string> tempVector;//用于存储被替换后的项

	for (auto ptri = nterminalStr.cbegin(); ptri!=nterminalStr.cend(); ++ptri) {//遍历所以文法中的非终结符
		
		for (auto ptrj = nterminalStr.cbegin(); ptrj!= ptri; ++ptrj){//遍历当前终结符前的所有非终极符
			flog=true;
			while (flog){
				flog=false;
				//遍历ptri指向非终极符的产生式右侧每一项，用ptrj指向非终极符的产生式右侧每一项进行代入操作
				for (auto ptr =production.at(*ptri).begin();ptr!=production.at(*ptri).end();++ptr){
					i=0;j=0;
					while(i<(*ptr).length()){
						if( ((*ptr)[i]!='\'')&&((*ptr)[i]!='^') ){//在ptri对应非终极符的产生式右侧寻找可替代项
							while( ((*ptr)[i+1]=='\'')||((*ptr)[i+1]=='^') ){
								mterminalStr.push_back((*ptr)[i]);
								i++;j++;
							}
							mterminalStr.push_back((*ptr)[i]);		
							for(auto k =0;k<i-j;++k){
								a.push_back((*ptr)[k]);
							}
							for(auto k=i+1;k<(*ptr).length();++k){
								b.push_back((*ptr)[k]);
							}
						}
						if(mterminalStr == *ptrj){//找到可替代项，进行替代
							production.at(*ptri).erase(ptr);
							for(auto ptr1 =production.at(*ptrj).begin();ptr1!=production.at(*ptrj).end();++ptr1){
								for(auto k =0;k<a.length();++k){//把前部分压入临时字符串
									tempStr.push_back(a[k]);
								}
								for(auto k =0;k<(*ptr1).length();++k){//把要替换非终极符的项压入临时字符串
									tempStr.push_back((*ptr1)[k]);
								}
								for(auto k =0;k<b.length();++k){//把后部分压入临时字符串
									tempStr.push_back(b[k]);
								}
								tempVector.push_back(tempStr);//把替换完成后的临时字符串压入被替换的产生式作为一个项
								flog=true;
								tempStr.clear();
							}
							break;
						}
						i++;
						mterminalStr.clear();
						a.clear();
						b.clear();
					}
					if(flog)
						break;
				}
				for(auto ptr=tempVector.begin();ptr!=tempVector.end();++ptr){
					production.at(*ptri).push_back(*ptr);
				}
				tempVector.clear();
			}
			clearDirectLeftRecursion(*ptri);
		}
	}
}

void ContextFreeGrammar::pickPublicLeftFactor(){
	char publicLeftFactor;
	std::string pplfProduction;
	std::string a_temp; 
	std::vector<std::string> a,b;  //用于消除左递归的中间变量

	int i;
	int count; //用来记录总共有几个项含有公共左因子

	for (auto ptr = nterminalStr.cbegin(); ptr!=nterminalStr.cend(); ++ptr) {
		count=0;
		publicLeftFactor=(*production.at(*ptr).begin())[0];
        for (auto ptr1 = production.at(*ptr).begin(); ptr1 != production.at(*ptr).end();++ptr1) {
			if( (*ptr1)[0] == publicLeftFactor ){
				count++;
				i=1;
				while(i<(*ptr1).length()){
					a_temp.push_back((*ptr1)[i]);
					i++;
				}
				a.push_back(a_temp);
				a_temp.clear();
			}
			else{
				b.push_back(*ptr1);
			}
			
		}

		//提取公共左因子
		if(count>1) {
			production.at(*ptr).clear();
			a_temp.push_back(publicLeftFactor);
			i=0;
			while(i<(*ptr).length()){
				a_temp.push_back((*ptr)[i]);
				i++;
			}
			a_temp.push_back('^');
			production.at(*ptr).push_back(a_temp);
			for(auto ptr2 = b.begin(); ptr2 != b.end();++ptr2) {
				production[*ptr].push_back(*ptr2);
			}

			i=0;
			while(i<(*ptr).length()){
				pplfProduction.push_back((*ptr)[i]);
				i++;
			}
			pplfProduction.push_back('^');
			pplfProduction.push_back('-');
			pplfProduction.push_back('>');
			for(auto ptr2 = a.begin(); ptr2 != a.end();++ptr2) {
				i=0;
				while(i<(*ptr2).length()){
					pplfProduction.push_back((*ptr2)[i]);
					i++;
				}
				if(ptr2+1!=a.end())
				{
					pplfProduction.push_back('|');
				}
			}
			getLine(pplfProduction);
			pplfProduction.clear();
		}
		
		a.clear();
		b.clear();
    }
}

void ContextFreeGrammar::getFileLine(const std::string &fileName){
    std::fstream input;
    input.open(fileName);
    
    if (input.fail()) {
        std::cout << "Open File Error!" << std::endl;
        input.close();
        return;
    }
    
    while (!input.eof()) {
        std::string rawProduction;  //raw line in the file       
            
        std::getline(input, rawProduction, '\n');
		getLine(rawProduction);
    }
    
    input.close();
}

void ContextFreeGrammar::getLine(const std::string &raw_Production){
	std::string rawProduction;  //raw line in the file
	std::string rhProduction;
    std::string lhProduction; //left hand of the production
    std::string mterminalStr; //just one terminal
    
	rawProduction=raw_Production;
        
    //erase space
    rawProduction.erase(std::remove_if(rawProduction.begin(),
                            rawProduction.end(), ::isspace), rawProduction.end());
    
	//std::cout<<"haha"<<rawProduction<<std::endl;
    int i = 0;
	int j;
	while (rawProduction[i]!='-') {
		lhProduction.push_back(rawProduction[i++]);
	}
        
	//remove the string initial in terminalStr
	if (terminalStr.find(lhProduction)!=terminalStr.end()) {
		terminalStr.erase(lhProduction);
	}
	addNTerminalStr(lhProduction);
	i+=2;    //skip '-' and '>'
        
	while (i<rawProduction.length()) {
		if (rawProduction[i] == '|') {
			production[lhProduction].push_back(rhProduction);
			rhProduction.clear();
			i++;
		}
		else{
			j=0;
			if ((rawProduction[i]!='\'')&&(rawProduction[i]!='^')) {//handle char(')and(^)
                while ((rawProduction[i+1] == '\'')||(rawProduction[i+1] == '^')) {
					mterminalStr.push_back(rawProduction[i]);
					i++;j++;
				}
				mterminalStr.push_back(rawProduction[i]);


				if (nterminalStr.find(mterminalStr)==nterminalStr.end()){
					addTerminalStr(mterminalStr);
				}
				mterminalStr.clear();
			}

			i=i-j;//进行回溯，让i回到出来'和^之前的位置
			while(j>=0){
				rhProduction.push_back(rawProduction[i++]);
				j--;
			}
		}
	}
    
	production[lhProduction].push_back(rhProduction); //push the last production
	lhProduction.clear();
    rawProduction.clear();
    
}
