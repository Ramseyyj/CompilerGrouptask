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
        std::string rhProduction;
        std::string lhProduction; //left hand of the production
        std::string mterminalStr; //just one terminal
        
            
        std::getline(input, rawProduction, '\n');
        
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
    
    input.close();
}

