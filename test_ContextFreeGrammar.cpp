//
//  test_ContextFreeGrammar.cpp
//  CompilerTask
//
//  Created by lms on 12/4/14.
//  Copyright (c) 2014 lms. All rights reserved.
//

#include "ContextFreeGrammar.h"
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

void printStr(const unordered_set<string> &test_set,const int flag){
    if (flag==1) {
        cout << "nterminal string is :" << endl;
    }
    else if(flag==0){
        cout << "terminal string is :" << endl;
    }
	else if(flag==2){
		cout <<"��FIRST����";
	}
	else if(flag==3){
		cout <<"��FOLLOW����";
	}
    
    for (auto ptr = test_set.cbegin(); ptr!=test_set.cend(); ++ptr) {
        cout << *ptr <<"  ";
    }
    cout<<endl;
	cout<<endl;
}

void printProdcution(const string &str, const unordered_set<string> &test_prdtion){
    for (auto ptr = test_prdtion.cbegin(); ptr!= test_prdtion.cend(); ++ptr) {
        cout << str << "->" <<*ptr<<endl;
    }
	cout<<endl;
}

int main()
{
	string sentence;
	PredictionTable_coordinate coord;
	string test_file;
    ContextFreeGrammar test_ContextFreeGrammar;

    unordered_set<string> test_terminalStr;
    unordered_set<string> test_nterminalStr;
    
    cout<<"������Ҫ�����ķ����ڵ��ļ�����";
	getline(cin,test_file);
	test_file="F:\\programming\\compiler\\CompilerGrouptask\\CompilerGrouptask\\"+test_file;
    test_ContextFreeGrammar.getFileLine(test_file);
	//test_ContextFreeGrammar.simplify();
	test_nterminalStr = test_ContextFreeGrammar.getAllNTerminalStr();
    test_terminalStr = test_ContextFreeGrammar.getAllTerminalStr();

	cout<<"������ķ����£�"<<endl;
	 //printStr�Ƿ�����еĺ���������������ļ����Լ������һ����1����������ս����0�����ս��
    //�����ļ��Ķ���
    printStr(test_nterminalStr, 1);
    printStr(test_terminalStr, 0);

	for( auto ptr = test_nterminalStr.cbegin(); ptr!=test_nterminalStr.cend(); ++ptr) {
        printProdcution(*ptr,test_ContextFreeGrammar.getProductionRhStr(*ptr));
    }
	


	cout<<"ת��ΪLL(1)����ķ����£�"<<endl;
	//����������ݹ�ĳ���
	//cout<<"������ݹ����ķ����£�"<<endl;
	test_ContextFreeGrammar.clearAllLeftRecursion();
    /*test_nterminalStr = test_ContextFreeGrammar.getAllNTerminalStr();
    test_terminalStr = test_ContextFreeGrammar.getAllTerminalStr();
    printStr(test_nterminalStr, 1);
    printStr(test_terminalStr, 0);

	for( auto ptr = test_nterminalStr.cbegin(); ptr!=test_nterminalStr.cend(); ++ptr) {
        printProdcution(*ptr,test_ContextFreeGrammar.getProductionRhStr(*ptr));
    }*/

	//������ȡ���������ӵĳ���
	//cout<<"��ȡ���������Ӻ���ķ����£�"<<endl;
	test_ContextFreeGrammar.pickPublicLeftFactor();
    test_nterminalStr = test_ContextFreeGrammar.getAllNTerminalStr();
    test_terminalStr = test_ContextFreeGrammar.getAllTerminalStr();
    printStr(test_nterminalStr, 1);
    printStr(test_terminalStr, 0);

	for( auto ptr = test_nterminalStr.cbegin(); ptr!=test_nterminalStr.cend(); ++ptr) {
        printProdcution(*ptr,test_ContextFreeGrammar.getProductionRhStr(*ptr));
    }
	
	test_ContextFreeGrammar.calFirstSet();
	for( auto ptr = test_nterminalStr.cbegin(); ptr!=test_nterminalStr.cend(); ++ptr) {
        cout<<*ptr<<" ";
		printStr(test_ContextFreeGrammar.getFirstSet(*ptr),2);
    }

	test_ContextFreeGrammar.calFollowSet();
	for( auto ptr = test_nterminalStr.cbegin(); ptr!=test_nterminalStr.cend(); ++ptr) {
        cout<<*ptr<<" ";
		printStr(test_ContextFreeGrammar.getFollowSet(*ptr),3);
    }

	//����Ԥ�������Ĺ���
	test_ContextFreeGrammar.calPredictionTable();
	//���Ԥ�������
	for(auto ptri=test_nterminalStr.cbegin();ptri!=test_nterminalStr.cend();++ptri){
		for(auto ptrj=test_terminalStr.cbegin();ptrj!=test_terminalStr.cend();++ptrj){
			if( (*ptrj)[0]!='$' ){
				coord.A=*ptri;
				coord.a=*ptrj;
				cout<<"M["<<*ptri<<","<<*ptrj<<"]="<<test_ContextFreeGrammar.getPredictionTable(coord)<<endl;
			}
		}
		coord.A=*ptri;
		coord.a="#";
		cout<<"M["<<*ptri<<",#]="<<test_ContextFreeGrammar.getPredictionTable(coord)<<endl;
	}
	cout<<endl;

	cout<<"��������Ҫ����Ԥ������ľ���:";
	cin>>sentence;
	sentence=sentence+"#";
	test_ContextFreeGrammar.analysisSentenceByPredictionTable(sentence);

	return 0;
}
