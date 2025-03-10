# Vulkan Informatopn
Information I gathered about the Vulkan graphics API.

## Graphics Pipelines
Creating graphcis pipelines at runtime is seemingly rare for anything that is not shader hot reloading.

Pipeline creation is done at application startup and have no impact on 
the runtime after creation (or theres is no advantage in creating and destroying them per scene).