#include "vulkanSemaphore.hpp"
#include "vulkanContext.hpp"

namespace nwt
{
    void VulkanSemaphore::initialize() {
        VkSemaphoreCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(_context->device().vkDevice(), &createInfo, nullptr, &_semaphore) != VK_SUCCESS) {
            std::runtime_error("Could not create semaphore!");
        }

        LOG_INFO("Semaphore Successfully Created! " << _semaphore);
    }

    void VulkanSemaphore::destroy() {
        if (_semaphore == VK_NULL_HANDLE) {
            return;
        }

        vkDestroySemaphore(_context->device().vkDevice(), _semaphore, nullptr);

        LOG_INFO("Semaphore Destroyed!");
    }

    VkSemaphore VulkanSemaphore::vkSemaphore() const {
        return _semaphore;
    }

    VulkanSemaphore::operator VkSemaphore() const {
        return _semaphore;
    }
} // namespace nwt