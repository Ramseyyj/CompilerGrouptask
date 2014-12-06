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
    else{
        cout << "terminal string is :" << endl;
    }
    
    for (auto ptr = test_set.cbegin(); ptr!=test_set.cend(); ++ptr) {
        cout << *ptr <<endl;
    }
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
    ContextFreeGrammar test_ContextFreeGrammar;

    unordered_set<string> test_production1;
    unordered_set<string> test_production2;
	unordered_set<string> test_production3;

    unordered_set<string> test_terminalStr;
    unordered_set<string> test_nterminalStr;
    
    
    test_ContextFreeGrammar.getFileLine("test_ContextFreeGrammar1.txt");
    test_nterminalStr = test_ContextFreeGrammar.getAllNTerminalStr();
    test_terminalStr = test_ContextFreeGrammar.getAllTerminalStr();

    //test_production1 = test_ContextFreeGrammar.getProductionRhStr("P");
    //test_production2 = test_ContextFreeGrammar.getProductionRhStr("E'");
    test_production1 = test_ContextFreeGrammar.getProductionRhStr("E");
	test_production2 = test_ContextFreeGrammar.getProductionRhStr("T");
	test_production3 = test_ContextFreeGrammar.getProductionRhStr("F");
    
    //printStr是非类库中的函数，我是在这个文件中自己定义的一个，1代表输出非终结符，0代表终结符
    //见此文件的顶部
    printStr(test_nterminalStr, 1);
    printStr(test_terminalStr, 0);
    
    //printProdcution("P", test_production1);
    //printProdcution("E'", test_production2);
    printProdcution("E", test_production1);
	printProdcution("T", test_production1);
	printProdcution("F", test_production1);
	return 0;
}
