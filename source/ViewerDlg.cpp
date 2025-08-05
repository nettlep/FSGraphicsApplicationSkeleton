// ---------------------------------------------------------------------------------------------------------------------------------
//  _     _ _                         _____  _                            
// | |   | (_)                       |  __ \| |                           
// | |   | |_  _____      __ ___ _ __| |  | | | __ _      ___ _ __  _ __  
//  \ \ / /| |/ _ \ \ /\ / // _ \ '__| |  | | |/ _` |    / __| '_ \| '_ \ 
//   \ V / | |  __/\ V  V /|  __/ |  | |__| | | (_| | _ | (__| |_) | |_) |
//    \_/  |_|\___| \_/\_/  \___|_|  |_____/|_|\__, |(_) \___| .__/| .__/ 
//                                              __/ |        | |   | |    
//                                             |___/         |_|   |_|    
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CViewerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewerDlg::IDD, pParent), render(NULL)
{
	//{{AFX_DATA_INIT(CViewerDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// ---------------------------------------------------------------------------------------------------------------------------------

	CViewerDlg::~CViewerDlg()
{
	delete render;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewerDlg)
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	CViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// Set the window size

	enum	{WIDTH  = 640};
	enum	{HEIGHT = 480};

	// Current window position & size

	CRect	clientRect;
	GetClientRect(clientRect);
	CRect	windowRect;
	GetWindowRect(windowRect);

	// New window position & size

	windowRect.right  += WIDTH  - (clientRect.right  - clientRect.left);
	windowRect.bottom += HEIGHT - (clientRect.bottom - clientRect.top); 
	MoveWindow(windowRect);

	// Init the Renderer

	render = new Render(*GetDC(), *this);

	// Setup a timer for rendering

	SetTimer(0, 0, NULL);

	// Done

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CViewerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (render) render->flip();
		CDialog::OnPaint();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

HCURSOR	CViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CViewerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (render) render->updateWindowPosition();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CViewerDlg::OnTimer(UINT nIDEvent) 
{
	// Draw a frame

	render->renderFrame();
	
	// Keeeep going

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ViewerDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
