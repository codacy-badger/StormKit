#pragma once
#include <storm/core/Memory.hpp>

#include <string>

namespace storm::tools {
	class STORM_PUBLIC TreeNode final {
		public:
			using Index = std::uint32_t;
			using DirtyBits = std::uint32_t;

			constexpr static const Index INVALID_INDEX = ~0U;

			TreeNode() : m_parent(INVALID_INDEX), m_next_sibling(INVALID_INDEX), m_first_child(INVALID_INDEX), m_dirty_bits(0) {

			}
			~TreeNode() = default;

			TreeNode(const TreeNode &node) = default;
			TreeNode(TreeNode &&node) = default;
			TreeNode &operator=(const TreeNode &node) = default;
			TreeNode &operator=(TreeNode &&node) = default;

			inline const std::string &name() const noexcept { return m_name; }
			template <typename T = std::string, typename = std::enable_if_t<std::is_same_v<std::decay_t<std::remove_cv_t<T>>, std::string>>>
			inline void setName(T &&name) noexcept { m_name = std::forward<T>(name); }

			inline Index parent() const noexcept { return m_parent; }
			inline void setParent(Index index) noexcept { m_parent = index; }

			inline Index nextSibling() const noexcept { return m_next_sibling; }
			inline void setNextSibling(Index index) noexcept { m_next_sibling = index; }

			inline Index firstChild() const noexcept { return m_first_child; }
			inline void setFirstChild(Index index) noexcept { m_first_child = index; }

			inline const DirtyBits &dirtyBits() const noexcept { return m_dirty_bits; }
			inline void setDirtyBits(DirtyBits bits) noexcept  { m_dirty_bits = bits; }

			void invalidate();
		private:
			Index m_parent;
			Index m_next_sibling;
			Index m_first_child;
			DirtyBits m_dirty_bits;

			std::string m_name;
	};
}
