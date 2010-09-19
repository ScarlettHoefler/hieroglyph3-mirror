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
// RendererDX11
//
//--------------------------------------------------------------------------------
#ifndef RendererDX11_h
#define RendererDX11_h
//--------------------------------------------------------------------------------
#include "IRenderer.h"
#include "TArray.h"
#include <boost/smart_ptr/shared_ptr.hpp>

#include <initguid.h>
#include <string>
#include <map>

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3dx10.lib" )
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "d3d9.lib" )

#include <dxgi.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include <d3dx11.h>
#include <D3DX10core.h>
#include <D3D11Shader.h>
#include <d3d9.h>

#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4f.h"

#include "ViewRenderParams.h"
#include "VertexStageDX11.h"
#include "HullStageDX11.h"
#include "DomainStageDX11.h"
#include "GeometryStageDX11.h"
#include "PixelStageDX11.h"
#include "ComputeStageDX11.h"

#include "OutputMergerStageDX11.h"

#include "ResourceProxyDX11.h"

#define SAFE_RELEASE( x ) {if(x){(x)->Release();(x)=NULL;}}
#define SAFE_DELETE( x ) {if(x){delete (x);(x)=NULL;}}

//--------------------------------------------------------------------------------

namespace Glyph3
{
	class VertexBufferDX11;
	class IndexBufferDX11;
	class ConstantBufferDX11;
	class StructuredBufferDX11;
	class ByteAddressBufferDX11;
	class Texture1dDX11;
	class Texture2dDX11;
	class Texture3dDX11;
	class ResourceDX11;
	class SwapChainDX11;
	class ViewPortDX11;

	class BufferConfigDX11;
	class Texture1dConfigDX11;
	class Texture2dConfigDX11;
	class Texture3dConfigDX11;
	class SwapChainConfigDX11;

	class CommandListDX11;
	class GeometryDX11;

	class ShaderResourceViewDX11;
	class RenderTargetViewDX11;
	class DepthStencilViewDX11;
	class UnorderedAccessViewDX11;

	class ShaderDX11;
	class InputLayoutDX11;
	class BlendStateDX11;
	class DepthStencilStateDX11;
	class RasterizerStateDX11;
	class SamplerStateDX11;

	class BlendStateConfigDX11;
	class DepthStencilStateConfigDX11;
	class RasterizerStateConfigDX11;

	class VertexShaderDX11;
	class HullShaderDX11;
	class DomainShaderDX11;
	class GeometryShaderDX11;
	class PixelShaderDX11;
	class ComputeShaderDX11;
	class RenderEffectDX11;

	class RenderParameterDX11;

	class ParameterManagerDX11;
	class PipelineManagerDX11;

	class IRenderView;

	enum ResourceType
	{
		RT_VERTEXBUFFER = 0x010000,
		RT_INDEXBUFFER = 0x020000,
		RT_CONSTANTBUFFER = 0x030000,
		RT_STRUCTUREDBUFFER = 0x040000,
		RT_BYTEADDRESSBUFFER = 0x050000,
		RT_TEXTURE1D = 0x060000,
		RT_TEXTURE2D = 0x070000,
		RT_TEXTURE3D = 0x080000
	};
	struct ThreadPayLoad
	{
		int id;
		bool bComplete;
		PipelineManagerDX11* pPipeline;
		ParameterManagerDX11* pParamManager;
		IRenderView* pRenderView;
		CommandListDX11* pList;
	};




	class RendererDX11 : public IRenderer
	{
	public:
		RendererDX11();
		virtual ~RendererDX11();

		// Access to the renderer.  There should only be a single instance
		// of the renderer at any given time.

		static RendererDX11* Get();

		// Provide the feature level of the current machine.  This can be
		// called before or after the device has been created.

		D3D_FEATURE_LEVEL GetAvailableFeatureLevel( D3D_DRIVER_TYPE DriverType );
		D3D_FEATURE_LEVEL GetCurrentFeatureLevel();

		// Renderer initialization and shutdown methods.  These methods
		// obtain and release all of the hardware specific resources that
		// are used during rendering.

		virtual bool Initialize( D3D_DRIVER_TYPE DriverType, D3D_FEATURE_LEVEL FeatureLevel );
		virtual void Shutdown();

		// These methods provide rendering frame control.  They are closely
		// related to the API for sequencing rendering batches.

		virtual void Present( HWND hWnd = 0, int SwapChain = -1 );

		// Allow the application to create swap chains

		int CreateSwapChain( SwapChainConfigDX11* pConfig );// ResourceManagerDX11

