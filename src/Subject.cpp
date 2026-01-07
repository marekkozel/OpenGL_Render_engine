#include "Subject.h"
#include "Observer.h"

void Subject::attach_observer(std::shared_ptr<Observer> observer)
{
    observers.emplace_back(observer);
}

void Subject::detach_observer(std::shared_ptr<Observer> observer)
{
    for (auto it = observers.begin(); it != observers.end();)
    {
        std::shared_ptr<Observer> current = it->lock();

        if (!current || current == observer)
        {
            it = observers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Subject::notify_observers()
{
    for (auto it = observers.begin(); it != observers.end();)
    {
        std::shared_ptr<Observer> current = it->lock();

        if (current)
        {
            current->notify(shared_from_this());
            ++it;
        }
        else
        {
            it = observers.erase(it);
        }
    }
}