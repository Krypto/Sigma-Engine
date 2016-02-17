#include "Shader.h"
#include "SIG_Utilities.h"
#include <algorithm>
#include <string.h>

std::vector<std::string> split(std::string &s,
                               const std::string &delim)
{
	std::vector<std::string> res;

	int pos = 0;
	std::string token;
	while ((pos = s.find(delim)) != std::string::npos) {
		token = s.substr(0, pos);
		res.push_back(token);
		s.erase(0, pos + delim.length());
	}

	return res;
}

namespace sig
{
	Shader* Shader::FromFile(std::string vsf, std::string fsf)
	{
		return new Shader(vsf, fsf);
	}

	Shader* Shader::FromString(std::string vsf, std::string fsf)
	{
		return new Shader(vsf, fsf, 0);
	}

	Shader::Shader(std::string vsf, std::string fsf, int d)
	{
		vert_count = 1;
		frag_count = 1;
		m_valid = true;
		this->m_vert = SIG_CreateShader(vsf, Shader::AE_SHADER_TYPE_VERTEX);
		this->m_frag = SIG_CreateShader(fsf, Shader::AE_SHADER_TYPE_FRAGMENT);
		this->m_program = SIG_CreateProgram(this->m_vert, this->m_frag);

		SIG_ExtractUniforms(vsf);
		SIG_ExtractUniforms(fsf);
	}

	Shader::Shader(std::string vsf, std::string fsf)
	{
		std::string vss = "", fss = "";
		if (!vsf.empty()) {
			std::ifstream t(vsf);
			vss = std::string(	std::istreambuf_iterator<char>(t),
			                    std::istreambuf_iterator<char>());

			TestEOL test;
			vert_count = std::count_if(	std::istreambuf_iterator<char>(t),
			                            std::istreambuf_iterator<char>(),
			                            test);
			if (test.last != '\n') {
				++vert_count;
			}
		}

		if (!fsf.empty()) {
			std::ifstream t(fsf);
			fss = std::string((	std::istreambuf_iterator<char>(t)),
			                  std::istreambuf_iterator<char>());

			TestEOL test;
			frag_count = std::count_if(	std::istreambuf_iterator<char>(t),
			                            std::istreambuf_iterator<char>(),
			                            test);

			if (test.last != '\n') {
				++frag_count;
			}
		}

		m_valid = true;
		this->m_vert = SIG_CreateShader(vss, Shader::AE_SHADER_TYPE_VERTEX);
		this->m_frag = SIG_CreateShader(fss, Shader::AE_SHADER_TYPE_FRAGMENT);
		this->m_program = SIG_CreateProgram(this->m_vert, this->m_frag);

		SIG_ExtractUniforms(vss);
		SIG_ExtractUniforms(fss);
	}

	Shader::Shader()
		: Shader("", "")
	{
		m_valid = true;
		vert_count = 1;
		frag_count = 1;
	}

	bool Shader::AddUniform(const std::string &name)
	{
		int uloc = -1;
		uloc = glGetUniformLocationARB(this->m_program, name.c_str());

		// SIG_LOG("ADD UNIFORM ID: " << uloc << " -> " << name.c_str());

		if (uloc > -1) {
			m_uniforms.insert( { name, uloc });
			return true;
		}
		return false;
	}

	void Shader::SetFloat(std::string name, float value)
	{
		int uloc = SIG_FindUniform(name);
		if (uloc != -1) {
			glUniform1fARB(uloc, value);
		} else {
			SIG_LOG("Could not find uniform \"" << name << "\"");
		}
	}
	void Shader::SetInt(std::string name, int value)
	{
		int uloc = SIG_FindUniform(name);
		if (uloc != -1) {
			glUniform1iARB(uloc, value);
		} else {
			SIG_LOG("Could not find uniform \"" << name << "\"");
		}
	}

	void Shader::SetMatrix4(std::string name, Matrix4 &value)
	{
		int uloc = SIG_FindUniform(name);
		if (uloc != -1) {
			glUniformMatrix4fvARB(uloc, 1, GL_FALSE, &value.Raw()[0][0]);
		} else {
			SIG_LOG("Could not find uniform \"" << name << "\"");
		}
	}

