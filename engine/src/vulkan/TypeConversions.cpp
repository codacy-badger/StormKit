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

	vk::ColorComponentFlags asVK(storm::engine::ColorComponent color) noexcept {
		auto _color = vk::ColorComponentFlags {};

		if ((color & storm::engine::ColorComponent::R)
		    == storm::engine::ColorComponent::R)
			_color |= vk::ColorComponentFlagBits::eR;
		if ((color & storm::engine::ColorComponent::G)
		    == storm::engine::ColorComponent::G)
			_color |= vk::ColorComponentFlagBits::eG;
		if ((color & storm::engine::ColorComponent::B)
		    == storm::engine::ColorComponent::B)
			_color |= vk::ColorComponentFlagBits::eB;
		if ((color & storm::engine::ColorComponent::A)
		    == storm::engine::ColorComponent::A)
			_color |= vk::ColorComponentFlagBits::eA;

		return _color;
	}

	storm::engine::ColorComponent fromVK(
	    vk::ColorComponentFlags color) noexcept {
		auto _color = storm::engine::ColorComponent {};

		if (color & vk::ColorComponentFlagBits::eR)
			_color |= storm::engine::ColorComponent::R;
		if (color & vk::ColorComponentFlagBits::eG)
			_color |= storm::engine::ColorComponent::G;
		if (color & vk::ColorComponentFlagBits::eB)
			_color |= storm::engine::ColorComponent::B;
		if (color & vk::ColorComponentFlagBits::eA)
			_color |= storm::engine::ColorComponent::A;

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

	vk::BufferUsageFlags asVK(storm::engine::BufferUsage usage) noexcept {
		using namespace storm::engine;

		auto _usage = vk::BufferUsageFlags{};

		if((usage & BufferUsage::VERTEX) == BufferUsage::VERTEX)
			_usage |= vk::BufferUsageFlagBits::eVertexBuffer;
		if((usage & BufferUsage::INDEX) == BufferUsage::INDEX)
			_usage |= vk::BufferUsageFlagBits::eIndexBuffer;
		if((usage & BufferUsage::UNIFORM) == BufferUsage::UNIFORM)
			_usage |= vk::BufferUsageFlagBits::eUniformBuffer;
		if((usage & BufferUsage::TRANSFER_SRC) == BufferUsage::TRANSFER_SRC)
			_usage |= vk::BufferUsageFlagBits::eTransferSrc;
		if((usage & BufferUsage::TRANSFER_DST) == BufferUsage::TRANSFER_DST)
			_usage |= vk::BufferUsageFlagBits::eTransferDst;

		return _usage;
	}

	storm::engine::BufferUsage fromVK(vk::BufferUsageFlags usage) noexcept {
		using namespace storm::engine;

		auto _usage = BufferUsage{};

		if((usage & vk::BufferUsageFlagBits::eVertexBuffer) == vk::BufferUsageFlagBits::eVertexBuffer)
			_usage |= BufferUsage::VERTEX;
		if((usage & vk::BufferUsageFlagBits::eIndexBuffer) == vk::BufferUsageFlagBits::eIndexBuffer)
			_usage |= BufferUsage::INDEX;
		if((usage & vk::BufferUsageFlagBits::eUniformBuffer) == vk::BufferUsageFlagBits::eUniformBuffer)
			_usage |= BufferUsage::UNIFORM;
		if((usage & vk::BufferUsageFlagBits::eTransferSrc) == vk::BufferUsageFlagBits::eTransferSrc)
			_usage |= BufferUsage::TRANSFER_SRC;
		if((usage & vk::BufferUsageFlagBits::eTransferDst) == vk::BufferUsageFlagBits::eTransferDst)
			_usage |= BufferUsage::TRANSFER_DST;

		return _usage;
	}

	/*static inline constexpr const auto TEXTURE_TYPE_MAP =
MappingArray<vk::ImageViewType, storm::engine:,
static_cast<std::size_t>(storm::engine::ColorType::SIZE)> {
	std::pair<vk::ImageViewType, storm::engine::ColorFormat>
	{vk::ImageViewType::e1D, storm::engine::ColorFormat::TEXTURE_1D},
	{vk::ImageViewType::e2D, storm::engine::ColorFormat::TEXTURE_2D},
	{vk::ImageViewType::e3D, storm::engine::ColorFormat::TEXTURE_3D},
	{vk::ImageViewType::eCube, storm::engine::ColorFormat::TEXTURE_CUBE_MAP},
	{vk::ImageViewType::e1DArray, storm::engine::ColorFormat::TEXTURE_1D_ARRAY},
	{vk::ImageViewType::e2DArray, storm::engine::ColorFormat::TEXTURE_2D_ARRAY},
	{vk::ImageViewType::eCubeArray,
storm::engine::ColorFormat::TEXTURE_CUBE_MAP_ARRAY},
};

inline vk::ImageViewType asVK(storm::engine::ColorFormat type) noexcept {
	AS(type, TEXTURE_TYPE_MAP)
}*/
	/*
static inline constexpr const auto SHADER_TYPE_MAP =
MappingArray<vk::ShaderStageFlagBits, storm::engine::ShaderStage,
static_cast<std::size_t>(storm::engine::ShaderStage::SIZE)> {
	std::pair<vk::ShaderStageFlagBits, storm::engine::ShaderStage>
	{vk::ShaderStageFlagBits::eVertex, storm::engine::ShaderStage::VERTEX},
	{vk::ShaderStageFlagBits::eFragment, storm::engine::ShaderStage::FRAGMENT},
	{vk::ShaderStageFlagBits::eGeometry, storm::engine::ShaderStage::GEOMETRY},
	{vk::ShaderStageFlagBits::eCompute, storm::engine::ShaderStage::COMPUTE},
	{vk::ShaderStageFlagBits::eTessellationControl,
storm::engine::ShaderStage::TESSELATION_CONTROL},
	{vk::ShaderStageFlagBits::eTessellationEvaluation,
storm::engine::ShaderStage::TESSELATION_EVALUATION},
};

template<>
inline vk::ShaderStageFlagBits asVK(storm::engine::ShaderStage type) noexcept {
	AS(type, SHADER_TYPE_MAP)
}

static inline constexpr const auto TEXTURE_FORMAT_MAP = MappingArray<vk::Format,
storm::engine::TextureFormat,
static_cast<std::size_t>(storm::engine::TextureFormat::SIZE)> {
	std::pair<vk::Format, storm::engine::TextureFormat>
	{vk::Format::eB8G8R8A8Unorm, storm::engine::TextureFormat::BGRA8888UNORM},
	{vk::Format::eB8G8R8A8Snorm, storm::engine::TextureFormat::BGRA8888SNORM},
	{vk::Format::eB8G8R8A8Srgb, storm::engine::TextureFormat::BGRA8888SRGB},
	{vk::Format::eR8G8B8A8Unorm, storm::engine::TextureFormat::RGBA8888UNORM},
	{vk::Format::eR8G8B8A8Snorm, storm::engine::TextureFormat::RGBA8888SNORM},
	{vk::Format::eR8G8B8A8Srgb, storm::engine::TextureFormat::RGBA8888SRGB},
	{vk::Format::eD32SfloatS8Uint,
storm::engine::TextureFormat::D32SFLOATS8UINT},
};

template<>
inline vk::Format asVK(storm::engine::TextureFormat type) noexcept {
	AS(type, TEXTURE_FORMAT_MAP)
}

template<>
inline storm::engine::TextureFormat fromVK(vk::Format type) noexcept {
	FROM(type, TEXTURE_FORMAT_MAP)
}

static inline constexpr const auto TEXTURE_COLORSPACE_MAP =
MappingArray<vk::ColorSpaceKHR, storm::engine::TextureColorSpace,
static_cast<std::size_t>(storm::engine::TextureColorSpace::SIZE)> {
	std::pair<vk::ColorSpaceKHR, storm::engine::TextureColorSpace>
	{vk::ColorSpaceKHR::eSrgbNonlinear,
storm::engine::TextureColorSpace::SRGB_NLINEAR}
};

template<>
inline vk::ColorSpaceKHR asVK(storm::engine::TextureColorSpace type) noexcept {
	AS(type, TEXTURE_COLORSPACE_MAP)
}

template<>
inline storm::engine::TextureColorSpace fromVK(vk::ColorSpaceKHR type) noexcept
{ FROM(type, TEXTURE_COLORSPACE_MAP)
}

static inline constexpr const auto TOPOLOGY_MAP =
MappingArray<vk::PrimitiveTopology, storm::engine::PrimitiveTopology,
static_cast<std::size_t>(storm::engine::PrimitiveTopology::SIZE)> {
	std::pair<vk::PrimitiveTopology, storm::engine::PrimitiveTopology>
	{vk::PrimitiveTopology::eTriangleList,
storm::engine::PrimitiveTopology::TRIANGLES},
	{vk::PrimitiveTopology::eTriangleStrip,
storm::engine::PrimitiveTopology::TRIANGLE_STRIP},
};

template <>
inline vk::PrimitiveTopology asVK(storm::engine::PrimitiveTopology topology)
noexcept { AS(topology, TOPOLOGY_MAP)
}

template <>
inline storm::engine::PrimitiveTopology fromVK(vk::PrimitiveTopology topology)
noexcept { FROM(topology, TOPOLOGY_MAP)
}

static inline constexpr const auto POLYGON_MODE_MAP =
MappingArray<vk::PolygonMode, storm::engine::PolygonMode,
static_cast<std::size_t>(storm::engine::PolygonMode::SIZE)> {
	std::pair<vk::PolygonMode, storm::engine::PolygonMode>
	{vk::PolygonMode::eFill, storm::engine::PolygonMode::FILL},
	{vk::PolygonMode::eLine, storm::engine::PolygonMode::LINE},
	{vk::PolygonMode::ePoint, storm::engine::PolygonMode::POINT},
};

template <>
inline vk::PolygonMode asVK(storm::engine::PolygonMode mode) noexcept {
	AS(mode, POLYGON_MODE_MAP)
}

template <>
inline storm::engine::PolygonMode fromVK(vk::PolygonMode mode) noexcept {
	FROM(mode, POLYGON_MODE_MAP)
}

static inline const auto CULL_MODE_MAP = MappingArray<vk::CullModeFlags,
storm::engine::CullMode,
static_cast<std::size_t>(storm::engine::CullMode::SIZE)> {
	std::pair<vk::CullModeFlags, storm::engine::CullMode>
	{vk::CullModeFlagBits::eBack, storm::engine::CullMode::BACK},
	{vk::CullModeFlagBits::eFront, storm::engine::CullMode::FRONT},
	{vk::CullModeFlagBits::eFrontAndBack,
storm::engine::CullMode::FRONT_AND_BACK}, {vk::CullModeFlagBits::eNone,
storm::engine::CullMode::NONE},
};

template <>
inline vk::CullModeFlags asVK(storm::engine::CullMode mode) noexcept {
	AS(mode, CULL_MODE_MAP)
}

template <>
inline storm::engine::CullMode fromVK(vk::CullModeFlags mode) noexcept {
	FROM(mode, CULL_MODE_MAP)
}

static inline constexpr const auto FRONT_FACE_MAP = MappingArray<vk::FrontFace,
storm::engine::FrontFace,
static_cast<std::size_t>(storm::engine::FrontFace::SIZE)> {
	std::pair<vk::FrontFace, storm::engine::FrontFace>
	{vk::FrontFace::eClockwise, storm::engine::FrontFace::CLOCKWISE},
	{vk::FrontFace::eCounterClockwise,
storm::engine::FrontFace::COUNTER_CLOCKWISE},
};

template <>
inline vk::FrontFace asVK(storm::engine::FrontFace face) noexcept {
	AS(face, FRONT_FACE_MAP)
}

template <>
inline storm::engine::FrontFace fromVK(vk::FrontFace face) noexcept {
	FROM(face, FRONT_FACE_MAP)
}

template <>
inline vk::ColorComponentFlags asVK(storm::engine::ColorComponent color)
noexcept { auto _color = vk::ColorComponentFlags{};

	if((color & storm::engine::ColorComponent::R) ==
storm::engine::ColorComponent::R) _color |= vk::ColorComponentFlagBits::eR;
	if((color & storm::engine::ColorComponent::G) ==
storm::engine::ColorComponent::G) _color |= vk::ColorComponentFlagBits::eG;
	if((color & storm::engine::ColorComponent::B) ==
storm::engine::ColorComponent::B) _color |= vk::ColorComponentFlagBits::eB;
	if((color & storm::engine::ColorComponent::A) ==
storm::engine::ColorComponent::A) _color |= vk::ColorComponentFlagBits::eA;

	return _color;
}

template <>
inline storm::engine::ColorComponent fromVK(vk::ColorComponentFlags color)
noexcept { auto _color = storm::engine::ColorComponent{};

	if(color & vk::ColorComponentFlagBits::eR)
	    _color |= storm::engine::ColorComponent::R;
	if(color & vk::ColorComponentFlagBits::eG)
	    _color |= storm::engine::ColorComponent::G;
	if(color & vk::ColorComponentFlagBits::eB)
	    _color |= storm::engine::ColorComponent::B;
	if(color & vk::ColorComponentFlagBits::eA)
	    _color |= storm::engine::ColorComponent::A;

	return _color;
}

static inline constexpr const auto LOGIC_OPERATION_MAP =
MappingArray<vk::LogicOp, storm::engine::LogicOperation,
static_cast<std::size_t>(storm::engine::LogicOperation::SIZE)> {
	std::pair<vk::LogicOp, storm::engine::LogicOperation>
	{vk::LogicOp::eAnd, storm::engine::LogicOperation::AND},
	{vk::LogicOp::eAndInverted, storm::engine::LogicOperation::AND_INVERTED},
	{vk::LogicOp::eAndReverse, storm::engine::LogicOperation::AND_REVERSE},
	{vk::LogicOp::eClear, storm::engine::LogicOperation::CLEAR},
	{vk::LogicOp::eCopy, storm::engine::LogicOperation::COPY},
	{vk::LogicOp::eCopyInverted, storm::engine::LogicOperation::COPY_INVERTED},
	{vk::LogicOp::eEquivalent, storm::engine::LogicOperation::EQUIVALENT},
	{vk::LogicOp::eInvert, storm::engine::LogicOperation::INVERT},
	{vk::LogicOp::eNand, storm::engine::LogicOperation::NAND},
	{vk::LogicOp::eNor, storm::engine::LogicOperation::NOR},
	{vk::LogicOp::eNoOp, storm::engine::LogicOperation::NO_OPERATION},
	{vk::LogicOp::eOr, storm::engine::LogicOperation::OR},
	{vk::LogicOp::eOrInverted, storm::engine::LogicOperation::OR_INVERTED},
	{vk::LogicOp::eOrReverse, storm::engine::LogicOperation::OR_REVERSE},
	{vk::LogicOp::eSet, storm::engine::LogicOperation::SET},
	{vk::LogicOp::eXor, storm::engine::LogicOperation::XOR},
};

template <>
inline vk::LogicOp asVK(storm::engine::LogicOperation operation) noexcept {
	AS(operation, LOGIC_OPERATION_MAP)
}

template <>
inline storm::engine::LogicOperation fromVK(vk::LogicOp operation) noexcept {
	FROM(operation, LOGIC_OPERATION_MAP)
}

static inline constexpr const auto PHYSICAL_DEVICE_TYPE_MAP =
MappingArray<vk::PhysicalDeviceType, storm::engine::PhysicalDevice::Type,
static_cast<std::size_t>(storm::engine::PhysicalDevice::Type::SIZE)> {
	std::pair<vk::PhysicalDeviceType, storm::engine::PhysicalDevice::Type>
	{vk::PhysicalDeviceType::eIntegratedGpu,
storm::engine::PhysicalDevice::Type::INTEGRATED},
	{vk::PhysicalDeviceType::eDiscreteGpu,
storm::engine::PhysicalDevice::Type::DISCRETE}, {vk::PhysicalDeviceType::eCpu,
storm::engine::PhysicalDevice::Type::CPU}, {vk::PhysicalDeviceType::eVirtualGpu,
storm::engine::PhysicalDevice::Type::VIRTUAL}, {vk::PhysicalDeviceType::eOther,
storm::engine::PhysicalDevice::Type::OTHER}
};

template <>
inline vk::PhysicalDeviceType asVK(storm::engine::PhysicalDevice::Type type)
noexcept{ AS(type, PHYSICAL_DEVICE_TYPE_MAP)
}

template <>
inline storm::engine::PhysicalDevice::Type fromVK(vk::PhysicalDeviceType type)
noexcept { FROM(type, PHYSICAL_DEVICE_TYPE_MAP)
}

static inline constexpr const auto ATTACHMENT_LOAD_OP_TYPE_MAP =
MappingArray<vk::AttachmentLoadOp, storm::engine::AttachmentLoadOp,
static_cast<std::size_t>(storm::engine::AttachmentLoadOp::SIZE)> {
	std::pair<vk::AttachmentLoadOp, storm::engine::AttachmentLoadOp>
	{vk::AttachmentLoadOp::eLoad, storm::engine::AttachmentLoadOp::LOAD},
	{vk::AttachmentLoadOp::eClear, storm::engine::AttachmentLoadOp::CLEAR},
	{vk::AttachmentLoadOp::eDontCare,
storm::engine::AttachmentLoadOp::DONT_CARE}
};

template <>
inline vk::AttachmentLoadOp asVK(storm::engine::AttachmentLoadOp type) noexcept{
	AS(type, ATTACHMENT_LOAD_OP_TYPE_MAP)
}

template <>
inline storm::engine::AttachmentLoadOp fromVK(vk::AttachmentLoadOp type)
noexcept { FROM(type, ATTACHMENT_LOAD_OP_TYPE_MAP)
}

static inline constexpr const auto ATTACHMENT_STORE_OP_TYPE_MAP =
MappingArray<vk::AttachmentStoreOp, storm::engine::AttachmentStoreOp,
static_cast<std::size_t>(storm::engine::AttachmentStoreOp::SIZE)> {
	std::pair<vk::AttachmentStoreOp, storm::engine::AttachmentStoreOp>
	{vk::AttachmentStoreOp::eStore, storm::engine::AttachmentStoreOp::STORE},
	{vk::AttachmentStoreOp::eDontCare,
storm::engine::AttachmentStoreOp::DONT_CARE}
};

template <>
inline vk::AttachmentStoreOp asVK(storm::engine::AttachmentStoreOp type)
noexcept { AS(type, ATTACHMENT_STORE_OP_TYPE_MAP)
}

template <>
inline storm::engine::AttachmentStoreOp fromVK(vk::AttachmentStoreOp type)
noexcept { FROM(type, ATTACHMENT_STORE_OP_TYPE_MAP)
}

static inline constexpr const auto VERTEX_INPUT_RATE_MAP =
MappingArray<vk::VertexInputRate, storm::engine::VertexInputRate,
static_cast<std::size_t>(storm::engine::VertexInputRate::SIZE)> {
	std::pair<vk::VertexInputRate, storm::engine::VertexInputRate>
	{vk::VertexInputRate::eVertex, storm::engine::VertexInputRate::PER_VERTEX},
	{vk::VertexInputRate::eInstance,
storm::engine::VertexInputRate::PER_INSTANCE}
};

template <>
inline vk::VertexInputRate asVK(storm::engine::VertexInputRate type) noexcept {
	AS(type, VERTEX_INPUT_RATE_MAP)
}
template <>
inline storm::engine::VertexInputRate fromVK(vk::VertexInputRate type) noexcept
{ FROM(type, VERTEX_INPUT_RATE_MAP)
}

static inline constexpr const auto FORMAT_MAP = MappingArray<vk::Format,
storm::engine::Format, static_cast<std::size_t>(storm::engine::Format::SIZE)> {
	std::pair<vk::Format, storm::engine::Format>
	{vk::Format::eR32Sfloat, storm::engine::Format::FLOAT},
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
	{vk::Format::eR32G32B32A32Uint, storm::engine::Format::UINT_VEC4} //TODO ADD
MORE
};


template <>
inline vk::Format asVK(storm::engine::Format type) noexcept {
	AS(type, FORMAT_MAP)
}
template <>
inline storm::engine::Format fromVK(vk::Format type) noexcept {
	FROM(type, FORMAT_MAP)
}

template <>
inline vk::BufferUsageFlags asVK(storm::engine::BufferUsage usage) noexcept {
	auto usage_ = vk::BufferUsageFlags{};

	if((usage & storm::engine::BufferUsage::VERTEX_BIT) ==
storm::engine::BufferUsage::VERTEX_BIT) usage_ |=
vk::BufferUsageFlagBits::eVertexBuffer; if((usage &
storm::engine::BufferUsage::INDEX_BIT) == storm::engine::BufferUsage::INDEX_BIT)
	    usage_ |= vk::BufferUsageFlagBits::eIndexBuffer;
	if((usage & storm::engine::BufferUsage::UNIFORM_BIT) ==
storm::engine::BufferUsage::UNIFORM_BIT) usage_ |=
vk::BufferUsageFlagBits::eUniformBuffer; if((usage &
storm::engine::BufferUsage::TRANSFER_DEST_BIT) ==
storm::engine::BufferUsage::TRANSFER_DEST_BIT) usage_ |=
vk::BufferUsageFlagBits::eTransferDst; if((usage &
storm::engine::BufferUsage::TRANSFER_SRC_BIT) ==
storm::engine::BufferUsage::TRANSFER_SRC_BIT) usage_ |=
vk::BufferUsageFlagBits::eTransferSrc;

	return usage_;
}

template <>
inline storm::engine::BufferUsage fromVK(vk::BufferUsageFlags usage) noexcept {
	auto usage_ = storm::engine::BufferUsage{};

	if(usage & vk::BufferUsageFlagBits::eVertexBuffer)
	    usage_ |=storm::engine::BufferUsage::VERTEX_BIT;
	if(usage & vk::BufferUsageFlagBits::eIndexBuffer)
	    usage_ |= storm::engine::BufferUsage::INDEX_BIT;
	if(usage & vk::BufferUsageFlagBits::eUniformBuffer)
	    usage_ |= storm::engine::BufferUsage::UNIFORM_BIT;
	if(usage & vk::BufferUsageFlagBits::eTransferDst)
	    usage_ |=  storm::engine::BufferUsage::TRANSFER_DEST_BIT;
	if(usage & vk::BufferUsageFlagBits::eTransferSrc)
	    usage_ |=  storm::engine::BufferUsage::TRANSFER_SRC_BIT;

	return usage_;
}
*/
}
