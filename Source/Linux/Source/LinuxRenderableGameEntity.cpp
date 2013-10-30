#include <RenderableGameEntity.hpp>
#include <Renderer/LinuxRendererOGL3.hpp>
#include <Renderer/OGL/GLModel.hpp>
#include <System/Debugger.hpp>
#include <Renderer/OGL/GLShader.hpp>
#include <System/File.hpp>

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

		ZED_CHAR8 *pFile = new ZED_CHAR8[ ZED_MAX_PATH ];
		if( p_Relative )
		{
			ZED::System::GetExecutablePath( &pFile, ZED_MAX_PATH );
			strcat( pFile, p_pFile );
		}
		else
		{
			strcat( pFile, p_pFile );
		}

		zedTrace( "[Dungoneer::RenderableGameEntity::LoadModel] <INFO> "
			"Loading model: \"%s\"\n", pFile );

		switch( m_pRenderer->BackEnd( ) )
		{
			case ZED_RENDERER_BACKEND_OPENGL:
			{
				m_pModel = new ZED::Renderer::GLModel( m_pRenderer );
				break;
			}
			default:
			{
				zedTrace( "[Dungeoneer::RenderableGameEntity::LoadModel] "
					"<ERROR> Unknown renderer back end\n" );

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

		if( m_pModel->Load( pFile ) != ZED_OK )
		{
			zedTrace( "[Dungeoneer::RenderableGameEntity::LoadModel] <ERROR> "
				"Failed to load model\n" );

			delete [ ] pFile;
			pFile = ZED_NULL;

			return ZED_FALSE;
		}

		delete [ ] pFile;
		pFile = ZED_NULL;

		return ZED_OK;
	}

	ZED_UINT32 RenderableGameEntity::LoadShader( const ZED_CHAR8 *p_pFile,
		const ZED_SHADER_TYPE p_Type, const ZED_BOOL p_Relative )
	{
		if( m_pRenderer == ZED_NULL )
		{
			zedTrace( "[Dungeoneer::RenderableGameEntity::LoadShader] <ERROR> "
				"Renderer not set\n" );

			return ZED_FAIL;
		}

		if( ( ZED_MAX_PATH - strlen( p_pFile ) ) < 0 )
		{
			zedTrace( "[Dungeoneer::RenderableGameEntity::LoadShader] <ERROR> "
				"File path too long, exceeded by %d characters\n",
				-( ZED_MAX_PATH - strlen( p_pFile ) ) );

			return ZED_FAIL;
		}

		if( m_pShader == ZED_NULL )
		{
			switch( m_pRenderer->BackEnd( ) )
			{
				case ZED_RENDERER_BACKEND_OPENGL:
				{
					if( m_pRenderer->ShaderSupport( ) )
					{
						m_pShader = new ZED::Renderer::GLShader( );

						return ZED_OK;
					}
					else
					{
						return ZED_FAIL;
					}
				}
				default:
				{
					zedTrace( "[Dungeoneer::RenderableGameEntity::LoadModel] "
						"<ERROR> Unknown renderer back end\n" );

					return ZED_FAIL;
				}
			}
		}
		ZED_CHAR8 *pFile = new ZED_CHAR8[ ZED_MAX_PATH ];

		if( p_Relative )
		{
			ZED::System::GetExecutablePath( &pFile, ZED_MAX_PATH );
			strcat( pFile, p_pFile );
		}
		else
		{
			strcat( pFile, p_pFile );
		}

		zedTrace( "[Dungeoneer::RenderableGameEntity::LoadShader] <INFO> "
			"Compliing shader: \"%s\"\n", p_pFile );

		if( m_pShader->Compile( const_cast< const ZED_CHAR8 ** >( &pFile ),
			p_Type, ZED_TRUE ) != ZED_OK )
		{
			delete [ ] pFile;
			pFile = ZED_NULL;

			return ZED_FAIL;
		}

		delete [ ] pFile;
		pFile = ZED_NULL;

		return ZED_OK;
	}
}

