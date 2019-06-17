#include "observer.h"
#include <iostream>
#include <string>
#include <algorithm>
void Observable::NotifyObservers()
{
    logging::DEBUG("NotifyObsevers");
    for (auto f: _obs)
    {
        f();
    }
}


void ASubject::Attach(Shop *shop)
{
    _list.push_back(shop);
}

void ASubject::Detach(Shop *shop)
{
       _list.erase(std::remove(_list.begin(), _list.end(), shop), _list.end());
}

void ASubject::Notify(float price)
{
    for (auto iter : _list)
    {
        if (iter != 0)
        {
            iter->Update(price);
        }
    }
}

void DummyProduct::ChangePrice(float price)
{
    Notify(price);
}

Shop::Shop(std::string name)
{
    this->_name = name;
}

void Shop::Update(float price)
{
    this->price = price;
    std::cout << " Price at " << _name << " is now " << price << std::endl;
}













