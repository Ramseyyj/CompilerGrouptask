#ifndef __PREDICTIONTABLE_COORDINATE_H__
#define __PREDICTIONTABLE_COORDINATE_H__
#include <string>
struct PredictionTable_coordinate
{
	std::string A;
	std::string a;
	friend bool operator < (const PredictionTable_coordinate &,const PredictionTable_coordinate &) ;	
	
};

//��<�������شӶ�ʹ����ṹ�������Ϊmap�ļ�ֵ
inline bool operator < (const PredictionTable_coordinate &ls,const PredictionTable_coordinate &rs) {
		return (ls.A < rs.A || (ls.A == rs.A && ls.a < rs.a));
	}

#endif/* defined(__PREDICTIONTABLE_COORDINATE_H__) */