#include <RenderableGameEntity.hpp>
#include <System/Memory.hpp>

namespace Dungeoneer
{
	RenderableGameEntity::RenderableGameEntity( )
	{
		m_pRenderer = ZED_NULL;
		m_pShader = ZED_NULL;
		m_pModel = ZED_NULL;
	}

	RenderableGameEntity::~RenderableGameEntity( )
	{
		zedSafeDelete( m_pShader );
		zedSafeDelete( m_pModel );
	}

	ZED_UINT32 RenderableGameEntity::Renderer(
		const ZED::Renderer::Renderer *p_pRenderer )
	{
		if( p_pRenderer )
		{
			m_pRenderer =
				const_cast< ZED::Renderer::Renderer * >( p_pRenderer );

			return ZED_OK;
		}

		return ZED_FAIL;
	}

	void RenderableGameEntity::Render( ) const
	{
		if( m_pRenderer->ShaderSupport( ) )
		{
			m_pShader->Activate( );
		}

		m_pModel->Render( );
	}
}