		// The create buffer method provides a way to create any of the buffer
		// structures - vertex, index, and constant buffers.  These all utilize the
		// same ID3D11Buffer interface, and only vary with a flag in the description.

		ResourcePtr CreateVertexBuffer( BufferConfigDX11* pConfig,  D3D11_SUBRESOURCE_DATA* pData );
		ResourcePtr CreateIndexBuffer( BufferConfigDX11* pConfig,  D3D11_SUBRESOURCE_DATA* pData );
		ResourcePtr CreateConstantBuffer( BufferConfigDX11* pConfig,  D3D11_SUBRESOURCE_DATA* pData );
		ResourcePtr CreateStructuredBuffer( BufferConfigDX11* pConfig,  D3D11_SUBRESOURCE_DATA* pData );
		ResourcePtr CreateByteAddressBuffer( BufferConfigDX11* pConfig,  D3D11_SUBRESOURCE_DATA* pData );

		ResourcePtr CreateTexture1D( Texture1dConfigDX11* pConfig, D3D11_SUBRESOURCE_DATA* pData );
		ResourcePtr CreateTexture2D( Texture2dConfigDX11* pConfig, D3D11_SUBRESOURCE_DATA* pData );
		ResourcePtr CreateTexture3D( Texture3dConfigDX11* pConfig, D3D11_SUBRESOURCE_DATA* pData );

		// The resources created in the above function calls can only be accessed by
		// the rendering pipeline when they are bound with resource views.  The following
		// functions provide the interface for creating the views, and returns an index
		// for the application to use during rendering setup.

		int CreateShaderResourceView( int ResourceID, D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc );
		int CreateRenderTargetView( int ResourceID, D3D11_RENDER_TARGET_VIEW_DESC* pDesc );
		int CreateDepthStencilView( int ResourceID, D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc );
		int CreateUnorderedAccessView( int ResourceID, D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc );

		// The input layout is required to exactly match the input signature of the first
		// programmable stage that will receive the input assembler output.  This method is
		// provided for creating an input layout and later indexing the value when setting
		// the appropriate vertex shader.

		int CreateInputLayout( TArray<D3D11_INPUT_ELEMENT_DESC>& elements, int ShaderID  );

		// The individual states available in the D3D11 pipeline can be created here.  They
		// are referred to by index after being created.

		int CreateBlendState( BlendStateConfigDX11* pConfig );
		int CreateDepthStencilState( DepthStencilStateConfigDX11* pConfig );
		int CreateRasterizerState( RasterizerStateConfigDX11* pConfig );
		int CreateSamplerState( D3D11_SAMPLER_DESC* pDesc );
		int CreateViewPort( D3D11_VIEWPORT viewport );


		// Each programmable shader stage can be loaded from file, and stored in a list for
		// later use.  Either an application can directly set these values or a render effect
		// can encapsulate the entire pipeline configuration.

		int LoadShader( ShaderType type, std::wstring& filename, std::wstring& function,
			std::wstring& model, bool enablelogging = true );

		
		ResourcePtr GetSwapChainResource( int ID );
		// This is an interim solution to get the resolution of the current
		// adapter output resolution.

		Vector2f GetDesktopResolution();

		ResourcePtr LoadTexture( std::wstring filename );

		// These functions are used to convert the enumerated values to human readable text.
		// This is convenient for logging or displaying various states.

		static const char* TranslateShaderVariableClass( D3D10_SHADER_VARIABLE_CLASS varclass );
		static const char* TranslateShaderVariableType( D3D10_SHADER_VARIABLE_TYPE vartype );
		static const char* TranslateSystemVariableName( D3D10_NAME name );
		static const char* TranslateRegisterComponentType( D3D10_REGISTER_COMPONENT_TYPE type );
		static const char* TranslateCBufferType( D3D11_CBUFFER_TYPE type );
		static const char* TranslateShaderInputType( D3D10_SHADER_INPUT_TYPE type );
		static const char* TranslateResourceReturnType( D3D11_RESOURCE_RETURN_TYPE type );

		static std::wstring Print_D3D11_SHADER_DESC( D3D11_SHADER_DESC description );
		static std::wstring Print_D3D10_PRIMITIVE_TOPOLOGY( D3D10_PRIMITIVE_TOPOLOGY topology );
		static std::wstring Print_D3D10_PRIMITIVE( D3D10_PRIMITIVE primitive );
		static std::wstring Print_D3D11_PRIMITIVE( D3D11_PRIMITIVE primitive );
		static std::wstring Print_D3D11_TESSELLATOR_OUTPUT_PRIMITIVE( D3D11_TESSELLATOR_OUTPUT_PRIMITIVE primitive );
		static std::wstring Print_D3D11_TESSELLATOR_PARTITIONING( D3D11_TESSELLATOR_PARTITIONING partitioning );
		static std::wstring Print_D3D11_TESSELLATOR_DOMAIN( D3D11_TESSELLATOR_DOMAIN domain );

