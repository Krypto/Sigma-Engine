#include "Texture2D.h"
#include "SIG_stbImage.h"

sig::Texture2D::Texture2D()
{
	m_bindCode = 0;
	m_width = 1;
	m_height = 1;
}

sig::Texture2D::Texture2D(int w, int h, int attachment)
{
	m_bindCode = 0;
	m_width = w;
	m_height = h;
	
	if (m_width < 0 || m_height < 0) {
		SIG_LOG_ERROR("Wrong texture size.");
		return;
	}
	Init(m_width, m_height, 0, GL_RGBA, GL_LINEAR, false, attachment);
}

sig::Texture2D::Texture2D(Byte* data, int w, int h)
{
	m_width = w;
	m_height = h;
	if (m_width < 0 || m_height < 0) {
		SIG_LOG_ERROR("Wrong texture size.");
		return;
	}
	int _w, _h, comp;
	Byte* pdata = stbi_load_from_memory(data, w*h, &_w, &_h, &comp, STBI_rgb_alpha);
	Init(w, h, pdata, comp == 3 ? GL_RGB : GL_RGBA, GL_LINEAR, false, NO_ATTACHMENT);
	stbi_image_free(pdata);
}

sig::Texture2D::Texture2D(const string& fileName)
{
	if (!fileName.empty()) {
		int w, h, comp;
		unsigned char *data = stbi_load(fileName.c_str(), &w, &h, &comp, STBI_rgb_alpha);
		
		if (data == nullptr) {
			SIG_LOG_ERROR("Could NOT load this image: " << fileName);
			return;
		}
		
		Init(w, h, data, comp == 3 ? GL_RGB : GL_RGBA, GL_NEAREST, false, NO_ATTACHMENT);
		
		stbi_image_free(data);
	}
}

sig::Texture2D::~Texture2D()
{
	if (m_bindCode) { glDeleteTextures(1, &m_bindCode); }
	
	if (GL_ARB_framebuffer_object) {
		if (m_fboBindCode) { glDeleteFramebuffers(1, &m_fboBindCode); }
	}
}

void sig::Texture2D::SetFilter(int filter)
{
	glBindTexture(GL_TEXTURE_2D, m_bindCode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void sig::Texture2D::Bind(int slot)
{
	if (slot > 31 || slot < 0) { return; }
	
	glActiveTextureARB(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_bindCode);
}

void sig::Texture2D::Unbind()
{
	if (GL_ARB_framebuffer_object) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void sig::Texture2D::Init(int w, int h, Byte* data, int format, int filter, bool clamp, int attach)
{
	if (w <= 0) { w = 1; }
	if (h <= 0) { h = 1; }
	
	glGenTextures(1, &m_bindCode);
	glBindTexture(GL_TEXTURE_2D, m_bindCode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	
	if (clamp) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	m_width = w;
	m_height = h;
	
	if (GL_ARB_framebuffer_object) {
		if (attach != NO_ATTACHMENT) {
			InitFBO(attach);
		}
	}
}

void sig::Texture2D::InitFBO(int attachment)
{
	glGenFramebuffers(1, &m_fboBindCode);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboBindCode);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_bindCode, 0);
	
	GLenum drawBuffers[1] = { (GLenum)attachment };
	glDrawBuffers(1, drawBuffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glDeleteFramebuffers(1, &m_fboBindCode);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void sig::Texture2D::BindForRendering()
{
	if (GL_ARB_framebuffer_object) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboBindCode);
		glViewport(0, 0, m_width, m_height);
	}
}

void sig::Texture2D::Draw(float x, float y, float w, float h)
{
	glUseProgram(0);
	Bind();
	glBegin(GL_TRIANGLE_STRIP);
	
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(1, 0); glVertex2f(x+w, y);
	glTexCoord2f(0, 1); glVertex2f(x, y+h);
	glTexCoord2f(1, 1); glVertex2f(x+w, y+h);
	
	glEnd();
	Unbind();
}
