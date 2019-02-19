// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <array>
#include <storm/engine/vulkan/TypeConversions.hpp>

namespace storm::engine {

	template <typename EnumA, typename EnumB, std::size_t size>
	using MappingArray = std::array<std::pair<EnumA, EnumB>, size>;

#define AS(type, map)                                                          \
	const auto iterator = std::find_if(std::begin(map), std::end(map),         \
	    [type](const auto &pair) { return pair.second == type; });             \
	return iterator->first;

#define FROM(type, map)                                                        \
	const auto iterator = std::find_if(std::begin(map), std::end(map),         \
	    [type](const auto &pair) { return pair.first == type; });              \
	return iterator->second;

	static inline constexpr const auto SHADER_TYPE_MAP
	    = MappingArray<vk::ShaderStageFlagBits, storm::engine::Shader::Stage,
	        static_cast<std::size_t>(storm::engine::Shader::Stage::SIZE)> {
	        std::pair<vk::ShaderStageFlagBits, storm::engine::Shader::Stage> {
	            vk::ShaderStageFlagBits::eVertex,
	            storm::engine::Shader::Stage::VERTEX},
	        {vk::ShaderStageFlagBits::eFragment,
	            storm::engine::Shader::Stage::FRAGMENT},
	        {vk::ShaderStageFlagBits::eGeometry,
	            storm::engine::Shader::Stage::GEOMETRY},
	        {vk::ShaderStageFlagBits::eCompute,
	            storm::engine::Shader::Stage::COMPUTE},
	        {vk::ShaderStageFlagBits::eTessellationControl,
	            storm::engine::Shader::Stage::TESSELATION_CONTROL},
	        {vk::ShaderStageFlagBits::eTessellationEvaluation,
	            storm::engine::Shader::Stage::TESSELATION_EVALUATION},
	    };

	vk::ShaderStageFlagBits asVK(storm::engine::Shader::Stage type) noexcept {
	    AS(type, SHADER_TYPE_MAP)}

	storm::engine::Shader::Stage fromVK(vk::ShaderStageFlagBits type) noexcept {
		FROM(type, SHADER_TYPE_MAP)
	}

	static inline constexpr const auto TOPOLOGY_MAP
	    = MappingArray<vk::PrimitiveTopology, storm::engine::PrimitiveTopology,
	        static_cast<std::size_t>(storm::engine::PrimitiveTopology::SIZE)> {
	        std::pair<vk::PrimitiveTopology, storm::engine::PrimitiveTopology> {
	            vk::PrimitiveTopology::eTriangleList,
	            storm::engine::PrimitiveTopology::TRIANGLES},
	        {vk::PrimitiveTopology::eTriangleStrip,
	            storm::engine::PrimitiveTopology::TRIANGLE_STRIP},
	    };

	vk::PrimitiveTopology asVK(
	    storm::engine::PrimitiveTopology topology) noexcept {
	    AS(topology, TOPOLOGY_MAP)}

	storm::engine::PrimitiveTopology
	    fromVK(vk::PrimitiveTopology topology) noexcept {
		FROM(topology, TOPOLOGY_MAP)
	}

	static inline constexpr const auto POLYGON_MODE_MAP
	    = MappingArray<vk::PolygonMode, storm::engine::PolygonMode,
	        static_cast<std::size_t>(storm::engine::PolygonMode::SIZE)> {
	        std::pair<vk::PolygonMode, storm::engine::PolygonMode> {
	            vk::PolygonMode::eFill, storm::engine::PolygonMode::FILL},
	        {vk::PolygonMode::eLine, storm::engine::PolygonMode::LINE},
	        {vk::PolygonMode::ePoint, storm::engine::PolygonMode::POINT},
	    };

	vk::PolygonMode asVK(storm::engine::PolygonMode mode) noexcept {
	    AS(mode, POLYGON_MODE_MAP)}

