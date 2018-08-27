#pragma once
#include "ConceptModelViewProjection.h"

namespace olab
{
	namespace concepts
	{
		class ConceptStencilTest : public ConceptModelViewProjection
		{

		protected:

			bool cubeSelected = false;

		public:

			ConceptStencilTest();
			~ConceptStencilTest() = default;

			void OnImGuiRender() override;

			void OnUpdate(float _deltaTime) override;

		};
	}
}

