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
// SamplerParameterDX11
//
//--------------------------------------------------------------------------------
#include "RenderParameterDX11.h"
//--------------------------------------------------------------------------------
#ifndef SamplerParameterDX11_h
#define SamplerParameterDX11_h
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class SamplerParameterDX11 : public RenderParameterDX11
	{
	public:
		SamplerParameterDX11();
		SamplerParameterDX11( SamplerParameterDX11& copy );
		virtual ~SamplerParameterDX11();

		virtual void SetParameterData( void* pData );
		virtual ParameterType GetParameterType();
		int GetIndex();

		void UpdateValue( RenderParameterDX11* pParameter );

	protected:
		int		m_iSampler;
	};
};
//--------------------------------------------------------------------------------
#endif // SamplerParameterDX11_h
//--------------------------------------------------------------------------------
