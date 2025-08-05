// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                 _               _     
// |  __ \               | |             | |    
// | |__) | ___ _ __   __| | ___ _ __    | |__  
// |  _  / / _ \ '_ \ / _` |/ _ \ '__|   | '_ \ 
// | | \ \|  __/ | | | (_| |  __/ |    _ | | | |
// |_|  \_\\___|_| |_|\__,_|\___|_|   (_)|_| |_|
//                                              
//                                              
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

#ifndef	_H_RENDER
#define	_H_RENDER

// ---------------------------------------------------------------------------------------------------------------------------------

class	Render  
{
public:
	// Construction/Destruction

				Render(CDC &dc, CWnd &window);
virtual				~Render();

	// Accessors

inline		CWnd		&window() {return _window;}
inline		CDC		&dc() {return _dc;}
inline		winDIB		&dib() {return _dib;}

inline	const	unsigned int	&width() const {return _width;}
inline		unsigned int	&width() {return _width;}

inline	const	unsigned int	&height() const {return _height;}
inline		unsigned int	&height() {return _height;}

inline	const	unsigned int	*frameBuffer() const {return _frameBuffer;}
inline		unsigned int	*frameBuffer() {return _frameBuffer;}

	// Utilitarian

inline		void		flip() {dib().copyToDisplay();}
virtual		void		clear(unsigned int color = 0);
virtual		bool		updateWindowPosition();
virtual		bool		renderFrame();

private:
		CWnd		&_window;
		CDC		&_dc;
		winDIB		_dib;
		unsigned int	_width, _height;
		unsigned int	*_frameBuffer;

		sVERT		p0[4];
		sVERT		p1[4];			
		sVERT		p2[4];			
		sVERT		p3[4];			
		sVERT		*polys[4];
		int		polyCount;
		double		theta;
};

#endif
// ---------------------------------------------------------------------------------------------------------------------------------
// Render.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
