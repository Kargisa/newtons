#pragma once

namespace nwt{

class Application{
public:
    Application(){

    }

    virtual ~Application(){

    }

public:
    void run();

private:
    void initWindow();
    void initVulkan();
    void cleanup();
};

Application* createApp();

} // namespace nwt