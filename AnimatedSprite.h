#ifndef SIGMA_DRAWABLE_ANIMATED_IMAGE
#define SIGMA_DRAWABLE_ANIMATED_IMAGE

#include "SIG_Utilities.h"
#include "Sprite.h"
#include <vector>
#include <initializer_list>

namespace sig
{
	
	typedef struct __sig__anim__ {
		vector<int> frames;
		string name;
		float speed;
		float time;
		bool loop;
		bool stopped;
		u32 currentIndex;
	} Animation;
	
	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite();
		AnimatedSprite(Texture2D *texture, int rows, int cols);
		
		/**
		 * @brief Set the current playing animation
		 * @param name The animation name
		 */
		void SetAnimation(string name);
		
		/**
		 * @brief Add a new animation
		 * @param name Animation name
		 * @param speed	Animation speed in seconds
		 * @param loop Animation loop
		 * @param frames Animation frame indices
		 */
		Animation* AddAnimation(string name, float speed, bool loop, std::initializer_list<int> frames);
		
		/**
		 * @brief Sets the frame of the current animation
		 * @param f Frame index
		 */
		void SetFrame(int f);
		int GetFrame() {
			if (m_current != nullptr) {
				return m_current->currentIndex;
			}
			return 0;
		}
		
		/**
		 * @brief Configure the sprite sheet
		 * @param rows How many frames vertically
		 * @param cols How many frames horizontally
		 */
		void Configure(int rows, int cols);
		
		int GetCols() const {return m_cols;}
		int GetRows() const {return m_rows;}
		
		void SetStatic(bool _static) {this->m_static = _static;}
		bool IsStatic() const {return m_static;}
		
		Animation* GetCurrent() {return m_current;}
		const vector<Animation*>& GetAnimations() const {return m_animations;}
				
		void Update(float dt);
		
		COMPONENT_NAME("AnimatedSprite")
	private:
		vector<Animation*> m_animations;
		Animation* m_current;
		vector<Rect> m_frames;
		
		int m_rows, m_cols;
		bool m_static;
		
		Animation* GetAnim(string name);
	};

}

#endif // SIGMA_DRAWABLE_ANIMATED_IMAGE
