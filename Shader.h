#ifndef AE_SHADER
#define AE_SHADER

#include "SIG_OpenGL.h"
#include "Vector.h"
#include "Matrix4.h"

#include <string>
#include <fstream>
#include <streambuf>

#include <map>

namespace sig
{
	using namespace math;

	struct TestEOL {
		bool operator ()(char c) {
			last		=	c;
			return last	==	'\n';
		}
		char last;
	};

	class Shader
	{
	public:
		Shader();
		Shader(std::string vsf, std::string fsf, int d);
		Shader(std::string vsf, std::string fsf);

		/**
		 * @brief Create a Shader from file
		 * @param vsf Vertex Shader file name
		 * @param fsf Fragment Shader file name
		 * @return Shader Object
		 */
		static Shader* FromFile(std::string vsf, std::string fsf);
		static Shader* FromString(std::string vsf, std::string fsf);
		
		/**
		 * @brief Unbind the currently bound shader.
		 */
		static void Unbind();
		
		/**
		 * @brief Add an uniform from the shader
		 * @param name Uniform name
		 * @return True if success, False otherwise
		 */
		bool AddUniform(const std::string &name);
		
		bool HasUniform(const std::string &name);
		
		/**
		 * @brief Use this shader
		 */
		void Bind();

		/**
		 * @brief Send a Float value to the shader
		 * @param name Uniform name
		 * @param value Value
		 */
		void SetFloat(std::string name, float value);
		
		/**
		 * @brief Send an Int value to the shader
		 * @param name Uniform name
		 * @param value Value
		 */
		void SetInt(std::string name, int value);
		
		/**
		 * @brief Send a Matrix4 value to the shader
		 * @param name Uniform name
		 * @param value Value
		 */
		void SetMatrix4(std::string name, Matrix4 &value);
		
		/**
		 * @brief Send a Vector4 value to the shader
		 * @param name Uniform name
		 * @param value Value
		 */
		void SetVector4(std::string name, Vector4 value);
		
		/**
		 * @brief Send a Vector3 value to the shader
		 * @param name Uniform name
		 * @param value Value
		 */
		void SetVector3(std::string name, Vector3 value);
		
		/**
		 * @brief Send a Vector2 value to the shader
		 * @param name Uniform name
		 * @param value Value
		 */
		void SetVector2(std::string name, Vector2 value);

		/**
		 * @brief
		 * @return True if the shader is valid for use, False otherwise.
		 */
		bool IsValid() const {return m_valid;}

		static const int AE_SHADER_TYPE_FRAGMENT = 0x0001;
		static const int AE_SHADER_TYPE_VERTEX = 0x0002;
		
		unsigned int GetProgram() const { return m_program; }
		
	private:
		bool m_valid;
		int vert_count, frag_count;
		unsigned int m_vert, m_frag, m_program;

		GLuint SIG_CreateShader(std::string src, int type);
		GLuint SIG_CreateProgram(GLuint vs, GLuint fs);
		void SIG_ExtractUniforms(std::string src);
		
		int SIG_FindUniform(std::string name);

		std::map<std::string, GLint> m_uniforms;
	};

}
#endif
