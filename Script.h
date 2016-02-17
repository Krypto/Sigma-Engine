#ifndef SIGMA_SCRIPT_COMPONENT
#define SIGMA_SCRIPT_COMPONENT

#include "Component.h"


#include <string>
#include <fstream>
#include <streambuf>
#include <functional>
using namespace std;

namespace sig {

	class ScriptResource
	{
	public:
		ScriptResource(const string &fileName);

		string GetFileName() const;
		void SetFileName(const string &source);

	private:
		string m_fileName;
	};

	class Script : public Component
	{
	public:
		Script(ScriptResource *res);

		virtual void Initialize();
		virtual void Update(float dt);

		void Compile(const string &classname);
	private:
		ScriptResource *m_resource;

		function<void(float)>	m_update;
		function<void()>		m_initialize;
	};
}

#endif // SIGMA_SCRIPT_COMPONENT
