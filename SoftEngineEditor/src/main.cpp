#include <iostream>
#include <memory>

#include <SoftEngineCore/Application.hpp>

class MyApp : public SoftEngine::Application 
{
	void on_update() override 
	{
		std::cout << "Update frame: " << frame++ << std::endl;
	}

	int frame = 0;
};

int main() {
	auto myApp = std::make_unique<MyApp>();

	int returnCode = myApp->start(1024, 768, "My first App");

	return returnCode;
}