#pragma once
#include <memory>

class Subject;

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void notify(std::shared_ptr<Subject> subject) = 0;
};
