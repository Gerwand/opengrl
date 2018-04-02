#include "../OpenGRL_GUI/RandomDecisionForest.h"

#include <iostream>

constexpr bool gpuTraining = false;

int main()
{
    grl::ForestTrainGPUContext gpuContext;

    if (gpuTraining) {
        cl_int err = CL_SUCCESS;

        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if (platforms.empty()) {
            std::cout << "No platforms found!\n";
            exit(ENODEV);
        }
        cl::Platform default_platform = platforms[1];
        std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

        cl_context_properties properties[] = 
           { CL_CONTEXT_PLATFORM, (cl_context_properties)(default_platform)(), 0};
        cl::Context clGPUContext(CL_DEVICE_TYPE_GPU, properties); 
  
        std::vector<cl::Device> devices = clGPUContext.getInfo<CL_CONTEXT_DEVICES>(&err);
        if (devices.empty()) {
            std::cout << "No devices found!\n";
            exit(ENODEV);
        }
        gpuContext.device = devices[0];
        std::cout << "Using device: " << gpuContext.device.getInfo<CL_DEVICE_NAME>(&err) << "\n";

        gpuContext.context = cl::Context({gpuContext.device});
        // Experimental
        gpuContext.maxImages = 5;
    }

    grl::RandomDecisionForest forest(5);
    grl::ForestTrainContext ctx = {
        gpuTraining ? &gpuContext : nullptr,
        5,
        2000,
        4000, // n tries
        20
    };

    printf("Loading RDF...\n");
    grl::loadDepthImagesWithClasses(
        0, 24000, 7,
        "H:\\Magisterka\\OpenGRL\\OpenGRL_GestureGenerator\\generated-train-small\\hand_classes_", // png
        "H:\\Magisterka\\OpenGRL\\OpenGRL_GestureGenerator\\generated-train-small\\hand_depth_", // exr
        ctx.classImages, ctx.depthImages);
    printf("Training RDF...\n");
    forest.train(ctx);
    forest.saveToFile("forest-small.txt");
}