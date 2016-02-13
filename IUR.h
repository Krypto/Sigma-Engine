#ifndef SIGMA_IUR
#define SIGMA_IUR

namespace sig
{
	class IUR
	{
	public:
		virtual void Initialize() 		{}
		virtual void Update(float dt) 	{}
		virtual void Render() 			{}
	};
}

#endif // SIGMA_IUR
