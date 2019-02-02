#pragma once

#include <utility>

#include <storm/core/Types.hpp>
#include <storm/core/Memory.hpp>

//#include <storm/engine/render/Texture.hpp>

namespace storm::engine {
	class STORM_PUBLIC Material final {
		public:
			Material();
			~Material();

			Material(const Material &);
			Material &operator=(const Material &);

			Material(Material &&);
			Material &operator=(Material &&);

			inline void setAmbiant(core::RGBColorF&& color)      noexcept { m_ambiant = std::move(color); }
			inline void setAmbiant(const core::RGBColorF& color) noexcept { m_ambiant = color; }
			inline const core::RGBColorF &ambiant() const noexcept { return m_ambiant; }

			inline void setDiffuse(core::RGBColorF&& color)      noexcept { m_diffuse = std::move(color); }
			inline void setDiffuse(const core::RGBColorF& color) noexcept { m_diffuse = color; }
			inline const core::RGBColorF &diffuse() const noexcept { return m_diffuse; }

			inline void setSpecular(core::RGBColorF&& color)      noexcept { m_specular = std::move(color); }
			inline void setSpecular(const core::RGBColorF& color) noexcept { m_specular = color; }
			inline const core::RGBColorF &specular() const noexcept { return m_specular; }

			inline void setEmissive(core::RGBColorF&& color)      noexcept { m_emissive = std::move(color); }
			inline void setEmissive(const core::RGBColorF& color) noexcept { m_emissive = color; }
			inline const core::RGBColorF &emissive() const noexcept { return m_emissive; }

			inline void setShinyness(float shinyness) noexcept { m_shinyness = shinyness; }
			inline float shinyness()            const noexcept { return m_shinyness; }

			bool operator==(const Material &other) const noexcept;
		private:
			core::RGBColorF m_ambiant;
			core::RGBColorF m_diffuse;
			core::RGBColorF m_specular;
			core::RGBColorF m_emissive;
			float           m_shinyness;

			friend struct std::hash<storm::engine::Material>;
	};
}
