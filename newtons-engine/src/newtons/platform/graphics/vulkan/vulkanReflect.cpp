#include "vulkanReflect.hpp"

namespace nwt
{
    void VulkanReflect::spirvReflectExample(const uint32_t* spirv_code, size_t spirv_nbytes)
    {
        // Generate reflection data for a shader
        SpvReflectShaderModule module;
        SpvReflectResult result = spvReflectCreateShaderModule(spirv_nbytes, spirv_code, &module);

        LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        // Enumerate and extract shader's input variables
        uint32_t var_count = 0;
        result = spvReflectEnumerateInputVariables(&module, &var_count, NULL);

        LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        std::vector<SpvReflectInterfaceVariable*> input_vars(var_count);
        result = spvReflectEnumerateInputVariables(&module, &var_count, input_vars.data());

        LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        for (auto&& var : input_vars) {
            LOG_INFO("Variable Name: " << var->name);
        }


        // Output variables, descriptor bindings, descriptor sets, and push constants
        // can be enumerated and extracted using a similar mechanism.

        // Destroy the reflection data when no longer required.
        spvReflectDestroyShaderModule(&module);
    }


} // namespace nwt
