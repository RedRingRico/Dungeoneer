#ifndef __DUNGEONEER_RENDERABLEGAMEENTITY_HPP__
#define __DUNGEONEER_RENDERABLEGAMEENTITY_HPP__

#include <GameEntity.hpp>
#include <Arithmetic/Matrix4x4.hpp>
#include <Renderer/Model.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Shader.hpp>

namespace Dungeoneer
{
	class RenderableGameEntity : public GameEntity
	{
	public:
		RenderableGameEntity( );
		virtual ~RenderableGameEntity( );

		ZED_UINT32 Renderer( const ZED::Renderer::Renderer *p_pRenderer );
		
		ZED_UINT32 LoadModel( const ZED_CHAR8 *p_pFile,
			const ZED_BOOL p_Relative = ZED_TRUE );
		ZED_UINT32 LoadShader( const ZED_CHAR8 *p_pFile,
			const ZED_SHADER_TYPE p_Type,
			const ZED_BOOL p_Relative = ZED_TRUE );

		virtual void Render(
			const ZED::Arithmetic::Matrix4x4 &p_ProjectionView ) const;

	protected:
		ZED::Renderer::Renderer		*m_pRenderer;
		ZED::Renderer::Shader		*m_pShader;
		ZED::Renderer::Model		*m_pModel;
	};
}

#endif // __DUNGEONEER_RENDERABLEGAMEENTITY_HPP__

