//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) 2003-2010 Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "ViewGBuffer.h"
#include "Entity3D.h"
#include "Node3D.h"
#include "Texture2dConfigDX11.h"
#include "Log.h"
#include "ParameterManagerDX11.h"
#include "PipelineManagerDX11.h"
#include "Texture2dDX11.h"
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
ViewGBuffer::ViewGBuffer( RendererDX11& Renderer )
    : m_Renderer( Renderer )
{
	m_sParams.iViewType = VT_GBUFFER;

	ViewMatrix.MakeIdentity();
	ProjMatrix.MakeIdentity();					
}
//--------------------------------------------------------------------------------
ViewGBuffer::~ViewGBuffer()
{
}
//--------------------------------------------------------------------------------
void ViewGBuffer::Update( float fTime )
{
}
//--------------------------------------------------------------------------------
void ViewGBuffer::PreDraw( RendererDX11* pRenderer )
{
	if ( m_pEntity != NULL )
	{
		Matrix4f view = m_pEntity->GetView();
		SetViewMatrix( view );
	}

	// Queue this view into the renderer for processing.
	pRenderer->QueueRenderView( this );

	if ( m_pRoot )
	{
		// Run through the graph and pre-render the entities
		m_pRoot->PreRender( pRenderer, GetType() );
	}
}
//--------------------------------------------------------------------------------
void ViewGBuffer::Draw( PipelineManagerDX11* pPipelineManager, ParameterManagerDX11* pParamManager )
{
	if ( m_pRoot )
	{
		// Set the parameters for rendering this view
		pPipelineManager->ClearRenderTargets();
		for ( int i = 0; i < m_GBufferTargets.count(); ++i)
			pPipelineManager->BindRenderTargets( i, m_GBufferTargets[i] );
		pPipelineManager->BindDepthTarget( m_DepthTarget );
		pPipelineManager->ApplyRenderTargets();

		pPipelineManager->SetViewPort( m_iViewport );		

		// Clear the G-Buffer targets
		Vector4f color(0.0f, 0.0f, 0.0f, 0.0f);
		pPipelineManager->ClearBuffers( color, 1.0f, 0 );

		// Set this view's render parameters
		SetRenderParams( pParamManager );

		// Run through the graph and render each of the entities
		m_pRoot->Render( pPipelineManager, pParamManager, GetType() );
	}
}
//--------------------------------------------------------------------------------
void ViewGBuffer::SetRenderParams( ParameterManagerDX11* pParamManager )
{
	pParamManager->SetViewMatrixParameter( &ViewMatrix );
	pParamManager->SetProjMatrixParameter( &ProjMatrix );
}
//--------------------------------------------------------------------------------
void ViewGBuffer::SetUsageParams( ParameterManagerDX11* pParamManager )
{

}
//--------------------------------------------------------------------------------
void ViewGBuffer::SetTargets( TArray<ResourcePtr>& GBufferTargets, 
                                ResourcePtr DepthTarget, int Viewport )
{
    m_GBufferTargets = GBufferTargets;    

    m_iViewport = Viewport;
    m_DepthTarget = DepthTarget;
}