	storm::engine::PolygonMode fromVK(vk::PolygonMode mode) noexcept {
		FROM(mode, POLYGON_MODE_MAP)
	}

	static inline const auto CULL_MODE_MAP
	    = MappingArray<vk::CullModeFlags, storm::engine::CullMode,
	        static_cast<std::size_t>(storm::engine::CullMode::SIZE)> {
	        std::pair<vk::CullModeFlags, storm::engine::CullMode> {
	            vk::CullModeFlagBits::eBack, storm::engine::CullMode::BACK},
	        {vk::CullModeFlagBits::eFront, storm::engine::CullMode::FRONT},
	        {vk::CullModeFlagBits::eFrontAndBack,
	            storm::engine::CullMode::FRONT_AND_BACK},
	        {vk::CullModeFlagBits::eNone, storm::engine::CullMode::NONE},
	    };

	vk::CullModeFlags asVK(storm::engine::CullMode mode) noexcept {
	    AS(mode, CULL_MODE_MAP)}

	storm::engine::CullMode fromVK(vk::CullModeFlags mode) noexcept {
		FROM(mode, CULL_MODE_MAP)
	}

	static inline constexpr const auto FRONT_FACE_MAP
	    = MappingArray<vk::FrontFace, storm::engine::FrontFace,
	        static_cast<std::size_t>(storm::engine::FrontFace::SIZE)> {
	        std::pair<vk::FrontFace, storm::engine::FrontFace> {
	            vk::FrontFace::eClockwise, storm::engine::FrontFace::CLOCKWISE},
	        {vk::FrontFace::eCounterClockwise,
	            storm::engine::FrontFace::COUNTER_CLOCKWISE},
	    };

	vk::FrontFace asVK(storm::engine::FrontFace face) noexcept {
	    AS(face, FRONT_FACE_MAP)}

	storm::engine::FrontFace
	    fromVK(vk::FrontFace face) noexcept {FROM(face, FRONT_FACE_MAP)}

	vk::ColorComponentFlags asVK(storm::engine::ColorComponentFlag color) noexcept {
		auto _color = vk::ColorComponentFlags {};

		if ((color & storm::engine::ColorComponentFlag::R)
		    == storm::engine::ColorComponentFlag::R)
			_color |= vk::ColorComponentFlagBits::eR;
		if ((color & storm::engine::ColorComponentFlag::G)
		    == storm::engine::ColorComponentFlag::G)
			_color |= vk::ColorComponentFlagBits::eG;
		if ((color & storm::engine::ColorComponentFlag::B)
		    == storm::engine::ColorComponentFlag::B)
			_color |= vk::ColorComponentFlagBits::eB;
		if ((color & storm::engine::ColorComponentFlag::A)
		    == storm::engine::ColorComponentFlag::A)
			_color |= vk::ColorComponentFlagBits::eA;

		return _color;
	}

	storm::engine::ColorComponentFlag fromVK(
	    vk::ColorComponentFlags color) noexcept {
		auto _color = storm::engine::ColorComponentFlag {};

		if (color & vk::ColorComponentFlagBits::eR)
			_color |= storm::engine::ColorComponentFlag::R;
		if (color & vk::ColorComponentFlagBits::eG)
			_color |= storm::engine::ColorComponentFlag::G;
		if (color & vk::ColorComponentFlagBits::eB)
			_color |= storm::engine::ColorComponentFlag::B;
		if (color & vk::ColorComponentFlagBits::eA)
			_color |= storm::engine::ColorComponentFlag::A;

		return _color;
	}

	static inline constexpr const auto PIPELINE_STATE_MAP
	    = MappingArray<vk::PipelineStageFlagBits, storm::engine::PipelineStage,
	        static_cast<std::size_t>(storm::engine::PipelineStage::SIZE)> {
	        std::pair<vk::PipelineStageFlagBits, storm::engine::PipelineStage> {
	            vk::PipelineStageFlagBits::eColorAttachmentOutput,
	            storm::engine::PipelineStage::COLOR_ATTACHMENT_OUTPUT},
	    };

