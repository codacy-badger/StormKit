#pragma once
#include <storm/core/Memory.hpp>
#include <string>

namespace storm::tools {
	class STORM_PUBLIC TreeNode final {
	public:
		using Index     = std::uint32_t;
		using DirtyBits = std::uint32_t;

		constexpr static const Index INVALID_INDEX = ~0U;

		TreeNode()
		    : m_parent(INVALID_INDEX), m_next_sibling(INVALID_INDEX),
		      m_first_child(INVALID_INDEX), m_dirty_bits(0) {}
		~TreeNode() = default;

		TreeNode(const TreeNode &node) = default;
		TreeNode(TreeNode &&node)      = default;
		TreeNode &operator=(const TreeNode &node) = default;
		TreeNode &operator=(TreeNode &&node) = default;

		inline const std::string &name() const noexcept;
		inline void               setName(std::string_view name) noexcept;

		inline Index parent() const noexcept;
		inline void  setParent(Index index) noexcept;

		inline Index nextSibling() const noexcept;
		inline void  setNextSibling(Index index) noexcept;

		inline Index firstChild() const noexcept;
		inline void  setFirstChild(Index index) noexcept;

		inline const DirtyBits &dirtyBits() const noexcept;
		inline void             setDirtyBits(DirtyBits bits) noexcept;

		void invalidate();

	private:
		Index     m_parent;
		Index     m_next_sibling;
		Index     m_first_child;
		DirtyBits m_dirty_bits;

		std::string m_name;
	};
}

#include "TreeNode.inl"
