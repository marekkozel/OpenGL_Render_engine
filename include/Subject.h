#pragma once

#include <vector>
#include <memory>
#include <algorithm>

class Observer;

enum class SubjectType
{
    Camera,
    DrawableObject,
    Light,
    Entity,
    Unknown
};

class Subject : public std::enable_shared_from_this<Subject>
{
protected:
    std::vector<std::weak_ptr<Observer>> observers;

public:
    virtual ~Subject() = default;

    virtual SubjectType get_type() const = 0;

    void attach_observer(std::shared_ptr<Observer> observer);
    void detach_observer(std::shared_ptr<Observer> observer);
    void notify_observers();
};