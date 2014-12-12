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
	else{
		cout <<"的FIRST集：";
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
	string test_file;
    ContextFreeGrammar test_ContextFreeGrammar;

    unordered_set<string> test_terminalStr;
    unordered_set<string> test_nterminalStr;
    
    cout<<"请输入要测试文法所在的文件名：";
	getline(cin,test_file);
	test_file="F:\\programming\\compiler\\CompilerGrouptask\\CompilerGrouptask\\"+test_file;
    test_ContextFreeGrammar.getFileLine(test_file);
	//test_ContextFreeGrammar.simplify();
	test_nterminalStr = test_ContextFreeGrammar.getAllNTerminalStr();
    test_terminalStr = test_ContextFreeGrammar.getAllTerminalStr();

	cout<<"输入的文法如下："<<endl;
	 //printStr是非类库中的函数，我是在这个文件中自己定义的一个，1代表输出非终结符，0代表终结符
    //见此文件的顶部
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

	cout<<"转换为LL(1)后的文法如下："<<endl;
	//测试消除左递归的程序
	//cout<<"消除左递归后的文法如下："<<endl;
	test_ContextFreeGrammar.clearAllLeftRecursion();
    /*test_nterminalStr = test_ContextFreeGrammar.getAllNTerminalStr();
    test_terminalStr = test_ContextFreeGrammar.getAllTerminalStr();
    printStr(test_nterminalStr, 1);
    printStr(test_terminalStr, 0);

	for( auto ptr = test_nterminalStr.cbegin(); ptr!=test_nterminalStr.cend(); ++ptr) {
        printProdcution(*ptr,test_ContextFreeGrammar.getProductionRhStr(*ptr));
    }*/

	//测试提取公共左因子的程序
	//cout<<"提取公共左因子后的文法如下："<<endl;
	test_ContextFreeGrammar.pickPublicLeftFactor();
    test_nterminalStr = test_ContextFreeGrammar.getAllNTerminalStr();
    test_terminalStr = test_ContextFreeGrammar.getAllTerminalStr();
    printStr(test_nterminalStr, 1);
    printStr(test_terminalStr, 0);

	for( auto ptr = test_nterminalStr.cbegin(); ptr!=test_nterminalStr.cend(); ++ptr) {
        printProdcution(*ptr,test_ContextFreeGrammar.getProductionRhStr(*ptr));
    }
	
	return 0;
}