	vk::PipelineStageFlagBits asVK(
	    storm::engine::PipelineStage state) noexcept {
	    AS(state, PIPELINE_STATE_MAP)}

	storm::engine::PipelineStage
	    fromVK(vk::PipelineStageFlagBits state) noexcept {
		FROM(state, PIPELINE_STATE_MAP)
	}

	static inline constexpr const auto VERTEX_INPUT_RATE_MAP
	    = MappingArray<vk::VertexInputRate, storm::engine::VertexInputRate,
	        static_cast<std::size_t>(storm::engine::VertexInputRate::SIZE)> {
	        std::pair<vk::VertexInputRate, storm::engine::VertexInputRate> {
	            vk::VertexInputRate::eVertex,
	            storm::engine::VertexInputRate::PER_VERTEX},
	        {vk::VertexInputRate::eInstance,
	            storm::engine::VertexInputRate::PER_INSTANCE}};

	vk::VertexInputRate asVK(storm::engine::VertexInputRate type) noexcept {
	    AS(type, VERTEX_INPUT_RATE_MAP)}

	storm::engine::VertexInputRate fromVK(vk::VertexInputRate type) noexcept {
		FROM(type, VERTEX_INPUT_RATE_MAP)
	}

	static inline constexpr const auto FORMAT_MAP = MappingArray<vk::Format,
	    storm::engine::Format,
	    static_cast<std::size_t>(storm::engine::Format::SIZE)> {
	    std::pair<vk::Format, storm::engine::Format> {
	        vk::Format::eR32Sfloat, storm::engine::Format::FLOAT},
	    {vk::Format::eR32G32Sfloat, storm::engine::Format::FLOAT_VEC2},
	    {vk::Format::eR32G32B32Sfloat, storm::engine::Format::FLOAT_VEC3},
	    {vk::Format::eR32G32B32A32Sfloat, storm::engine::Format::FLOAT_VEC4},
	    {vk::Format::eR32Sint, storm::engine::Format::INT},
	    {vk::Format::eR32G32Sint, storm::engine::Format::INT_VEC2},
	    {vk::Format::eR32G32B32Sint, storm::engine::Format::INT_VEC3},
	    {vk::Format::eR32G32B32A32Sint, storm::engine::Format::INT_VEC4},
	    {vk::Format::eR32Uint, storm::engine::Format::UINT},
	    {vk::Format::eR32G32Uint, storm::engine::Format::UINT_VEC2},
	    {vk::Format::eR32G32B32Uint, storm::engine::Format::UINT_VEC3},
	    {vk::Format::eR32G32B32A32Uint,
	        storm::engine::Format::UINT_VEC4}, // TODO ADD MORE
	    {vk::Format::eB8G8R8A8Unorm, storm::engine::ColorFormat::BGRA8888UNORM},
	    {vk::Format::eB8G8R8A8Snorm, storm::engine::ColorFormat::BGRA8888SNORM},
	    {vk::Format::eB8G8R8A8Srgb, storm::engine::ColorFormat::BGRA8888SRGB},
	    {vk::Format::eR8G8B8A8Unorm, storm::engine::ColorFormat::RGBA8888UNORM},
	    {vk::Format::eR8G8B8A8Snorm, storm::engine::ColorFormat::RGBA8888SNORM},
	    {vk::Format::eR8G8B8A8Srgb, storm::engine::ColorFormat::RGBA8888SRGB},
	    {vk::Format::eD16Unorm, storm::engine::ColorFormat::D16},
	    {vk::Format::eD16UnormS8Uint, storm::engine::ColorFormat::D16S8},
	    {vk::Format::eD32Sfloat, storm::engine::ColorFormat::D32},
	    {vk::Format::eD32SfloatS8Uint, storm::engine::ColorFormat::D32S8},
	    {vk::Format::eD24UnormS8Uint, storm::engine::ColorFormat::D24S8UNORM},
	};

