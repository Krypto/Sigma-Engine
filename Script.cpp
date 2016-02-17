#include "Script.h"

sig::Script::Script(ScriptResource *res)
	:
		m_resource(res)
{

}

void sig::Script::Initialize()
{
	if (m_initialize) {
		m_initialize();
	}
}

void sig::Script::Update(float dt)
{
	if (m_update) {
		m_update(dt);
	}
}

void sig::Script::Compile(const string &classname)
{
	if (m_resource == nullptr) { return; }

}

sig::ScriptResource::ScriptResource(const string &fileName)
{
	m_fileName = fileName;
}

std::string sig::ScriptResource::GetFileName() const
{
	return m_fileName;
}

void sig::ScriptResource::SetFileName(const string &source)
{
	m_fileName = source;
}
