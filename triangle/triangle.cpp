#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <evolve/core/window.h>
#include <evolve/core/instance.h>
#include <evolve/core/gpudevices.h>
#include <evolve/core/swapchain.h>
#include <evolve/log/log.h>

const int WIDTH = 800;
const int HEIGHT = 600;

class TriangleApplication : public evolve::core::Window {
public:
	TriangleApplication()
		:evolve::core::Window(WIDTH, HEIGHT) {
		EVOLVE_LOG_DEBUG("Start application");
	}

	virtual ~TriangleApplication() {
		EVOLVE_LOG_DEBUG("End application");
	}
	
	void run() {
		initVulkan();
		mainLoop();
	}

private:
	virtual void onResize(int iWidth, int iHeight){
		_swapchain->recreateSwapchain(*this);
	}

	std::shared_ptr<evolve::core::ValidationLayers> _validationLayers;
	std::shared_ptr<evolve::core::Instance> _instance;
	std::shared_ptr<evolve::core::GPUDevices> _devices;
	std::shared_ptr<evolve::core::SwapChain> _swapchain;

	void initVulkan() {

		_validationLayers = std::shared_ptr<evolve::core::ValidationLayers>(
			new evolve::core::ValidationLayers());
		_instance = std::shared_ptr<evolve::core::Instance>(
			new evolve::core::Instance("Evolve test", _validationLayers));

		createSurface(_instance);

		_devices = std::shared_ptr<evolve::core::GPUDevices>(
			new evolve::core::GPUDevices(_instance, *this));

		_swapchain = std::shared_ptr<evolve::core::SwapChain>(
			new evolve::core::SwapChain(_instance, _devices, *this));
	}

	void mainLoop() {
		static unsigned int frameCounter = 0;
		while (!shouldClose()) {
			pollEvents();
			_swapchain->drawFrame(*this);
			++frameCounter;
		}

		_devices->waitIdleDevice();
	}

};

//INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//PSTR lpCmdLine, INT nCmdShow)
int main() {
	// Init log and profiler
	EVOLVE_ATTACH_LOGGER_REPORTER(new evolve::log::FileLoggerReporter("../bin/out.log"))

	//EVOLVE_LOG_DEBUG("Hello world !!");
	//EVOLVE_LOG_INFO("Hello world !!");
	//EVOLVE_LOG_WARNING("Hello world !!");
	//EVOLVE_LOG_ERROR("Hello world !!");


	try {
		EVOLVE_LOG_INFO("Begin run");

		TriangleApplication app;
		app.run();

		EVOLVE_LOG_INFO("End run");
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	//return 0;
	return EXIT_SUCCESS;
}