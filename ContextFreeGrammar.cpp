//
//  ContextFreeGrammar.cpp
//  CompilerTask
//
//  Created by lms on 11/13/14.
//  Copyright (c) 2014 lms. All rights reserved.
//

#include <iomanip>
#include <fstream>
#include <algorithm>
#include <stack>
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

void ContextFreeGrammar::calFirstSet(){
	std::string mterminalStr;
	int i;
	bool isBigger=true; //判断遍历一遍所有产生式后各个非终极符的First集有没有变大，有则值为true，否则值为false

	for(auto ptr=terminalStr.cbegin();ptr!=terminalStr.cend();++ptr){//所有终结符的first集为本身
		first[*ptr].insert(*ptr);
	}

	while(isBigger){//循环利用求FIRST集的规则直至每个FIRST集都不再变大为止
		isBigger=false;
		for(auto ptri=nterminalStr.cbegin();ptri!=nterminalStr.cend();++ptri){
			for(auto ptrj=production.at(*ptri).cbegin();ptrj!=production.at(*ptri).cend();++ptrj){
				i=0;
		      	while( ((*ptrj)[i+1]=='\'')||((*ptrj)[i+1]=='^') ){
				    mterminalStr.push_back((*ptrj)[i]);
			    	i++;
		     	}
		    	mterminalStr.push_back((*ptrj)[i++]);	

		    	if(terminalStr.find(mterminalStr)!=terminalStr.end()){//如果右侧项中第一个为终结符
					if(first[*ptri].find(mterminalStr)==first[*ptri].end()){
						first[*ptri].insert(mterminalStr);
						isBigger=true;
					}
		    	}
		    	else if(nterminalStr.find(mterminalStr)!=nterminalStr.end()){//如果右侧项中第一个为非终极符
		     		for(auto ptr=first[mterminalStr].cbegin();ptr!=first[mterminalStr].cend();++ptr){
			    		if( ((*ptr)[0]!='$')&&(first[*ptri].find(*ptr)==first[*ptri].end()) ){
			    			first[*ptri].insert(*ptr);
							isBigger=true;
			    		}
			    	}

					while( (i<(*ptrj).length())&&isContain$(mterminalStr) ){//处理项中前面都是非终极符且都能推出$的情况
						if(i<(*ptrj).length()){
					    	mterminalStr.clear();
				        	while( ((*ptrj)[i+1]=='\'')||((*ptrj)[i+1]=='^') ){
			             	    mterminalStr.push_back((*ptrj)[i]);
			                	i++;
		                	}
		                	mterminalStr.push_back((*ptrj)[i++]);
    
					    	if(nterminalStr.find(mterminalStr)!=nterminalStr.end()){
					     		for(auto ptr=first[mterminalStr].cbegin();ptr!=first[mterminalStr].cend();++ptr){
			    	            	if( ((*ptr)[0]!='$')&&(first[*ptri].find(*ptr)==first[*ptri].end()) ){
			    	            		first[*ptri].insert(*ptr);
					            		isBigger=true;
			    	            	}
			                 	}
				    		}
						}
		    		}
					if( (i=(*ptrj).length())&&(first[*ptri].find("$")==first[*ptri].end()) ){
						first[*ptri].insert("$");
						isBigger=true;
					}

			    }
				mterminalStr.clear();
	    	}		
    	}
	}
}

