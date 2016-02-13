#ifndef SIGMA_RECT
#define SIGMA_RECT

namespace sig
{

	class Rect
	{
	public:
		Rect(float x=0, float y=0, float w=1, float h=1);

		/**
		 * @brief Checks if this rectangle intersects with another one
		 * @param b The second rectangle
		 * @return True if the rectangles intersect with each other
		 */
		bool Intersects(Rect b);
		
		/**
		 * @brief Checks if this rectangle has a point inside
		 * @param x X component of the point
		 * @param y Y component of the point
		 * @return True if the specified point is inside this rectangle
		 */
		bool HasPoint(float x, float y);
		
		/**
		 * @brief Generate an Inflated version of this Rect
		 * @param px Percent X
		 * @param py Percent Y
		 */
		Rect Inflated(float px, float py);
		
		float x, y, width, height;
	};

}

#endif // SIGMA_RECT
