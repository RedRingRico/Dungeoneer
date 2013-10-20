#include <GameEntity.hpp>
#include <cstring>

namespace Dungeoneer
{
	void GameEntity::ID( const ZED_UINT32 p_ID )
	{
		m_ID = p_ID;
	}

	ZED_UINT32 GameEntity::ID( ) const
	{
		return m_ID;
	}

	void GameEntity::Name( const char *p_pName )
	{
		strcpy( m_pName, p_pName );
	}

	char *GameEntity::Name( ) const
	{
		return m_pName;
	}
}