void ContextFreeGrammar::calFollowSet(){
	int i;
	bool isBigger=true;//判断遍历一遍所有产生式后各个非终极符的FOLLOW集有没有变大，有则值为true，否则值为false
	std::string mterminalStrNow;//用于记录项中当前的处理对象（终结符或非终极符）
	std::string mterminalStrNext;//用于记录项中前一个处理对象（终结符或非终极符）

	follow[getStartStr()].insert("#");//对于文法的开始符，置#于FOLLOW集中
	
	while(isBigger){//循环利用求FOLLOW集的规则直至每个FOLLOW集都不再变大为止
		isBigger=false;
		for(auto ptri=nterminalStr.cbegin();ptri!=nterminalStr.cend();++ptri){
			for(auto ptrj=production.at(*ptri).cbegin();ptrj!=production.at(*ptri).cend();++ptrj){
				if((*ptrj)=="$")
					continue;
				i=0;
				while( ((*ptrj)[i+1]=='\'')||((*ptrj)[i+1]=='^') ){
					mterminalStrNow.push_back((*ptrj)[i]);
					i++;
				}
				mterminalStrNow.push_back((*ptrj)[i]);	//先取出第一个终结符或非终极符放入mterminalStrNow
				i++;
				while(i<(*ptrj).length()){//遍历产生式右侧的一个项
					if(terminalStr.find(mterminalStrNow)!=terminalStr.end()){//如果当前的为终结符，直接跳到下一个
						mterminalStrNow.clear();
						while( ((*ptrj)[i+1]=='\'')||((*ptrj)[i+1]=='^') ){
				         	mterminalStrNow.push_back((*ptrj)[i]);
				         	i++;
						}
			        	mterminalStrNow.push_back((*ptrj)[i]);
						i++;
						continue;
					}

					mterminalStrNext.clear();
					while( ((*ptrj)[i+1]=='\'')||((*ptrj)[i+1]=='^') ){
				       	mterminalStrNext.push_back((*ptrj)[i]);
				       	i++;
			    	}
			       	mterminalStrNext.push_back((*ptrj)[i]);	//取出项中的下一个终结符或非终极符放入mterminalStrNext
					i++;

					for(auto ptr=first[mterminalStrNext].cbegin();ptr!=first[mterminalStrNext].cend();++ptr){
						if( ((*ptr)[0]!='$')&&(follow[mterminalStrNow].find(*ptr)==follow[mterminalStrNow].end()) ){
							follow[mterminalStrNow].insert(*ptr);
							isBigger=true;
						}
					}
					if( (nterminalStr.find(mterminalStrNext)!=nterminalStr.end())&&(isContain$(mterminalStrNext)) ){//如果下一个是非终极符且能推出$
						for (auto ptr=follow[*ptri].cbegin();ptr!=follow[*ptri].cend();++ptr){
					    	if(follow[mterminalStrNow].find(*ptr)==follow[mterminalStrNow].end()){
					    		follow[mterminalStrNow].insert(*ptr);
					    		isBigger=true;
					    	}
						}
					}
					
					mterminalStrNow.clear();
					mterminalStrNow=mterminalStrNext;
				}
				if(nterminalStr.find(mterminalStrNext)!=nterminalStr.end()){
					for (auto ptr=follow[*ptri].cbegin();ptr!=follow[*ptri].cend();++ptr){
						if(follow[mterminalStrNext].find(*ptr)==follow[mterminalStrNext].end()){
							follow[mterminalStrNext].insert(*ptr);
							isBigger=true;
						}
					}
				}
			}
			mterminalStrNow.clear();
			mterminalStrNext.clear();
		}
	}
}

void ContextFreeGrammar::calPredictionTable(){
	int i;
	std::string mterminalStr;
	PredictionTable_coordinate coord;

	for(auto ptri=nterminalStr.cbegin();ptri!=nterminalStr.cend();++ptri){
		for(auto ptrj=terminalStr.cbegin();ptrj!=terminalStr.cend();++ptrj){
			coord.A=*ptri;
			coord.a=*ptrj;
			predictiontable[coord]="error";
		}
		coord.A=*ptri;
		coord.a="#";
		predictiontable[coord]="error";
	}

	for(auto ptri=nterminalStr.cbegin();ptri!=nterminalStr.cend();++ptri){
		for(auto ptrj=production.at(*ptri).cbegin();ptrj!=production.at(*ptri).cend();++ptrj){
			i=0;
		   	while( ((*ptrj)[i+1]=='\'')||((*ptrj)[i+1]=='^') ){
	     	    mterminalStr.push_back((*ptrj)[i]);
			   	i++;
		   	}
		   	mterminalStr.push_back((*ptrj)[i]);	

			for(auto ptr=first[mterminalStr].cbegin();ptr!=first[mterminalStr].cend();++ptr){
				if((*ptr)=="$"){
					for(auto ptrb=follow[*ptri].cbegin();ptrb!=follow[*ptri].cend();++ptrb){
						coord.A=*ptri;
						coord.a=*ptrb;
						predictiontable[coord]=*ptrj;
					}
				}
				else{
					coord.A=*ptri;
					coord.a=*ptr;
					predictiontable[coord]=*ptrj;
				}
			}
			mterminalStr.clear();
		}
	}
}

