#ifndef SIGMA_COLOR
#define SIGMA_COLOR

namespace sig
{

	class Color
	{
	public:
		/**
		 * @brief New Color from HEX (HTML) value
		 * @param hex The color in hexadecimal form
		 * @param alpha The opacity
		 */
		Color(unsigned int hex, float alpha=1);
		
		/**
		 * @brief New Color in RGBA format
		 * @param r Red value in percent
		 * @param g Green value in percent
		 * @param b Blue value in percent
		 * @param a Alpha value in percent
		 */
		Color(float r=0, float g=0, float b=0, float a=1);
		
		/**
		 * @brief Blend this Color with another Color
		 * @param b The other color
		 * @param factor How much to blend
		 * @return The modified color
		 */
		Color Blend(Color b, float factor);
		
		/**
		 * @brief Change the color brightness
		 * @param factor Brightness percent
		 * @return The modified color
		 */
		Color Brightness(float factor);
		
		/**
		 * @brief Color from other Color object plus Alpha
		 * @param color The other color
		 * @param a Alpha percent
		 * @return The Color with Alpha
		 */
		static Color FromColorAlpha(const Color& color, float a=1);
		
		float r, g, b, a;
		
		static Color BLACK;
		static Color WHITE;
		static Color DARK_GRAY;
		static Color GRAY;
		static Color RED;
		static Color GREEN;
		static Color BLUE;
		static Color DARK_RED;
		static Color DARK_GREEN;
		static Color DARK_BLUE;
		static Color YELLOW;
		static Color DARK_YELLOW;
		static Color PINK;
		static Color MAGENTA;
		static Color CYAN;
		static Color TRANSPARENT;
	};
	
}

#endif // SIGMA_COLOR
