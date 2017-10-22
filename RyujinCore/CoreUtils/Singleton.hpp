#ifndef SINGLETON_HPP
#define SINGLETON_HPP


namespace Ryujin
{
	template <typename T>
	class Singleton
	{
	public:
        virtual ~Singleton()
        {
            
        }
        
		static T* Instance()
		{
            static T instance;
            return &instance;
		}
	};
};


#endif