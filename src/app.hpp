#pragma once

class App {
public:
    virtual void load() {}
    virtual void update() {}
    virtual void draw() {}

    void run();

private:

};