void ContextFreeGrammar::analysisSentenceByPredictionTable(const std::string &sentence){
	int i=0,j,step=0;
	bool flag,isPro;
	PredictionTable_coordinate coord;
	std::stack<std::string> tempStack;
	std::stack<std::string> STACK;
	std::string tempTop;
	std::string tempStr;
	std::string tempPro;//用于记录分析所用的产生式
	std::string sentenceNow;//当前输入符号
	std::string stackTop;//STACK栈顶元素

	STACK.push("#");
	STACK.push(getStartStr());//首先把#和文法的开始符压入STACK栈

	while( (sentence[i+1]=='\'')||(sentence[i+1]=='^') ){
		sentenceNow.push_back(sentence[i]);
	   	i++;	
	}
	sentenceNow.push_back(sentence[i++]);//把第一个输入符号读进sentenceNow

	std::cout<<std::setw(5)<<std::left<<"step"<<std::setw(15)<<std::left<<"STACK";
	std::cout<<std::setw(10)<<std::right<<"sentence"<<"    "<<std::setw(20)<<std::left<<"used production"<<std::endl;
	std::cout<<"========================================================"<<std::endl;
	flag=true;
	isPro=false;
	while (flag) {
		std::cout<<std::setw(5)<<std::left<<step++;
		
		while(!STACK.empty()){//把STACK中的元素暂时移至tempStack中用于输出STACK栈
			tempTop=STACK.top();
			tempStack.push(tempTop);
			STACK.pop();
		}
		while(!tempStack.empty()){
			tempTop=tempStack.top();
			tempStr=tempStr+tempTop;
			STACK.push(tempTop);
			tempStack.pop();
		}
		std::cout<<std::setw(15)<<std::left<<tempStr;
		tempStr.clear();
		tempStr=tempStr+sentenceNow;
		for(auto k=i;k<sentence.length();++k){
			tempStr=tempStr+sentence[k];
		}
		std::cout<<std::setw(10)<<std::right<<tempStr;
		tempStr.clear();

		if(isPro){
			std::cout<<"    "<<std::setw(20)<<std::left<<tempPro;
		}
		std::cout<<std::endl;

		stackTop=STACK.top();
		coord.A=stackTop;
		coord.a=sentenceNow;
		isPro=false;
		if( terminalStr.find(stackTop)!=terminalStr.end() ){//如果栈顶元素为终结符
			if(stackTop==sentenceNow){//如果栈顶元素和当前输入符号相同，读入下一个输入符号				
				sentenceNow.clear();
				while( (sentence[i+1]=='\'')||(sentence[i+1]=='^') ){
					sentenceNow.push_back(sentence[i]);
					i++;	
				}
				sentenceNow.push_back(sentence[i++]);
				STACK.pop();
			}
			else{
				std::cout<<"error1";
				break;
			}
		}
		else if(stackTop=="#"){
			if(stackTop==sentenceNow){//分析结束
				flag=false;
			}
			else{
				std::cout<<"error2";
				break;
			}
		}
		else if(predictiontable[coord]!="error"){
			tempPro=coord.A+"->"+predictiontable[coord];
			STACK.pop();
			isPro=true;
			if(predictiontable[coord]!="$"){
				j=0;
				while(j<predictiontable[coord].length()){
					while( (predictiontable[coord][j+1]=='\'')||(predictiontable[coord][j+1]=='^') ){
		   		    	tempStr.push_back(predictiontable[coord][j]);
				    	j++;	
			    	}
			    	tempStr.push_back(predictiontable[coord][j++]);
					tempStack.push(tempStr);
					tempStr.clear();
				}
				while(!tempStack.empty()){//把产生式右侧项中的每一个终结符或非终极符倒序压入STACK栈中
					tempTop=tempStack.top();
					STACK.push(tempTop);
					tempStack.pop();
				}
			}
		}
		else{
			std::cout<<"error3";
			break;
		}
	}


}

