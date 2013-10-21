#ifndef __DUNGEONEER_PLAYER_HPP__
#define __DUNGEONEER_PLAYER_HPP__

#include <GameEntity.hpp>

namespace Dungeoneer
{
	class Player : public GameEntity
	{
	public:
		ZED_EXPLICIT Player( const char *p_pModel );
		virtual ~Player( );

	private:
	};
}

#endif // __DUNGEONEER_PLAYER_HPP__

