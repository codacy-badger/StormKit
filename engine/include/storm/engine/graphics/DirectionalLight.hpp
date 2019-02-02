#pragma once

#include <storm/core/Types.hpp>
#include <storm/engine/render/Types.hpp>

namespace storm::engine {
	class DirectionalLight {
		public:
			DirectionalLight();
			~DirectionalLight();

			DirectionalLight(const DirectionalLight &);
			DirectionalLight &operator=(const DirectionalLight &);

			DirectionalLight(DirectionalLight &&);
			DirectionalLight &operator=(DirectionalLight &&);

			inline void setPosition(glm::vec3&& position)      noexcept { m_position = std::move(position); }
			inline void setPosition(const glm::vec3& position) noexcept { m_position = position; }
			inline const glm::vec3 &position() const noexcept { return m_position; }

			inline void setAmbiant(core::RGBColorF&& color)      noexcept { m_ambiant = std::move(color); }
			inline void setAmbiant(const core::RGBColorF& color) noexcept { m_ambiant = color; }
			inline const core::RGBColorF &ambiant() const noexcept { return m_ambiant; }

			inline void setDiffuse(core::RGBColorF&& color)      noexcept { m_diffuse = std::move(color); }
			inline void setDiffuse(const core::RGBColorF& color) noexcept { m_diffuse = color; }
			inline const core::RGBColorF &diffuse() const noexcept { return m_diffuse; }

			inline void setSpecular(core::RGBColorF&& color)      noexcept { m_specular = std::move(color); }
			inline void setSpecular(const core::RGBColorF& color) noexcept { m_specular = color; }
			inline const core::RGBColorF &specular() const noexcept { return m_specular; }
		private:
			vec3       m_position;
			core::RGBColorF m_ambiant;
			core::RGBColorF m_diffuse;
			core::RGBColorF m_specular;
			
			friend struct std::hash<storm::engine::DirectionalLight>;
	};
}
