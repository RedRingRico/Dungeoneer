#include <Player.hpp>

namespace Dungeoneer
{
	Player::Player( )
	{
	}

	Player::~Player( )
	{
	}

	ZED_UINT32 Player::Initialise( )
	{
		this->LoadModel( "Player.zed", ZED_TRUE );
		this->LoadShader( "Player.vsh", ZED_VERTEX_SHADER, ZED_TRUE );
		this->LoadShader( "Player.fsh", ZED_FRAGMENT_SHADER, ZED_TRUE );

		ZED_SHADER_CONSTANT_MAP Constants[ 2 ];
		zedSetConstant( Constants, 0, ZED_MAT4X4, "u_ProjectionViewMatrix" );
		zedSetConstant( Constants, 1, ZED_FLOAT4, "u_Colour" );

		m_pShader->Activate( );
		m_pShader->SetConstantTypes( Constants, 2 );

		ZED_FLOAT32 Green[ 4 ] = { 0.0f, 1.0f, 0.0f, 1.0f };
		m_pShader->SetConstantData( 1, &Green );

		return ZED_OK;
	}

	void Player::Update( const ZED_UINT64 p_MicroSeconds )
	{
	}
}

