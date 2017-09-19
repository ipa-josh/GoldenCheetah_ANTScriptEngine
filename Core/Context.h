#pragma once

//simplified version of context


class Context
{
	int CP_;
public:
	Context() : CP_(-1) {}
	
	void setCP(const int cp) {CP_=cp;}

	bool hasCP() const {return CP_>0;}
	int CP() const {return CP_;}
};