		// Provide access to the pipeline states.
		boost::shared_ptr<BlendStateDX11>				GetBlendState( int index );
		boost::shared_ptr<DepthStencilStateDX11>		GetDepthState( int index );
		boost::shared_ptr<RasterizerStateDX11>			GetRasterizerState( int index );
		ViewPortDX11*									GetViewPort( int index );
		ResourceDX11*									GetResource( int index );

		ShaderResourceViewDX11*							GetShaderResourceView( int index );
		RenderTargetViewDX11*							GetRenderTargetView( int index );
		DepthStencilViewDX11*							GetDepthStencilView( int index );
		UnorderedAccessViewDX11*						GetUnorderedAccessView( int index );

		InputLayoutDX11*								GetInputLayout( int index );
		SamplerStateDX11*								GetSamplerState( int index );

		ShaderDX11*										GetShader( int index );

		// Here is the caching API functions
		void QueueRenderView( IRenderView* pRenderView );
		void ProcessRenderViewQueue( );

		// PIX marker and event functions
		static void PIXBeginEvent( const wchar_t* name );
		static void PIXEndEvent();

		// Basic controls over the threading system
		void SetMultiThreadingState( bool enable );

	private:

		// The main API interfaces used in the renderer.
		ID3D11Device*				m_pDevice;
		ID3D11Debug*				m_pDebugger;
		D3D_DRIVER_TYPE				m_driverType;

		// Static renderer access - used for accessing the renderer when no reference
		// is already available.

		static RendererDX11*				m_spRenderer;

		// In general, all resources and API objects are housed in expandable arrays
		// wrapper objects.  The position within the array is used to provide fast
		// random access to the renderer clients.

		TArray<SwapChainDX11*>				m_vSwapChains;

		// Resource allocation containers are stored in an expandable array, which
		// provides fast random access with indices.

		TArray<ResourceDX11*>				m_vResources;

		// Resource view containers.  These are indexed by the application for
		// the various pipeline binding operations.

		TArray<ShaderResourceViewDX11*>		m_vShaderResourceViews;
		TArray<RenderTargetViewDX11*>		m_vRenderTargetViews;
		TArray<DepthStencilViewDX11*>		m_vDepthStencilViews;
		TArray<UnorderedAccessViewDX11*>	m_vUnorderedAccessViews;

		// The shader programs are stored in an expandable array of their base classes.

		TArray<ShaderDX11*>					m_vShaders;
		
		// These states are stored as shared pointers to the object.  This is the direction
		// that the renderer is heading in - eventually references to the objects will be
		// returned instead of indices.  This will eliminate issues with creating and
		// destroying many resources, and allow the renderer clients to have greater access
		// the objects without querying the renderer.

		TArray<boost::shared_ptr<BlendStateDX11>>				m_vBlendStates;
		TArray<boost::shared_ptr<DepthStencilStateDX11>>		m_vDepthStencilStates;
		TArray<boost::shared_ptr<RasterizerStateDX11>>			m_vRasterizerStates;

		TArray<InputLayoutDX11*>								m_vInputLayouts;
		TArray<SamplerStateDX11*>								m_vSamplerStates;
		TArray<ViewPortDX11*>									m_vViewPorts;

	public:
		ParameterManagerDX11*									m_pParamMgr;
		PipelineManagerDX11*									pImmPipeline;
		PipelineManagerDX11*									m_pDeferredPipeline;

	protected:

		bool													m_bMultiThreadActive;
		D3D_FEATURE_LEVEL										m_FeatureLevel;

		TArray<IRenderView*>									m_vQueuedViews;

		// Internal API for friends to use - only the rendering system should use these!
		VertexBufferDX11* GetVertexBuffer( int index );
		IndexBufferDX11* GetIndexBuffer( int index );

		friend GeometryDX11;
	};
};

unsigned int WINAPI _RenderViewThreadProc( void* lpParameter );

// Multithreading support objects
#define NUM_THREADS 4
extern HANDLE						g_aThreadHandles[NUM_THREADS];
extern Glyph3::ThreadPayLoad		g_aPayload[NUM_THREADS];
extern HANDLE						g_aBeginEventHandle[NUM_THREADS];
extern HANDLE						g_aEndEventHandle[NUM_THREADS];

#endif // RendererDX11_h