bool ContextFreeGrammar::isTerminalStr(const std::string &tempStr) const{
    if (terminalStr.find(tempStr) == terminalStr.end()) {
        return false;
    }
    return true;
}

bool ContextFreeGrammar::isContain$(const std::string &lhStr) const{
	bool flag=false;
	for(auto ptr=production.at(lhStr).cbegin();ptr!=production.at(lhStr).cend();++ptr){
		if((*ptr)[0]=='$')
			flag=true;
	}
	return flag;
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
		getLine(clrProduction,0);
		clrProduction.clear();
	}
		
	a.clear();
	b.clear();
}

void ContextFreeGrammar::clearAllLeftRecursion(){
	int i,j;
	bool flog;
	std::string mterminalStr;//just one terminal or nterminal
	std::string tempStr;
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
					//在ptri对应非终极符的产生式右侧寻找可替代项
					while( ((*ptr)[i+1]=='\'')||((*ptr)[i+1]=='^') ){
						mterminalStr.push_back((*ptr)[i]);
						i++;j++;
					}
					mterminalStr.push_back((*ptr)[i]);		
					for(auto k=i+1;k<(*ptr).length();++k){
						b.push_back((*ptr)[k]);
					}
					if(mterminalStr == *ptrj){//找到可替代项，进行替代
						production.at(*ptri).erase(ptr);
						for(auto ptr1 =production.at(*ptrj).begin();ptr1!=production.at(*ptrj).end();++ptr1){
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
					b.clear();
			    	if(flog)
						break;
				}
				for(auto ptr=tempVector.begin();ptr!=tempVector.end();++ptr){
					production.at(*ptri).push_back(*ptr);
				}
				tempVector.clear();
			}
		}
		clearDirectLeftRecursion(*ptri);
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
			getLine(pplfProduction,0);
			pplfProduction.clear();
		}
		
		a.clear();
		b.clear();
    }
}

void ContextFreeGrammar::simplify(){
	std::string mterminalStr;
	int i;
	bool flag;//用于记录当前非终极符是否多余，如果是值为true，否则为false
	bool isDelete=true;//用于记录一次循环中有没有删除非终极符，如果删除了值为true,否则为false

	while(isDelete){
		isDelete=false;
		for (auto ptri= nterminalStr.begin();ptri!=nterminalStr.end();++ptri){
			flag=true;
			for(auto ptrj=nterminalStr.cbegin();ptrj!=nterminalStr.cend();++ptrj){
				for (auto ptr=production.at(*ptrj).begin();ptr!=production.at(*ptrj).end();++ptr){
					i=0;
					while(i<(*ptr).length()){
						if( ((*ptr)[i]!='\'')&&((*ptr)[i]!='^') ){//在ptri对应非终极符的产生式右侧寻找可替代项
							while( ((*ptr)[i+1]=='\'')||((*ptr)[i+1]=='^') ){
								mterminalStr.push_back((*ptr)[i]);
								i++;
							}
							mterminalStr.push_back((*ptr)[i]);	
						}
						if(mterminalStr == *ptri){
							flag=false;
							break;
						}
						i++;
					}
					if(!flag)
						break;
				}
				if(!flag)
					break;
			}
			if(flag && (*ptri)!=getStartStr()){
				nterminalStr.erase(ptri);
				isDelete=true;
				break;
			}
		}
	}
}

void ContextFreeGrammar::getFileLine(const std::string &fileName){
	int line=0;//用来记录当前读取的是文法的第几个产生式，为后面设定开始符做依据
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
		line++;
		getLine(rawProduction,line);
    }
    
    input.close();
}

void ContextFreeGrammar::getLine(const std::string &raw_Production,const int &line){
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
        
	if(line==1){
		setStartStr(lhProduction);
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
