#pragma once
#include <string>
#include <vector>
#include <functional>

namespace olab {
	namespace concepts {

		class Concept
		{

		public:

			Concept() = default;
			virtual ~Concept() = default;

			virtual void OnUpdate(float _deltaTime) {}
			virtual void OnRender() {}
			virtual void OnImGuiRender() {}
		};

		class ConceptsMenu : public Concept {

		public:

			explicit ConceptsMenu(Concept*& _conceptPointer);

			void OnImGuiRender() override;

			template<typename T>
			void RegisterConcept(const std::string& _name);

		private:

			// Reference to the Current Selected Pointer. It is defined and is used outside the scope of this class as well. So we just use the reference to that pointer.
			Concept*& currenConcept;

			// A list of strings and the Concept Classes.
			std::vector < std::pair<std::string, std::function<Concept *()> >> concepts;

		};

		template <typename T>
		void ConceptsMenu::RegisterConcept(const std::string& _name)
		{
			std::cout << "Registered Test : " << _name << std::endl;
			concepts.push_back(std::make_pair(_name, []() {return new T(); }));
		}
	}
}
