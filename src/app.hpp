#ifndef APP_HPP
#define APP_HPP

namespace taf {

    class App {
    public:
        virtual void load() {}
        virtual void update() {}
        virtual void draw() {}

        void run();

    private:

    };

}

#endif