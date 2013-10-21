#ifndef __DUNGEONEER_GAMEENTITY_HPP__
#define __DUNGEONEER_GAMEENTITY_HPP__

#include <System/DataTypes.hpp>
#include <Arithmetic/Vector3.hpp>
#include <Arithmetic/AABB.hpp>

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

		void Position( const ZED::Arithmetic::Vector3 &p_Position );
		ZED::Arithmetic::Vector3 Position( ) const;

		void Orientation( const ZED::Arithmetic::Vector3 &p_Orientation );
		ZED::Arithmetic::Vector3 Orientation( ) const;

	protected:
		ZED_UINT32					m_ID;
		char						*m_pName;
		ZED::Arithmetic::Vector3	m_Position;
		ZED::Arithmetic::Vector3	m_Orientation;
		ZED::Arithmetic::AABB		m_BoundingBox;
	};
}

#endif // __DUNGEONEER_GAMEENTITY_HPP__

