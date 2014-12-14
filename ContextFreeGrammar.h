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
#include "PredictionTable_Coordinate.h"

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

	void calFirstSet();
	void calFollowSet();
	void calPredictionTable();
	void analysisSentenceByPredictionTable(const std::string &);

	std::unordered_set<std::string> getFirstSet(const std::string &) const;
	std::unordered_set<std::string> getFollowSet(const std::string &) const;
	std::string getPredictionTable(const PredictionTable_coordinate &) const;
    std::string getStartStr() const;
    
    bool isTerminalStr(const std::string &) const;

	//�ж�һ�����ռ����Ĳ���ʽ�Ҳ��Ƿ����$��
	bool isContain$(const std::string &) const;

	//�ж�һ������ʽ���Ƿ�����ݹ�ĺ���
	bool isLeftRecursion(const std::string &, const std::string &) const;
	void clearDirectLeftRecursion(const std::string &);
	void clearAllLeftRecursion();
	void ContextFreeGrammar::pickPublicLeftFactor();
	void ContextFreeGrammar::simplify();
	void getLine(const std::string &,const int &);

    //���ļ���ȡ
    void getFileLine(const std::string &);	

private:
    std::string startStr;
    std::unordered_set<std::string> terminalStr;    //�ս��
    std::unordered_set<std::string> nterminalStr;   //���ս��
    std::map<std::string,std::vector<std::string>> production;  //����ʽ
    std::map<std::string,std::unordered_set<std::string>> first;//FIRST��
	std::map<std::string,std::unordered_set<std::string>> follow;//FOLLOW��
	std::map<PredictionTable_coordinate,std::string> predictiontable;//Ԥ�������
};

inline std::unordered_set<std::string> ContextFreeGrammar::getAllTerminalStr() const{
    return terminalStr;
}

inline std::unordered_set<std::string> ContextFreeGrammar::getAllNTerminalStr() const{
    return nterminalStr;
}

inline std::unordered_set<std::string> ContextFreeGrammar::getFirstSet(const std::string &Str) const{
	return first.at(Str);
}

inline std::unordered_set<std::string> ContextFreeGrammar::getFollowSet(const std::string &Str) const{
	return follow.at(Str);
}

inline std::string ContextFreeGrammar::getPredictionTable(const PredictionTable_coordinate &coord) const{
	return predictiontable.at(coord);
}

inline void ContextFreeGrammar::setStartStr(const std::string &str){
    startStr = str;
}

inline std::string ContextFreeGrammar::getStartStr() const{
    return startStr;
}

#endif /* defined(__CONTEXT_FREE_GRAMMAR_H__) */
