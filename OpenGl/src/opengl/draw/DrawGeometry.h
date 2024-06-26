#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawGeometry : public Draw {
	public:
		virtual ~DrawGeometry() {}

		virtual void Bind() const override;//绑定顶点数据
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//绘制

	};
}