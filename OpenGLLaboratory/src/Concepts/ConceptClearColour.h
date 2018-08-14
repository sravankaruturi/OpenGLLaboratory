#pragma once
#include "Concept.h"

namespace olab {
	namespace concepts {

		class ConceptClearColour : public Concept
		{

		private:

				float colour[4];

		public:
			ConceptClearColour();
			~ConceptClearColour();

			void OnUpdate(float _deltaTime) override;
			void OnRender(const Renderer& _renderer) override;
			void OnImGuiRender() override;
		};

	}
}
