#pragma once

namespace DW
{
	class TextRender : public IDWriteTextRenderer
	{
	private:
		ULONG ref_count_;
		ID2D1Factory* d2d_factory_;
		ID2D1RenderTarget *render_target_;
		ID2D1Brush *text_brush_;
		ID2D1SolidColorBrush *text_outline_brush_;
		float stroke_width_;

	public:
		TextRender(
			ID2D1Factory* d2d_factory, ID2D1RenderTarget* render_target,
			ID2D1Brush *text_brush, ID2D1SolidColorBrush *ptext_outline_brush,
			float stroke_width = 2.0f);

		~TextRender();

		IFACEMETHOD(DrawGlyphRun)(
			void                               *clientDrawingContext,
			FLOAT                              baselineOriginX,
			FLOAT                              baselineOriginY,
			DWRITE_MEASURING_MODE              measuringMode,
			DWRITE_GLYPH_RUN const             *glyphRun,
			DWRITE_GLYPH_RUN_DESCRIPTION const *glyphRunDescription,
			IUnknown                           *clientDrawingEffect
			);

		IFACEMETHOD(DrawUnderline)(
			void                   *clientDrawingContext,
			FLOAT                  baselineOriginX,
			FLOAT                  baselineOriginY,
			DWRITE_UNDERLINE const *underline,
			IUnknown               *clientDrawingEffect
			);

		IFACEMETHOD(DrawStrikethrough)(
			void                       *clientDrawingContext,
			FLOAT                      baselineOriginX,
			FLOAT                      baselineOriginY,
			DWRITE_STRIKETHROUGH const *strikethrough,
			IUnknown                   *clientDrawingEffect
			);

		IFACEMETHOD(DrawInlineObject)(
			void                *clientDrawingContext,
			FLOAT               originX,
			FLOAT               originY,
			IDWriteInlineObject *inlineObject,
			BOOL                isSideways,
			BOOL                isRightToLeft,
			IUnknown            *clientDrawingEffect
			)
		{
			return E_NOTIMPL;
		}

		IFACEMETHOD(IsPixelSnappingDisabled)(
			void* clientDrawingContext,
			BOOL* isDisabled
			)
		{
			*isDisabled = FALSE;
			return S_OK;
		}

		IFACEMETHOD(GetCurrentTransform)(
			void* clientDrawingContext,
			DWRITE_MATRIX* transform
			)
		{
			render_target_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
			return S_OK;
		}

		IFACEMETHOD(GetPixelsPerDip)(
			void* clientDrawingContext,
			FLOAT* pixelsPerDip
			)
		{
			float x, yUnused;

			render_target_->GetDpi(&x, &yUnused);
			*pixelsPerDip = x / 96;
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
		{
			if (iid == IID_IUnknown /*|| iid == IID_IDWritePixelSnapping || iid == IID_IDWriteTextRenderer*/)
			{
				*ppvObj = this;
				AddRef();
				return NOERROR;
			}
			return E_NOINTERFACE;
		}

		ULONG STDMETHODCALLTYPE AddRef()
		{
			return ++ref_count_;
		}

		ULONG STDMETHODCALLTYPE Release()
		{
			if (0 == --ref_count_)
			{
				delete this;
			}
			return ref_count_;
		}
	};
}