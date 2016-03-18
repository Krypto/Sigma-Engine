#ifndef SIGMA_GUI_IMAGE
#define SIGMA_GUI_IMAGE

#include "Widget.h"
#include "Texture2D.h"

namespace sig
{

	class Image : public Widget
	{
	public:
		Image();

		Texture2D *GetImage() { return m_image; }
		void SetImage(Texture2D *img) { m_image = img; }

		void Render();

		WIDGET_TYPE(WType::IMAGE)
	private:
		Texture2D *m_image;
	};

}

#endif // SIGMA_GUI_IMAGE
