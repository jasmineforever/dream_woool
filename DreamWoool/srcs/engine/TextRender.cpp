#include "pch.h"
#include "TextRender.h"
namespace DW
{
	TextRender::TextRender(
		ID2D1Factory* d2d_factory, ID2D1RenderTarget* render_target,
		ID2D1Brush *text_brush, ID2D1SolidColorBrush *ptext_outline_brush,
		float stroke_width) :
		d2d_factory_(d2d_factory),
		render_target_(render_target),
		text_brush_(text_brush),
		text_outline_brush_(ptext_outline_brush),
		stroke_width_(stroke_width)
	{
		d2d_factory_->AddRef();
		render_target_->AddRef();
		text_brush_->AddRef();
		if(text_outline_brush_)
			text_outline_brush_->AddRef();
	}

	TextRender::~TextRender()
	{
		SafeRelease(&d2d_factory_);
		SafeRelease(&render_target_);
		SafeRelease(&text_brush_);
		SafeRelease(&text_outline_brush_);
	}
	IFACEMETHODIMP TextRender::DrawGlyphRun(
		void *clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode, DWRITE_GLYPH_RUN const *glyphRun,
		DWRITE_GLYPH_RUN_DESCRIPTION const *glyphRunDescription, IUnknown *clientDrawingEffect)
	{
		HRESULT hr = S_OK;

		ID2D1PathGeometry* pPathGeometry = nullptr;
		hr = d2d_factory_->CreatePathGeometry(&pPathGeometry);
		ID2D1GeometrySink* pSink = nullptr;
		hr = pPathGeometry->Open(&pSink);

		hr = glyphRun->fontFace->GetGlyphRunOutline(
			glyphRun->fontEmSize,
			glyphRun->glyphIndices,
			glyphRun->glyphAdvances,
			glyphRun->glyphOffsets,
			glyphRun->glyphCount,
			glyphRun->isSideways,
			glyphRun->bidiLevel,
			pSink
		);
		hr = pSink->Close();

		// Initialize a matrix to translate the origin of the glyph run.
		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);
		ID2D1TransformedGeometry *pTransformedGeometry = nullptr;
		hr = d2d_factory_->CreateTransformedGeometry(pPathGeometry, &matrix, &pTransformedGeometry);

		// 绘制文字描边部分
		if(text_outline_brush_)
			render_target_->DrawGeometry(pTransformedGeometry, text_outline_brush_, stroke_width_);
		// 绘制文字填充部分
		render_target_->FillGeometry(pTransformedGeometry, text_brush_);

		SafeRelease(&pPathGeometry);
		SafeRelease(&pSink);
		SafeRelease(&pTransformedGeometry);

		return hr;
	}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawUnderline                              *
*                                                                 *
*  Draws underlines below the text using a Direct2D recatangle    *
*  geometry                                                       *
*                                                                 *
******************************************************************/

	IFACEMETHODIMP TextRender::DrawUnderline(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
	)
	{
		HRESULT hr;

		D2D1_RECT_F rect = D2D1::RectF(
			0,
			underline->offset,
			underline->width,
			underline->offset + underline->thickness
		);

		ID2D1RectangleGeometry* pRectangleGeometry = NULL;
		hr = d2d_factory_->CreateRectangleGeometry(
			&rect,
			&pRectangleGeometry
		);

		// Initialize a matrix to translate the origin of the underline
		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY + 2 //underline position
		);

		ID2D1TransformedGeometry* pTransformedGeometry = NULL;
		if (SUCCEEDED(hr))
		{
			hr = d2d_factory_->CreateTransformedGeometry(
				pRectangleGeometry,
				&matrix,
				&pTransformedGeometry
			);
		}

		// Draw the outline of the rectangle

		if (text_outline_brush_)
		{
			render_target_->DrawGeometry(
				pTransformedGeometry,
				text_outline_brush_
			);
		}

		// Fill in the rectangle
		render_target_->FillGeometry(
			pTransformedGeometry,
			text_brush_
		);

		SafeRelease(&pRectangleGeometry);
		SafeRelease(&pTransformedGeometry);

		return S_OK;
	}

	/******************************************************************
	*                                                                 *
	*  CustomTextRenderer::DrawStrikethrough                          *
	*                                                                 *
	*  Draws strikethroughs below the text using a Direct2D           *
	*  recatangle geometry                                            *
	*                                                                 *
	******************************************************************/

	IFACEMETHODIMP TextRender::DrawStrikethrough(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
	)
	{
		HRESULT hr;

		D2D1_RECT_F rect = D2D1::RectF(
			0,
			strikethrough->offset,
			strikethrough->width,
			strikethrough->offset + strikethrough->thickness
		);

		ID2D1RectangleGeometry* pRectangleGeometry = NULL;
		hr = d2d_factory_->CreateRectangleGeometry(
			&rect,
			&pRectangleGeometry
		);

		// Initialize a matrix to translate the origin of the strikethrough
		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		ID2D1TransformedGeometry* pTransformedGeometry = NULL;
		if (SUCCEEDED(hr))
		{
			hr = d2d_factory_->CreateTransformedGeometry(
				pRectangleGeometry,
				&matrix,
				&pTransformedGeometry
			);
		}

		// Draw the outline of the rectangle
		if (text_outline_brush_)
		{
			render_target_->DrawGeometry(
				pTransformedGeometry,
				text_outline_brush_
			);
		}
		// Fill in the rectangle
		render_target_->FillGeometry(
			pTransformedGeometry,
			text_brush_
		);

		SafeRelease(&pRectangleGeometry);
		SafeRelease(&pTransformedGeometry);

		return S_OK;
	}
}