	void Shader::SetVector4(std::string name, Vector4 value)
	{
		int uloc = SIG_FindUniform(name);
		if (uloc != -1) {
			glUniform4fARB(uloc, value.X(), value.Y(), value.Z(), value.W());
		} else {
			SIG_LOG("Could not find uniform \"" << name << "\"");
		}
	}
	void Shader::SetVector3(std::string name, Vector3 value)
	{
		int uloc = SIG_FindUniform(name);
		if (uloc != -1) {
			glUniform3fARB(uloc, value.X(), value.Y(), value.Z());
		} else {
			SIG_LOG("Could not find uniform \"" << name << "\"");
		}
	}
	void Shader::SetVector2(std::string name, Vector2 value)
	{
		int uloc = SIG_FindUniform(name);
		if (uloc != -1) {
			glUniform2fARB(uloc, value.X(), value.Y());
		} else {
			SIG_LOG("Could not find uniform \"" << name << "\"");
		}
	}

	void Shader::Bind()
	{
		glUseProgram(this->m_program);
	}

	int Shader::SIG_FindUniform(std::string name)
	{
		auto pos = m_uniforms.find(name);
		if (pos != m_uniforms.end())
			return pos->second;
		return -1;
	}

	GLuint Shader::SIG_CreateShader(std::string src, int type)
	{
		GLuint ret;
		int c = 1;

		switch (type) {
			default:
			case Shader::AE_SHADER_TYPE_VERTEX:
				ret = glCreateShader(GL_VERTEX_SHADER);
				c = vert_count;
				break;
			case Shader::AE_SHADER_TYPE_FRAGMENT:
				ret = glCreateShader(GL_FRAGMENT_SHADER);
				c = frag_count;
				break;
		}

		const char* shaderSource = src.c_str();
		glShaderSourceARB(ret, c, (const GLchar**)&shaderSource, nullptr);

		glCompileShaderARB(ret);

		const char* typetext = type == Shader::AE_SHADER_TYPE_VERTEX ? "VERTEX_SHADER" : "FRAGMENT_SHADER";

		int compiled = GL_FALSE;
		glGetShaderiv(ret, GL_COMPILE_STATUS, &compiled);

		if (compiled != GL_TRUE) {
			m_valid = false;

			int bufflen = 1;
			glGetShaderiv(ret, GL_INFO_LOG_LENGTH, &bufflen);

			if (bufflen > 1) {
				GLchar* log_string = new char[bufflen + 1];
				glGetShaderInfoLog(ret, bufflen, 0, log_string);
				SIG_LOG(src);
				SIG_LOG(typetext << " ERROR: ");
				SIG_LOG(log_string);
			}

			glDeleteShader(ret);
			return -1;
		}

		return ret;
	}

	GLuint Shader::SIG_CreateProgram(GLuint vs, GLuint fs)
	{
		GLuint prog = glCreateProgram();

		if (vs != 0) {
			glAttachShader(prog, vs);
		}

		if (fs != 0) {
			glAttachShader(prog, fs);
		}

		glLinkProgramARB(prog);

		int linkStatus;
		glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);

		if (linkStatus != GL_TRUE) {
			m_valid = false;

			int bufflen = 1;
			glGetProgramivARB(prog, GL_INFO_LOG_LENGTH, &bufflen);

			GLchar* log_string = new char[bufflen + 1];
			glGetProgramInfoLog(prog, bufflen, 0, log_string);
			SIG_LOG("SHADER_PROGRAM_ERROR: Could not link program:");
			SIG_LOG(log_string);

			glDeleteProgram(prog);

			return 0;
		}

		return prog;
	}

}
void sig::Shader::Unbind()
{
	glUseProgram(0);
}

void sig::Shader::SIG_ExtractUniforms(std::string src)
{
	std::vector<std::string> spl = split(src, ";");

	for (auto it = spl.begin(); it != spl.end(); ++it) {
		std::string ln = (*it);
		ln.erase(0, ln.find_first_not_of(" \n\r\t"));
		ln.erase(ln.find_last_not_of(" \n\r\t")+1);

		std::string uni = "";
		if (strncmp(ln.c_str(), "uniform", 7) == 0) {
			for (int i = ln.size()-1; i >= 0; i--) {
				if (ln[i] != ';' && ln[i] != ' ') {
					uni += ln[i];
				} else {
					break;
				}
			}
			std::reverse(uni.begin(), uni.end());
			AddUniform(uni);
		}
	}
}

bool sig::Shader::HasUniform(const std::string& name)
{
	return SIG_FindUniform(name) != -1;
}
