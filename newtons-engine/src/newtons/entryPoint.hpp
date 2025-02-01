#pragma once

extern nwt::Application* nwt::createApp();

int main(int argc, char** argv){
    auto app = nwt::createApp();
    app->run();
    delete app;
}