#pragma once

#include <storm/engine/graphics/Geometry.hpp>
#include <storm/engine/scenegraph/SceneNode.hpp>
#include <vector>

namespace storm::engine {
    class STORM_PUBLIC GeometryNode : public SceneNode {
    public:
        SUR_Object(GeometryNode)

          using DataType = Geometry;

        ~GeometryNode() override;

        GeometryNode(GeometryNode &&);
        GeometryNode &operator=(GeometryNode &&);

        inline const DataType &data() const noexcept;
        inline void setGeometry(Geometry geometry) noexcept;

    protected:
        explicit GeometryNode(Scene &graph);

        DirtyType dirtyValue() const noexcept override;

        friend class Scene;

    private:
        DataType m_mesh;
    };
} // namespace storm::engine

#include "GeometryNode.inl"
