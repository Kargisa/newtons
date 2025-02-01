#include "newtons.hpp"

class Game : public nwt::Application{

};

nwt::Application* nwt::createApp(){
    return new Game;
}