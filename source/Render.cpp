// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                 _                                
// |  __ \               | |                               
// | |__) | ___ _ __   __| | ___ _ __      ___ _ __  _ __  
// |  _  / / _ \ '_ \ / _` |/ _ \ '__|    / __| '_ \| '_ \ 
// | | \ \|  __/ | | | (_| |  __/ |    _ | (__| |_) | |_) |
// |_|  \_\\___|_| |_|\__,_|\___|_|   (_) \___| .__/| .__/ 
//                                            | |   | |    
//                                            |_|   |_|    
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/22/2000 by Paul Nettle
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include <math.h>

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

		Render::Render(CDC &dc, CWnd &window)
		:_window(window), _dc(dc), _dib(dc), _width(0), _height(0), _frameBuffer(NULL)
{
	// Setup the window stuff

	updateWindowPosition();

	// Init the texture mapper

	drawTexture();

	// Setup the 4 adjacent polygons

	p0[0].x = -1.0; p0[0].y = -1.0; p0[0].z =  1.0; p0[0].next = &p0[1];
	p0[1].x =    0; p0[1].y = -1.0; p0[1].z =  1.0; p0[1].next = &p0[2];
	p0[2].x =    0; p0[2].y =    0; p0[2].z =    0; p0[2].next = &p0[3];
	p0[3].x = -1.0; p0[3].y =    0; p0[3].z =    0; p0[3].next = NULL;

	p1[0].x =    0; p1[0].y = -1.0; p1[0].z =  1.0; p1[0].next = &p1[1];
	p1[1].x =  1.0; p1[1].y = -1.0; p1[1].z =  1.0; p1[1].next = &p1[2];
	p1[2].x =  1.0; p1[2].y =    0; p1[2].z =    0; p1[2].next = &p1[3];
	p1[3].x =    0; p1[3].y =    0; p1[3].z =    0; p1[3].next = NULL;

	p2[0].x = -1.0; p2[0].y =    0; p2[0].z =    0; p2[0].next = &p2[1];
	p2[1].x =    0; p2[1].y =    0; p2[1].z =    0; p2[1].next = &p2[2];
	p2[2].x =    0; p2[2].y =  1.0; p2[2].z = -1.0; p2[2].next = &p2[3];
	p2[3].x = -1.0; p2[3].y =  1.0; p2[3].z = -1.0; p2[3].next = NULL;

	p3[0].x =    0; p3[0].y =    0; p3[0].z =    0; p3[0].next = &p3[1];
	p3[1].x =  1.0; p3[1].y =    0; p3[1].z =    0; p3[1].next = &p3[2];
	p3[2].x =  1.0; p3[2].y =  1.0; p3[2].z = -1.0; p3[2].next = &p3[3];
	p3[3].x =    0; p3[3].y =  1.0; p3[3].z = -1.0; p3[3].next = NULL;

	polys[0] = p0;
	polys[1] = p1;
	polys[2] = p2;
	polys[3] = p3;
	polyCount = 4;
	theta = 0.0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

		Render::~Render()
{
	delete[] _frameBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void		Render::clear(unsigned int color)
{
	if (!color)
	{
		memset(_frameBuffer, 0, width() * height() *sizeof(unsigned int));
	}
	else
	{
		unsigned int	size = width() * height();
		unsigned int	*ptr = _frameBuffer;

		for (unsigned int i = 0; i < size; i++, ptr++)
		{
			*ptr = color;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool		Render::updateWindowPosition()
{
	// The new DIB dimensions

	{
		CRect	clientRect;
		window().GetClientRect(clientRect);
		unsigned int	w = clientRect.Width();
		unsigned int	h = clientRect.Height();

		// Bail if we don't have any dimension

		if (!w || !h) return true;

		// Bail if there's no change

		if (width() == w && height() == h && frameBuffer()) return true;

		// Update

		width() = w;
		height() = h;
	}

	// Reallocate our frame buffer

	delete[] _frameBuffer;
	_frameBuffer = new unsigned int[width()*height()];
	if (!_frameBuffer) return false;

	// Setup the dib

	dib().dstRect() = CRect(0, 0, width(), height());
	dib().srcRect() = CRect(0, 0, width(), height());
	dib().frameBuffer((unsigned char *) frameBuffer(), width(), height());
	dib().depth(32);
	
	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool		Render::renderFrame()
{
	// Clear the frame buffer

	clear();

	// Animate

	const	double	speed = 30.0;
	theta   += 0.0003 * speed;

	// Draw the polygons

	for (int i = 0; i < polyCount; i++)
	{
		// Temporary polygon

		sVERT	poly[4];

		// Offset/scale the vertices

		sVERT	*src = polys[i];
		sVERT	*dst = poly;

		while(src)
		{
			// Rotate

			dst->u = src->x * (0.49f * textureWidth)  + (0.5f * textureWidth);
			dst->v = src->y * (0.49f * textureHeight) + (0.5f * textureHeight);
			dst->w = 1.0f;
			dst->x = src->x * (float) cos(theta) - src->y * (float) sin(theta);
			dst->y = src->x * (float) sin(theta) + src->y * (float) cos(theta);
			dst->z = src->z;

			// Scale

			dst->x *= width() * 3;
			dst->y *= height() * 3;
			dst->z *= 10.0;
			dst->z += 20.0;

			// Project

			#ifndef USE_AFFINE
			dst->u /= dst->z;
			dst->v /= dst->z;
			dst->w /= dst->z;
			#endif
			dst->x /= dst->z;
			dst->y /= dst->z;

			// Offset to screen center

			dst->x += width()  / 2.0f + 0.5f;
			dst->y += height() / 2.0f + 0.5f;

			// Terminate the list

			dst->next = src->next ? &dst[1] : NULL;

			// Next!

			src = src->next;
			dst = dst->next;
		}

		// Do some drawing...

		#ifdef USE_AFFINE
		drawAffineTexturedPolygon(poly, frameBuffer(), width());
		#endif

		#ifdef USE_EXACT_PERSPECTIVE
		drawPerspectiveTexturedPolygon(poly, frameBuffer(), width());
		#endif

		#ifdef USE_SUB_AFFINE_PERSPECTIVE
		drawSubPerspectiveTexturedPolygon(poly, frameBuffer(), width());
		#endif
	}

	// Update the screen

	flip();

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Render.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