	vk::Format asVK(storm::engine::Format type) noexcept {AS(type, FORMAT_MAP)}

	storm::engine::Format fromVK(vk::Format type) noexcept {
		FROM(type, FORMAT_MAP)
	}

	static inline constexpr const auto COMPARE_OP_MAP
	    = MappingArray<vk::CompareOp, storm::engine::CompareOperation,
	        static_cast<std::size_t>(storm::engine::CompareOperation::SIZE)> {
	        std::pair<vk::CompareOp, storm::engine::CompareOperation> {
	            vk::CompareOp::eNever, storm::engine::CompareOperation::NEVER},
	        {vk::CompareOp::eLess, storm::engine::CompareOperation::LESS},
	        {vk::CompareOp::eEqual, storm::engine::CompareOperation::EQUAL},
	        {vk::CompareOp::eLessOrEqual,
	            storm::engine::CompareOperation::LESS_OR_EQUAL},
	        {vk::CompareOp::eGreater, storm::engine::CompareOperation::GREATER},
	        {vk::CompareOp::eNotEqual,
	            storm::engine::CompareOperation::NOT_EQUAL},
	        {vk::CompareOp::eGreaterOrEqual,
	            storm::engine::CompareOperation::GREATER_OR_EQUAL},
	        {vk::CompareOp::eAlways, storm::engine::CompareOperation::ALWAYS},

	    };

	vk::CompareOp asVK(storm::engine::CompareOperation op) noexcept {
		AS(op, COMPARE_OP_MAP)
	}

	storm::engine::CompareOperation fromVK(vk::CompareOp op) noexcept {
		FROM(op, COMPARE_OP_MAP)
	}

	vk::BufferUsageFlags asVK(storm::engine::BufferUsageFlag usage) noexcept {
		using namespace storm::engine;

		auto _usage = vk::BufferUsageFlags{};

		if((usage & BufferUsageFlag::VERTEX) == BufferUsageFlag::VERTEX)
			_usage |= vk::BufferUsageFlagBits::eVertexBuffer;
		if((usage & BufferUsageFlag::INDEX) == BufferUsageFlag::INDEX)
			_usage |= vk::BufferUsageFlagBits::eIndexBuffer;
		if((usage & BufferUsageFlag::UNIFORM) == BufferUsageFlag::UNIFORM)
			_usage |= vk::BufferUsageFlagBits::eUniformBuffer;
		if((usage & BufferUsageFlag::TRANSFER_SRC) == BufferUsageFlag::TRANSFER_SRC)
			_usage |= vk::BufferUsageFlagBits::eTransferSrc;
		if((usage & BufferUsageFlag::TRANSFER_DST) == BufferUsageFlag::TRANSFER_DST)
			_usage |= vk::BufferUsageFlagBits::eTransferDst;

		return _usage;
	}

	storm::engine::BufferUsageFlag fromVK(vk::BufferUsageFlags usage) noexcept {
		using namespace storm::engine;

		auto _usage = BufferUsageFlag{};

		if((usage & vk::BufferUsageFlagBits::eVertexBuffer) == vk::BufferUsageFlagBits::eVertexBuffer)
			_usage |= BufferUsageFlag::VERTEX;
		if((usage & vk::BufferUsageFlagBits::eIndexBuffer) == vk::BufferUsageFlagBits::eIndexBuffer)
			_usage |= BufferUsageFlag::INDEX;
		if((usage & vk::BufferUsageFlagBits::eUniformBuffer) == vk::BufferUsageFlagBits::eUniformBuffer)
			_usage |= BufferUsageFlag::UNIFORM;
		if((usage & vk::BufferUsageFlagBits::eTransferSrc) == vk::BufferUsageFlagBits::eTransferSrc)
			_usage |= BufferUsageFlag::TRANSFER_SRC;
		if((usage & vk::BufferUsageFlagBits::eTransferDst) == vk::BufferUsageFlagBits::eTransferDst)
			_usage |= BufferUsageFlag::TRANSFER_DST;

		return _usage;
	}

