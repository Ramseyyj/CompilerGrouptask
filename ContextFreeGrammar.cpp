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
	bool isBigger=true; //�жϱ���һ�����в���ʽ��������ռ�����First����û�б������ֵΪtrue������ֵΪfalse

	for(auto ptr=terminalStr.cbegin();ptr!=terminalStr.cend();++ptr){//�����ս����first��Ϊ����
		first[*ptr].insert(*ptr);
	}

	while(isBigger){//ѭ��������FIRST���Ĺ���ֱ��ÿ��FIRST�������ٱ��Ϊֹ
		isBigger=false;
		for(auto ptri=nterminalStr.cbegin();ptri!=nterminalStr.cend();++ptri){
			for(auto ptrj=production.at(*ptri).cbegin();ptrj!=production.at(*ptri).cend();++ptrj){
				i=0;
		      	while( ((*ptrj)[i+1]=='\'')||((*ptrj)[i+1]=='^') ){
				    mterminalStr.push_back((*ptrj)[i]);
			    	i++;
		     	}
		    	mterminalStr.push_back((*ptrj)[i++]);	

		    	if(terminalStr.find(mterminalStr)!=terminalStr.end()){//����Ҳ����е�һ��Ϊ�ս��
					if(first[*ptri].find(mterminalStr)==first[*ptri].end()){
						first[*ptri].insert(mterminalStr);
						isBigger=true;
					}
		    	}
		    	else if(nterminalStr.find(mterminalStr)!=nterminalStr.end()){//����Ҳ����е�һ��Ϊ���ռ���
		     		for(auto ptr=first[mterminalStr].cbegin();ptr!=first[mterminalStr].cend();++ptr){
			    		if( ((*ptr)[0]!='$')&&(first[*ptri].find(*ptr)==first[*ptri].end()) ){
			    			first[*ptri].insert(*ptr);
							isBigger=true;
			    		}
			    	}

					while( (i<(*ptrj).length())&&isContain$(mterminalStr) ){//��������ǰ�涼�Ƿ��ռ����Ҷ����Ƴ�$�����
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
	bool isBigger=true;//�жϱ���һ�����в���ʽ��������ռ�����FOLLOW����û�б������ֵΪtrue������ֵΪfalse
	std::string mterminalStrNow;//���ڼ�¼���е�ǰ�Ĵ�������ս������ռ�����
	std::string mterminalStrNext;//���ڼ�¼����ǰһ����������ս������ռ�����

	follow[getStartStr()].insert("#");//�����ķ��Ŀ�ʼ������#��FOLLOW����
	
	while(isBigger){//ѭ��������FOLLOW���Ĺ���ֱ��ÿ��FOLLOW�������ٱ��Ϊֹ
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
				mterminalStrNow.push_back((*ptrj)[i]);	//��ȡ����һ���ս������ռ�������mterminalStrNow
				i++;
				while(i<(*ptrj).length()){//��������ʽ�Ҳ��һ����
					if(terminalStr.find(mterminalStrNow)!=terminalStr.end()){//�����ǰ��Ϊ�ս����ֱ��������һ��
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
			       	mterminalStrNext.push_back((*ptrj)[i]);	//ȡ�����е���һ���ս������ռ�������mterminalStrNext
					i++;

					for(auto ptr=first[mterminalStrNext].cbegin();ptr!=first[mterminalStrNext].cend();++ptr){
						if( ((*ptr)[0]!='$')&&(follow[mterminalStrNow].find(*ptr)==follow[mterminalStrNow].end()) ){
							follow[mterminalStrNow].insert(*ptr);
							isBigger=true;
						}
					}
					if( (nterminalStr.find(mterminalStrNext)!=nterminalStr.end())&&(isContain$(mterminalStrNext)) ){//�����һ���Ƿ��ռ��������Ƴ�$
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
	std::string tempPro;//���ڼ�¼�������õĲ���ʽ
	std::string sentenceNow;//��ǰ�������
	std::string stackTop;//STACKջ��Ԫ��

	STACK.push("#");
	STACK.push(getStartStr());//���Ȱ�#���ķ��Ŀ�ʼ��ѹ��STACKջ

	while( (sentence[i+1]=='\'')||(sentence[i+1]=='^') ){
		sentenceNow.push_back(sentence[i]);
	   	i++;	
	}
	sentenceNow.push_back(sentence[i++]);//�ѵ�һ��������Ŷ���sentenceNow

	std::cout<<std::setw(5)<<std::left<<"step"<<std::setw(15)<<std::left<<"STACK";
	std::cout<<std::setw(10)<<std::right<<"sentence"<<"    "<<std::setw(20)<<std::left<<"used production"<<std::endl;
	std::cout<<"========================================================"<<std::endl;
	flag=true;
	isPro=false;
	while (flag) {
		std::cout<<std::setw(5)<<std::left<<step++;
		
		while(!STACK.empty()){//��STACK�е�Ԫ����ʱ����tempStack���������STACKջ
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
		if( terminalStr.find(stackTop)!=terminalStr.end() ){//���ջ��Ԫ��Ϊ�ս��
			if(stackTop==sentenceNow){//���ջ��Ԫ�غ͵�ǰ���������ͬ��������һ���������				
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
			if(stackTop==sentenceNow){//��������
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
				while(!tempStack.empty()){//�Ѳ���ʽ�Ҳ����е�ÿһ���ս������ռ�������ѹ��STACKջ��
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
	std::vector<std::string> a,b;  //����������ݹ���м����

	int i;
	bool flog; //�����жϵ�ǰ���ռ����Ƿ������ݹ�������true��ʾ���ڣ����򲻴���
	
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

	//������ݹ�
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
	std::string b;//�洢����ʽ�Ҳ����н�Ҫ���滻�ķ��ռ����ĺ󲿷�


	std::vector<std::string> tempVector;//���ڴ洢���滻�����

	for (auto ptri = nterminalStr.cbegin(); ptri!=nterminalStr.cend(); ++ptri) {//���������ķ��еķ��ս��
		
		for (auto ptrj = nterminalStr.cbegin(); ptrj!= ptri; ++ptrj){//������ǰ�ս��ǰ�����з��ռ���
			flog=true;
			while (flog){
				flog=false;
				//����ptriָ����ռ����Ĳ���ʽ�Ҳ�ÿһ���ptrjָ����ռ����Ĳ���ʽ�Ҳ�ÿһ����д������
				for (auto ptr =production.at(*ptri).begin();ptr!=production.at(*ptri).end();++ptr){
					i=0;j=0;
					//��ptri��Ӧ���ռ����Ĳ���ʽ�Ҳ�Ѱ�ҿ������
					while( ((*ptr)[i+1]=='\'')||((*ptr)[i+1]=='^') ){
						mterminalStr.push_back((*ptr)[i]);
						i++;j++;
					}
					mterminalStr.push_back((*ptr)[i]);		
					for(auto k=i+1;k<(*ptr).length();++k){
						b.push_back((*ptr)[k]);
					}
					if(mterminalStr == *ptrj){//�ҵ��������������
						production.at(*ptri).erase(ptr);
						for(auto ptr1 =production.at(*ptrj).begin();ptr1!=production.at(*ptrj).end();++ptr1){
							for(auto k =0;k<(*ptr1).length();++k){//��Ҫ�滻���ռ�������ѹ����ʱ�ַ���
								tempStr.push_back((*ptr1)[k]);
							}
							for(auto k =0;k<b.length();++k){//�Ѻ󲿷�ѹ����ʱ�ַ���
								tempStr.push_back(b[k]);
							}
							tempVector.push_back(tempStr);//���滻��ɺ����ʱ�ַ���ѹ�뱻�滻�Ĳ���ʽ��Ϊһ����
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
	std::vector<std::string> a,b;  //����������ݹ���м����

	int i;
	int count; //������¼�ܹ��м�����й���������

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

		//��ȡ����������
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
	bool flag;//���ڼ�¼��ǰ���ռ����Ƿ���࣬�����ֵΪtrue������Ϊfalse
	bool isDelete=true;//���ڼ�¼һ��ѭ������û��ɾ�����ռ��������ɾ����ֵΪtrue,����Ϊfalse

	while(isDelete){
		isDelete=false;
		for (auto ptri= nterminalStr.begin();ptri!=nterminalStr.end();++ptri){
			flag=true;
			for(auto ptrj=nterminalStr.cbegin();ptrj!=nterminalStr.cend();++ptrj){
				for (auto ptr=production.at(*ptrj).begin();ptr!=production.at(*ptrj).end();++ptr){
					i=0;
					while(i<(*ptr).length()){
						if( ((*ptr)[i]!='\'')&&((*ptr)[i]!='^') ){//��ptri��Ӧ���ռ����Ĳ���ʽ�Ҳ�Ѱ�ҿ������
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
	int line=0;//������¼��ǰ��ȡ�����ķ��ĵڼ�������ʽ��Ϊ�����趨��ʼ��������
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

			i=i-j;//���л��ݣ���i�ص�����'��^֮ǰ��λ��
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
