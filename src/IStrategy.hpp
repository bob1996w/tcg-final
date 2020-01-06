#pragma once

class IStrategy {
public:
    virtual int boardScore();
    virtual int moveOrdering();
};