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

bool ContextFreeGrammar::isTerminalStr(const std::string &tempStr) const{
    if (terminalStr.find(tempStr) == terminalStr.end()) {
        return false;
    }
    return true;
}

void ContextFreeGrammar::clearLeftRecursion() {
	std::string clrProduction;
	std::string a_temp; 
	std::vector<std::string> a,b;  //用于消除左递归的中间变量

	int i;
	bool flog; //用来判断当前非终极符是否存在左递归的情况，true表示存在，否则不存在

	for (auto ptr = nterminalStr.cbegin(); ptr!=nterminalStr.cend(); ++ptr) {
		flog=false;
        for (auto ptr1 = production.at(*ptr).begin(); ptr1 != production.at(*ptr).end();++ptr1) {
			if((*ptr)[0] == (*ptr1)[0]){
				flog=true;
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

		//消除左递归
		if(flog) {
			production.at(*ptr).clear();
			for(auto ptr2 = b.begin(); ptr2 != b.end();++ptr2) {
				(*ptr2).push_back((*ptr)[0]);
				(*ptr2).push_back('\'');
				production[*ptr].push_back(*ptr2);
			}

			clrProduction.push_back((*ptr)[0]);
			clrProduction.push_back('\'');
			clrProduction.push_back('-');
			clrProduction.push_back('>');
			for(auto ptr2 = a.begin(); ptr2 != a.end();++ptr2) {
				i=0;
				while(i<(*ptr2).length()){
					clrProduction.push_back((*ptr2)[i]);
					i++;
				}
				clrProduction.push_back((*ptr)[0]);
				clrProduction.push_back('\'');
				clrProduction.push_back('|');
			}
			clrProduction.push_back('$');
			std::cout<<clrProduction<<std::endl;
			getLine(clrProduction);
			clrProduction.clear();
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
        
    int i = 0;
    
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
			if (rawProduction[i]!='\'') {//handle char(')
				if (rawProduction[i+1] == '\'') {
					mterminalStr.push_back(rawProduction[i]);
					mterminalStr.push_back(rawProduction[i+1]);
				}
				else {
					mterminalStr.push_back(rawProduction[i]);
				}
                    
				if (nterminalStr.find(mterminalStr)==nterminalStr.end()){
					addTerminalStr(mterminalStr);
				}
				mterminalStr.clear();
			}
			rhProduction.push_back(rawProduction[i++]);
		}
	}
    
	production[lhProduction].push_back(rhProduction); //push the last production
	lhProduction.clear();
    rawProduction.clear();
    
}
