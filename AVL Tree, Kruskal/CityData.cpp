#include "CityData.h"

CityData::CityData()
{
	LocationId = 0;
	name = new char[50];
	country = new char[50];
}

CityData::~CityData()
{
}

int CityData::GetLocationId()
{
	return this->LocationId;
}

char * CityData::Getname()
{
	return this->name;
}

char * CityData::Getcountry()
{
	return this->country;
}

void CityData::SetLocationId(int LocationId)
{
	this->LocationId = LocationId;
}

void CityData::Setname(char * name)
{
	strcpy(this->name, name);
}

void CityData::Setcountry(char * country)
{
	strcpy(this->country, country);
}
