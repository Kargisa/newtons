#pragma once

#include "newtons/pch.hpp"
#include "spirv_reflect.h"

namespace nwt
{
    class VulkanReflect {
    public:
        static void spirvReflectExample(const uint32_t* spirv_code, size_t spirv_nbytes);
    };
} // namespace nwt
