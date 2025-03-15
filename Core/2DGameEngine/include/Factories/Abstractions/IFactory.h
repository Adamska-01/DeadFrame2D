#pragma once


template<typename T>
class IFactory
{
public:
	virtual ~IFactory() = default;
	
	
	virtual T CreateProduct() = 0;
};