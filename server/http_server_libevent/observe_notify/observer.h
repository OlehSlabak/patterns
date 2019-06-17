#ifndef OBSERVER_H_INCLUDED
#define OBSERVER_H_INCLUDED
#include <functional>
#include <vector>
#include "../singleton_logging/logging.hpp"

#include <list>

class IObserver
{
    public:
        virtual void Update(float price) = 0;
};

class Shop : IObserver
{
    public:
        Shop(std::string n);
        void Update(float price);
    private:
        std::string _name;
        float price;
};

class ASubject
{
    public:
        void Attach(Shop *product);
        void Detach(Shop *product);
        void Notify(float price);
    private:
    std::vector<Shop*> _list;
};

class DummyProduct : public ASubject
{
    public:
        void ChangePrice(float price);
};





class Observable
{
    public:
        using Callback = std::function<void(void)>;

        template<typename O, typename F>
        void AddObserver(O self, F f)
        {
            _obs.push_back(Callback([self, f](){ (self->*f)();}));
        }
        void NotifyObservers();
    private:
        std::vector<Callback> _obs{};
};

#endif // OBSERVER_H_INCLUDED
