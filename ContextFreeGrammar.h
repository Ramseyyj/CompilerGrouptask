//
//  ContextFreeGrammar.h
//  CompilerTask
//
//  Created by lms on 11/13/14.
//  Copyright (c) 2014 lms. All rights reserved.
//

#ifndef __CONTEXT_FREE_GRAMMAR_H__
#define __CONTEXT_FREE_GRAMMAR_H__

#include <iostream>
#include <string>
#include <unordered_set>
#include <map>
#include <vector>
#include <utility>

class ContextFreeGrammar
{
public:
    void addTerminalStr(const std::string &);
    void addNTerminalStr(const std::string &);
    void setStartStr(const std::string &);
    
    //����ĳһ�ض����ս�������в���ʽ
    std::unordered_set<std::string> getProductionRhStr(const std::string &) const;
    std::unordered_set<std::string> getAllTerminalStr() const;
    std::unordered_set<std::string> getAllNTerminalStr() const;
    std::string getStartStr() const;
    
    bool isTerminalStr(const std::string &) const;
	void clearLeftRecursion() ;
	void ContextFreeGrammar::pickPublicLeftFactor();

	void getLine(const std::string &);

    //���ļ���ȡ
    void getFileLine(const std::string &);

	

private:
    std::string startStr;
    std::unordered_set<std::string> terminalStr;    //�ս��
    std::unordered_set<std::string> nterminalStr;   //���ս��
    std::map<std::string,std::vector<std::string>> production;  //����ʽ
    
};

inline std::unordered_set<std::string> ContextFreeGrammar::getAllTerminalStr() const{
    return terminalStr;
}

inline std::unordered_set<std::string> ContextFreeGrammar::getAllNTerminalStr() const{
    return nterminalStr;
}

inline void ContextFreeGrammar::setStartStr(const std::string &str){
    startStr = str;
}

inline std::string ContextFreeGrammar::getStartStr() const{
    return startStr;
}

#endif /* defined(__CONTEXT_FREE_GRAMMAR_H__) */
