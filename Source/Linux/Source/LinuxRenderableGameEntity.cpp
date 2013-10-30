#include <RenderableGameEntity.hpp>
#include <Renderer/LinuxRendererOGL3.hpp>
#include <Renderer/OGL/GLModel.hpp>
#include <System/Debugger.hpp>
#include <Renderer/OGL/GLShader.hpp>

namespace Dungeoneer
{
	ZED_UINT32 RenderableGameEntity::LoadModel( const ZED_CHAR8 *p_pFile,
		const ZED_BOOL p_Relative )
	{
		if( m_pRenderer == ZED_NULL )
		{
			zedTrace( "[Dungeoneer::RenderableGameEntity::LoadModel] <ERROR> "
				"Renderer has not been set\n" );

			return ZED_FAIL;
		}

		zedTrace( "[Dungoneer::RenderableGameEntity::LoadModel] <INFO> "
			"Loading model: \"%s\"\n", p_pFile );

		switch( m_pRenderer->BackEnd( ) )
		{
			case ZED_RENDERER_BACKEND_OPENGL:
			{
				m_pModel = new ZED::Renderer::GLModel( m_pRenderer );
				break;
			}
			default:
			{
				return ZED_FAIL;
			}
		}

		if( m_pModel == ZED_NULL )
		{
			zedTrace( "[Dungoneer::RenderableGameEntity::LoadModel] <ERROR> "
				"Could not instantiate a model class for model: %s\n",
				p_pFile );
			return ZED_FAIL;
		}

		zedTrace( "Loading model %s...\n", p_pFile );
		if( m_pModel->Load( p_pFile ) != ZED_OK )
		{
			zedTrace( "Failed to load model\n" );
			return ZED_FALSE;
		}

		return ZED_OK;
	}

	ZED_UINT32 RenderableGameEntity::LoadShader( const ZED_CHAR8 *p_pFile,
		const ZED_SHADER_TYPE p_Type, const ZED_BOOL p_Relative )
	{
		zedTrace( "Loading shader...\n" );

		if( m_pRenderer == ZED_NULL )
		{
			return ZED_FAIL;
		}

		if( m_pShader )
		{
			zedTrace( "Compiling shader...\n" );
			m_pShader->Compile( &p_pFile, p_Type, ZED_TRUE );

			return ZED_OK;
		}

		switch( m_pRenderer->BackEnd( ) )
		{
			case ZED_RENDERER_BACKEND_OPENGL:
			{
				if( m_pRenderer->ShaderSupport( ) )
				{
					m_pShader = new ZED::Renderer::GLShader( );
					zedTrace( "Compiling shader...\n" );
					m_pShader->Compile( &p_pFile, p_Type, ZED_TRUE );

					return ZED_OK;
				}
				else
				{
					return ZED_FAIL;
				}
			}
			default:
			{
				return ZED_FAIL;
			}
		}
		
		return ZED_OK;
	}
}

