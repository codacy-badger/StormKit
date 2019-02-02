// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/Device.hpp>
#include <storm/window/Window.hpp>

#include <storm/engine/vulkan/UniqueHandle.hpp>
#include <storm/engine/vulkan/PipelineCache.hpp>

namespace storm::engine {
	struct BackedVkImage {
		UniqueHandle<vk::Image>        image;
		UniqueHandle<vk::ImageView>    view;
		UniqueHandle<vk::DeviceMemory> memory;

		uvec2 extent;

		vk::ImageLayout layout = vk::ImageLayout::eUndefined;
	};

	struct BackedVkTexture {
		UniqueHandle<vk::Sampler> sampler;
		BackedVkImage             image;
	};

	struct BackedVkBuffer {
		UniqueHandle<vk::Buffer>       buffer;
		UniqueHandle<vk::DeviceMemory> memory;
	};

	class DeviceImpl : public core::NonCopyable {
		public:
			struct FamilyQueueIndices {
				std::uint8_t graphics_queue_index = 255u;
				std::uint8_t compute_queue_index  = 255u;
				std::uint8_t transfert_queue_index  = 255u;
			};

			enum class QueueType {
				GRAPHICS = 0,
				COMPUTE  = 1,
				TRANSFERT  = 2
			};
			
			explicit DeviceImpl(const Device &parent, const Context &context,
								window::NativeHandle handle, PhysicalDevice &&device);
			~DeviceImpl();
			
			DeviceImpl(DeviceImpl &&);
			
			ColorFormat bestDepthFormat() const noexcept;

			BackedVkImage createBackedVkImage(const vk::ImageCreateInfo &infos, vk::ImageViewType type,
											  vk::ImageAspectFlags aspect, vk::ImageLayout layout,
											  vk::MemoryPropertyFlags memory_properties) const;
			BackedVkTexture createBackedVkTexture(const vk::SamplerCreateInfo &sampler_info, const vk::ImageCreateInfo &infos, vk::ImageViewType type,
												  vk::ImageAspectFlags aspect, vk::ImageLayout layout,
												  vk::MemoryPropertyFlags memory_properties) const;
			BackedVkBuffer createBackedVkBuffer(vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memory_properties,
												vk::DeviceSize size) const;

			UniqueHandle<vk::CommandBuffer> allocateCommandBuffer(QueueType type = QueueType::GRAPHICS, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;

			std::int32_t findMemoryRequirement(std::uint32_t filter, vk::MemoryPropertyFlags properties) const;
			
			void waitIdle() const;

			vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates,
									vk::ImageTiling tiling,
									vk::FormatFeatureFlags features) const noexcept;

			inline const Context &context() const noexcept;
			inline const PhysicalDevice &physicalDevice() const noexcept;
			inline FamilyQueueIndices familyQueuesIndices() const noexcept;
			inline PipelineCache &pipelineCache() noexcept;
			inline const PipelineCache &pipelineCache() const noexcept;

			inline const vk::Device &vkDevice() const noexcept;
			inline const vk::Queue &vkGraphicsQueue() const noexcept;
			inline const vk::Queue &vkComputeQueue() const noexcept;
			inline const vk::Queue &vkTransfertQueue() const noexcept;
		private:
			void createLogicalDevice();
			void updateQueueData(window::NativeHandle handle);
			void createCommandPools();
			
			FamilyQueueIndices m_family_queue_indices;
			
			UniqueHandle<vk::Device> m_logical_device;
			vk::Queue m_graphics_queue;
			vk::Queue m_compute_queue;
			vk::Queue m_transfert_queue;

			std::array<UniqueHandle<vk::CommandPool>, 3> m_command_pools;
			
			PhysicalDevice m_physical_device;
			PipelineCache  m_pipeline_cache;
			
			const Context &m_context;
			const Device &m_parent;
	};
}

#include "DeviceImpl.inl"
