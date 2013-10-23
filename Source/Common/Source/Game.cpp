#include <Game.hpp>
#include <cstring>
#include <System/Memory.hpp>
#include <unistd.h>
#include <GitVersion.hpp>
#include <Utility/Grid.hpp>
#include <Renderer/Camera.hpp>

const ZED_FLOAT32 STRAFE_SPEED = 0.2f;
const ZED_FLOAT32 THRUST_SPEED = 0.2f;

namespace Dungeoneer
{
	Game::Game( )
	{
		m_pWindow = ZED_NULL;
		m_pRenderer = ZED_NULL;
		m_pInputManager = ZED_NULL;

		memset( &m_Canvas, 0, sizeof( m_Canvas ) );
		m_Running = ZED_FALSE;
	}

	Game::~Game( )
	{
		zedSafeDelete( m_pInputManager );
		zedSafeDelete( m_pRenderer );
		zedSafeDelete( m_pWindow );
	}

	ZED_UINT32 Game::Execute( )
	{
		// NOT CROSS-PLATFORM!
		XEvent Event;
		ZED::System::WINDOWDATA	WindowData = m_pWindow->WindowData( );
		// !NOT CROSS-PLATFORM
		char *pTmpBuffer = new char[ 1024 ];
		memset( pTmpBuffer, '\0', sizeof( char )*1024 );
		strcat( pTmpBuffer, "Dungeoneer for " );
#if defined ZED_PLATFORM_LINUX
		strcat( pTmpBuffer, "Linux " );
#elif defined ZED_PLATFORM_WINDOWS
		strcat( pTmpBuffer, "Windows " );
#endif // ZED_PLATFORM_LINUX

#if defined ZED_ARCH_X86
#if defined ZED_BITSIZE_32
		strcat( pTmpBuffer, "x86" );
#elif defined ZED_BITSIZE_64
		strcat( pTmpBuffer, "x86_64" );
#endif // ZED_BITSIZE_32
#endif // ZED_ARCH_X86

#if defined ZED_BUILD_DEBUG
		strcat( pTmpBuffer, " [DEBUG] | Build " );
		strcat( pTmpBuffer, GIT_COMMITHASH );
#elif defined ZED_BUILD_PROFILE
		strcat( pTmpBuffer, " [PROFILE] | Build " );
		strcat( pTmpBuffer, GIT_COMMITHASH );
#elif defined ZED_BUILD_RELEASE
#else
		#error Incorrect build profile
#endif // ZED_BUILD_DEBUG

		m_pWindow->Title( pTmpBuffer );

		delete [ ] pTmpBuffer;
		pTmpBuffer = ZED_NULL;
		
		m_Running = ZED_TRUE;
		ZED::Utility::Grid TestGrid( m_pRenderer );
		ZED_COLOUR GridColour;
		GridColour.Red = 1.0f;
		GridColour.Green = 1.0f;
		GridColour.Blue = 0.0f;
		GridColour.Alpha = 1.0f;

		if( TestGrid.Initialise( 10, 10, ZED::Utility::PLANE_AXIS_XZ,
			GridColour, 0.0f, 0.1f ) != ZED_OK )
		{
			zedTrace( "Failed to create a grid\n" );
			return ZED_FAIL;
		}

		ZED::Renderer::Camera		MainCam;
		ZED::Arithmetic::Vector3	CamPosition( 0.0f, 10.0f, 12.0f ),
									CamLook( 0.0f, 0.0f, 0.0f ),
									WorldUp( 0.0f, 1.0f, 0.0f );
		ZED::Arithmetic::Matrix4x4	Projection, View, ProjView;


		MainCam.ClippingPlanes( 1.0f, 10000.0f );
		MainCam.PerspectiveProjection( 45.0f, 1280.0f / 720.0f, &Projection );
		ZED::Arithmetic::Vector3 Velocity( 0.0f, 0.0f, 0.0f );

		while( m_Running )
		{
			m_pWindow->Update( );
			m_pInputManager->Update( );

			// The reason for this loop is that the left over messages in the
			// queue need to be processed
			while( XPending( WindowData.pX11Display ) > 0 )
			{
				XNextEvent( WindowData.pX11Display, &Event );
			}

			Velocity.Set( 0.0f, 0.0f, 0.0f );

			if( m_Keyboard.IsKeyDown( 'a' ) )
			{
				Velocity[ 0 ] = -STRAFE_SPEED;
			}
			if( m_Keyboard.IsKeyDown( 'd' ) )
			{
				Velocity[ 0 ] = STRAFE_SPEED;
			}
			if( m_Keyboard.IsKeyDown( 'w' ) )
			{
				Velocity[ 2 ] = -THRUST_SPEED;
			}
			if( m_Keyboard.IsKeyDown( 's' ) )
			{
				Velocity[ 2 ] = THRUST_SPEED;
			}

			if( m_Keyboard.IsKeyDown( K_ESCAPE ) )
			{
				m_Running = ZED_FALSE;
			}

			CamPosition += Velocity;

			MainCam.ViewLookAt( CamPosition, CamLook, WorldUp );
			MainCam.View( &View );

			ProjView = Projection * View;
			
			this->Update( 16667ULL );
//			this->Render( );
		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
			TestGrid.Render( &ProjView );

		m_pRenderer->EndScene( );
		}

		return ZED_OK;
	}

	void Game::Update( const ZED_UINT64 p_MicroSeconds )
	{
	}

	void Game::Render( )
	{
		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
		m_pRenderer->EndScene( );
	}
}

