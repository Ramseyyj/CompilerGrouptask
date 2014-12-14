#ifndef __PREDICTIONTABLE_COORDINATE_H__
#define __PREDICTIONTABLE_COORDINATE_H__
#include <string>
struct PredictionTable_coordinate
{
	std::string A;
	std::string a;
	friend bool operator < (const PredictionTable_coordinate &,const PredictionTable_coordinate &) ;	
	
};

//对<进行重载从而使这个结构体可以作为map的键值
inline bool operator < (const PredictionTable_coordinate &ls,const PredictionTable_coordinate &rs) {
		return (ls.A < rs.A || (ls.A == rs.A && ls.a < rs.a));
	}

#endif/* defined(__PREDICTIONTABLE_COORDINATE_H__) */