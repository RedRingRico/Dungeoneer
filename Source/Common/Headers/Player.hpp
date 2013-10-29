#ifndef __DUNGEONEER_PLAYER_HPP__
#define __DUNGEONEER_PLAYER_HPP__

#include <RenderableGameEntity.hpp>

namespace Dungeoneer
{
	class Player : public RenderableGameEntity
	{
	public:
		Player( );
		virtual ~Player( );

		virtual ZED_UINT32 Initialise( );

		virtual void Update( const ZED_UINT64 p_MicroSeconds );
	private:
	};
}

#endif // __DUNGEONEER_PLAYER_HPP__