    vk::ImageUsageFlags asVK(storm::engine::ImageUsageFlag usage) noexcept {
        using namespace storm::engine;

        auto _usage = vk::ImageUsageFlags{};

        if((usage & ImageUsageFlag::COLOR_ATTACHMENT) == ImageUsageFlag::COLOR_ATTACHMENT)
            _usage |= vk::ImageUsageFlagBits::eColorAttachment;
        if((usage & ImageUsageFlag::DEPTH_STENCIL_ATTACHMENT) == ImageUsageFlag::DEPTH_STENCIL_ATTACHMENT)
            _usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        if((usage & ImageUsageFlag::INPUT_ATTACHMENT) == ImageUsageFlag::INPUT_ATTACHMENT)
            _usage |= vk::ImageUsageFlagBits::eInputAttachment;
        if((usage & ImageUsageFlag::TRANSFERT_SRC) == ImageUsageFlag::TRANSFERT_SRC)
            _usage |= vk::ImageUsageFlagBits::eTransferSrc;
        if((usage & ImageUsageFlag::TRANSFERT_DST) == ImageUsageFlag::TRANSFERT_DST)
            _usage |= vk::ImageUsageFlagBits::eTransferDst;
        if((usage & ImageUsageFlag::SAMPLED) == ImageUsageFlag::SAMPLED)
            _usage |= vk::ImageUsageFlagBits::eSampled;
        if((usage & ImageUsageFlag::STORAGE) == ImageUsageFlag::STORAGE)
            _usage |= vk::ImageUsageFlagBits::eStorage;

        return _usage;
    }

    static inline constexpr const auto IMAGE_LAYOUT_MAP =
      MappingArray<vk::ImageLayout, storm::engine::ImageLayout,
        static_cast<std::size_t>(storm::engine::ImageLayout::SIZE)> {
        std::pair<vk::ImageLayout, storm::engine::ImageLayout> {
          vk::ImageLayout::eColorAttachmentOptimal, storm::engine::ImageLayout::COLOR_ATTACHMENT_OPTIMAL
        },
        {vk::ImageLayout::eDepthStencilAttachmentOptimal, storm::engine::ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
        {vk::ImageLayout::eDepthStencilReadOnlyOptimal, storm::engine::ImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL},
        {vk::ImageLayout::eTransferSrcOptimal, storm::engine::ImageLayout::TRANSFERT_SRC_OPTIMAL},
        {vk::ImageLayout::eTransferDstOptimal, storm::engine::ImageLayout::TRANSFERT_DST_OPTIMAL},
        {vk::ImageLayout::eShaderReadOnlyOptimal, storm::engine::ImageLayout::SHADER_READ_ONLY_OPTIMAL},
    };

    vk::ImageLayout asVK(storm::engine::ImageLayout layout) noexcept {
        AS(layout, IMAGE_LAYOUT_MAP)
    }

    storm::engine::ImageLayout fromVK(vk::ImageLayout layout) noexcept {
        FROM(layout, IMAGE_LAYOUT_MAP)
    }

    vk::ImageAspectFlags asVK(storm::engine::ImageAspectFlag aspect) noexcept {
        using namespace storm::engine;

        auto _aspect = vk::ImageAspectFlags{};

        if((aspect & ImageAspectFlag::COLOR) == ImageAspectFlag::COLOR)
            _aspect |= vk::ImageAspectFlagBits::eColor;
        if((aspect & ImageAspectFlag::DEPTH) == ImageAspectFlag::DEPTH)
            _aspect |= vk::ImageAspectFlagBits::eDepth;
        if((aspect & ImageAspectFlag::STENCIL) == ImageAspectFlag::STENCIL)
            _aspect |= vk::ImageAspectFlagBits::eStencil;

        return _aspect;
    }
}
