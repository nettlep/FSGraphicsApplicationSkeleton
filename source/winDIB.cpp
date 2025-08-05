// ---------------------------------------------------------------------------------------------------------------------------------
//           _       _____  _____ ____                       
//          (_)     |  __ \|_   _|  _ \                      
// __      ___ _ __ | |  | | | | | |_) |     ___ _ __  _ __  
// \ \ /\ / / | '_ \| |  | | | | |  _ <     / __| '_ \| '_ \ 
//  \ V  V /| | | | | |__| |_| |_| |_) | _ | (__| |_) | |_) |
//   \_/\_/ |_|_| |_|_____/|_____|____/ (_) \___| .__/| .__/ 
//                                              | |   | |    
//                                              |_|   |_|    
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

// ---------------------------------------------------------------------------------------------------------------------------------

		winDIB::winDIB(CDC &dc)
		:_dc(dc), _frameBuffer(NULL)
{
	_bmi[0].bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bmi[0].bmiHeader.biPlanes = 1;
	_bmi[0].bmiHeader.biBitCount = 0;
	_bmi[0].bmiHeader.biXPelsPerMeter = 72;
	_bmi[0].bmiHeader.biYPelsPerMeter = 72;
	_bmi[0].bmiHeader.biClrUsed = 0;
	_bmi[0].bmiHeader.biClrImportant = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void		winDIB::depth(const WORD bitdepth)
{
	unsigned int	bytesPerPixel;
	DWORD		*colors = (DWORD *) _bmi[0].bmiColors;

	switch(bitdepth)
	{
		case 8:
			bytesPerPixel = 1;
			break;

		case 15:
			_bmi[0].bmiHeader.biCompression = BI_BITFIELDS;
			_bmi[0].bmiHeader.biBitCount = 16;
			colors[0] = 0x7C00;
			colors[1] = 0x03E0;
			colors[2] = 0x001F;
			bytesPerPixel = 2;
			break;

		case 16:
			_bmi[0].bmiHeader.biCompression = BI_BITFIELDS;
			_bmi[0].bmiHeader.biBitCount = 16;
			colors[0] = 0xF800;
			colors[1] = 0x07E0;
			colors[2] = 0x001F;
			bytesPerPixel = 2;
			break;

		case 24:
			_bmi[0].bmiHeader.biCompression = BI_RGB;
			_bmi[0].bmiHeader.biBitCount = 24;
			bytesPerPixel = 3;
			break;

		case 32:
			_bmi[0].bmiHeader.biCompression = BI_RGB;
			_bmi[0].bmiHeader.biBitCount = 32;
			bytesPerPixel = 4;
			break;

		default:
			bytesPerPixel = 0;
			_bmi[0].bmiHeader.biBitCount = 0;
			break;
	}

	_bmi[0].bmiHeader.biSizeImage = width() * height() * bytesPerPixel;
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned char	*winDIB::frameBuffer(unsigned char *fb, const LONG width, const LONG height, const WORD bitdepth)
{
	_bmi[0].bmiHeader.biWidth = width;
	_bmi[0].bmiHeader.biHeight = -height;
	_frameBuffer = fb;
	if (bitdepth)	depth(bitdepth);
	return _frameBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	winDIB::stretchToDisplay()
{
	int	srcX = _srcRect.left;
	int	srcY = _srcRect.top;
	int	srcW = _srcRect.Width();
	int	srcH = _srcRect.Height();
	int	dstX = _dstRect.left;
	int	dstY = _dstRect.top;
	int	dstW = _dstRect.Width();
	int	dstH = _dstRect.Height();
	return StretchDIBits(dc().GetSafeHdc(), dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, _frameBuffer, _bmi, DIB_RGB_COLORS, SRCCOPY);
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	winDIB::copyToDisplay()
{
	int	srcX = _srcRect.left;
	int	srcY = _srcRect.top;
	int	dstX = _dstRect.left;
	int	dstY = _dstRect.top;
	int	dstW = _dstRect.Width();
	int	dstH = _dstRect.Height();
	return SetDIBitsToDevice(dc().GetSafeHdc(), dstX, dstY, dstW, dstH, srcX, srcY, 0, dstH, _frameBuffer, _bmi, DIB_RGB_COLORS);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// winDIB.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
