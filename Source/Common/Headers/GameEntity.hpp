#ifndef __DUNGEONEER_GAMEENTITY_HPP__
#define __DUNGEONEER_GAMEENTITY_HPP__

#include <System/DataTypes.hpp>
#include <Arithmetic/Vector3.hpp>
#include <Arithmetic/AABB.hpp>
#include <Renderer/Model.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Shader.hpp>

namespace Dungeoneer
{
	class GameEntity
	{
	public:
		virtual ~GameEntity( ) { }

		void ID( const ZED_UINT32 p_ID );
		ZED_UINT32 ID( ) const;

		void Name( const ZED_CHAR8 *p_pName );
		ZED_CHAR8 *Name( ) const;

		void Position( const ZED::Arithmetic::Vector3 &p_Position );
		ZED::Arithmetic::Vector3 Position( ) const;

		void Orientation( const ZED::Arithmetic::Vector3 &p_Orientation );
		ZED::Arithmetic::Vector3 Orientation( ) const;

		virtual void Update( const ZED_UINT64 p_MicroSeconds ) = 0;

	protected:
		ZED_UINT32					m_ID;
		ZED_CHAR8					*m_pName;
		ZED::Arithmetic::Vector3	m_Position;
		ZED::Arithmetic::Vector3	m_Orientation;
		ZED::Arithmetic::AABB		m_BoundingBox;
	};
}

#endif // __DUNGEONEER_GAMEENTITY_HPP__

