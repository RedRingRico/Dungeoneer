#ifndef __DUNGEONEER_GAMEENTITY_HPP__
#define __DUNGEONEER_GAMEENTITY_HPP__

#include <System/DataTypes.hpp>

namespace Dungeoneer
{
	class GameEntity
	{
	public:
		virtual ~GameEntity( );

		void ID( const ZED_UINT32 p_ID );
		ZED_UINT32 ID( ) const;

		void Name( const char *p_pName );
		char *Name( ) const;

	protected:
		ZED_UINT32	m_ID;
		char		*m_pName;
	};
}

#endif // __DUNGEONEER_GAMEENTITY_HPP__

