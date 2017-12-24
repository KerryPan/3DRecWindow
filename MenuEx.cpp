// MyMenuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MenuEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWnd*	CMenuEx::s_pMainMenu = NULL;

CMenuEx::CMenuEx()
{
	m_id = 0;
	m_pWndParent = NULL;
	m_pSubMenuShowing = NULL;
	m_nCurSel = -1;

	if (s_pMainMenu == NULL)
	{
		s_pMainMenu = this;
	}
}

CMenuEx::~CMenuEx()
{
	int nItemCnt = GetItemCount();

	for (int i = 0; i<nItemCnt; i++)
	{
		MENU_NODE node = m_arrMenuItems.GetAt(i);
		if (node.right != NULL)
		{
			delete node.right;
		}
	}
	if (this == s_pMainMenu)
	{
		s_pMainMenu = NULL;
	}
}

BEGIN_MESSAGE_MAP(CMenuEx, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_MENU_HIDE, __OnChildMenuHide)
END_MESSAGE_MAP()

void CMenuEx::OnPaint()
{
	CPaintDC dc(this);

	if (m_nCurSel >= 0)
	{
		CRect rctPaint = m_rctFirstItem;
		rctPaint.top += m_rctFirstItem.Height()*m_nCurSel;
		rctPaint.bottom += m_rctFirstItem.Height()*m_nCurSel;

		CDC dcMem;
		CBitmap bmpMem;
		dcMem.CreateCompatibleDC(&dc);
		bmpMem.CreateCompatibleBitmap(&dc, rctPaint.Width(), rctPaint.Height());
		dcMem.SelectObject(&bmpMem);
		dcMem.SelectObject(&m_font);

		// paint cursel
		dcMem.FillSolidRect(0, 0, rctPaint.Width(), rctPaint.Height(), m_rgbCursel);

		MENU_NODE node = m_arrMenuItems.GetAt(m_nCurSel);

		dcMem.SetBkMode(TRANSPARENT);
		COLORREF rgbText = node.item.bEnable ? m_rgbTextEnable : m_rgbTextDisable;
		if (m_bDstinvertText)
		{
			rgbText = ~rgbText & 0xFFFFFF;
		}
		dcMem.SetTextColor(rgbText);

		CRect rctText(m_nSpacingWidth, 0, rctPaint.Width(), rctPaint.Height());
		// output text
		dcMem.DrawText(node.item.strText, rctText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		if (node.right != NULL)
		{	// draw triangle
			CBrush brush;
			brush.CreateSolidBrush(rgbText);
			dcMem.SelectObject(&brush);

			CPoint p1(rctText.right, rctText.bottom - rctText.Height() / 2);
			CPoint p2(rctText.right - m_nSpacingWidth, p1.y - m_nSpacingWidth);
			CPoint p3(p2.x, p1.y + m_nSpacingWidth);

			dcMem.BeginPath();
			dcMem.MoveTo(p1);
			dcMem.LineTo(p2);
			dcMem.LineTo(p3);
			dcMem.LineTo(p1);
			dcMem.EndPath();
			dcMem.FillPath();
		}
		dc.BitBlt(rctPaint.left, rctPaint.top, rctPaint.Width(), rctPaint.Height(), &dcMem, 0, 0, SRCCOPY);

		dcMem.DeleteDC();
		bmpMem.DeleteObject();
	}
}


BOOL CMenuEx::OnEraseBkgnd(CDC* pDC)
{
	// 	CRect rctWnd;
	// 	GetWindowRect(rctWnd);
	// 	ScreenToClient(rctWnd);

	CRect rctWnd(0, 0, 0, 0);
	rctWnd.right = m_nBorderWidth * 2 + m_rctFirstItem.Width();
	rctWnd.bottom = m_nBorderWidth * 2 + m_rctFirstItem.Height()*GetItemCount();

	CRgn rgn;
	rgn.CreateRoundRectRgn(0, 0, rctWnd.Width() + 1, rctWnd.Height() + 1, m_nBorderWidth, m_nBorderWidth);
	// draw border
	pDC->FrameRgn(&rgn, &m_brushBorder, m_nBorderWidth, m_nBorderWidth);
	rgn.GetRgnBox(&rctWnd);
	SetWindowPos(NULL, 0, 0, rctWnd.Width(), rctWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowRgn((HRGN)rgn.Detach(), FALSE);

	int nItemCnt = GetItemCount();
	int nItemHeight = m_rctFirstItem.Height();
	CRect rctText = m_rctFirstItem;
	rctText.right -= m_nSpacingWidth;
	rctText.left += m_nSpacingWidth;
	CRect rctCursel = m_rctFirstItem;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_font);

	CBrush brush;
	brush.CreateSolidBrush(m_rgbTextEnable);
	pDC->SelectObject(&brush);
	for (int i = 0; i<nItemCnt; i++)
	{
		MENU_NODE node = m_arrMenuItems.GetAt(i);
		// paint back ground
		pDC->FillSolidRect(&rctCursel, m_rgbBackGnd);

		if (node.item.bEnable)
			pDC->SetTextColor(m_rgbTextEnable);
		else
			pDC->SetTextColor(m_rgbTextDisable);

		// output text
		pDC->DrawText(node.item.strText, rctText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		if (node.right != NULL)
		{	// draw triangle
			CPoint p1(rctText.right, rctText.bottom - nItemHeight / 2);
			CPoint p2(rctText.right - m_nSpacingWidth, p1.y - m_nSpacingWidth);
			CPoint p3(p2.x, p1.y + m_nSpacingWidth);

			pDC->BeginPath();
			pDC->MoveTo(p1);
			pDC->LineTo(p2);
			pDC->LineTo(p3);
			pDC->LineTo(p1);
			pDC->EndPath();
			pDC->FillPath();
		}
		rctText.top += nItemHeight;
		rctText.bottom += nItemHeight;

		rctCursel.top += nItemHeight;
		rctCursel.bottom += nItemHeight;
	}

	return TRUE;
}

void CMenuEx::OnMouseMove(UINT nFlags, CPoint point)
{
	int nOldCursel = m_nCurSel;
	m_nCurSel = __IsPtInItem(point);

	if (m_nCurSel == -1
		&& m_pSubMenuShowing != NULL)
	{
		m_nCurSel = nOldCursel;
	}

	if (nOldCursel != m_nCurSel)
	{
		if (m_pSubMenuShowing != NULL)
		{
			CMenuEx *tmp = m_pSubMenuShowing;
			m_pSubMenuShowing = NULL;
			tmp->ShowMenu();
		}
		if (nOldCursel >= 0)
		{
			CRect rct = m_rctFirstItem;
			rct.top += m_rctFirstItem.Height()*nOldCursel;
			rct.bottom += m_rctFirstItem.Height()*nOldCursel;

			InvalidateRect(rct);
		}
		if (m_nCurSel >= 0)
		{	// mouse is in an item now.
			CRect rct = m_rctFirstItem;
			rct.top += m_rctFirstItem.Height()*m_nCurSel;
			rct.bottom += m_rctFirstItem.Height()*m_nCurSel;

			InvalidateRect(rct);

			MENU_NODE node = m_arrMenuItems.GetAt(m_nCurSel);
			// is there any child menu in this item?
			if (node.right != NULL)
			{	// yes, ther is.
				m_pSubMenuShowing = node.right;
				// show child menu
				CRect rctWnd;
				GetWindowRect(&rctWnd);
				rct.OffsetRect(rctWnd.left, rctWnd.top - m_nBorderWidth);
				m_pSubMenuShowing->ShowMenu(CPoint(rct.right, rct.top), TRUE);
			}
		}
	} // nOldCursel != m_nCurSel

	CWnd::OnMouseMove(nFlags, point);
}

void CMenuEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nCurSel = __IsPtInItem(point);
	if (m_nCurSel >= 0)
	{
		MENU_NODE node = m_arrMenuItems.GetAt(m_nCurSel);
		if (node.right == NULL
			&& node.item.bEnable)
		{
			SendMessage(WM_COMMAND, node.item.uItemID, 0);
		}
	}
}

BOOL CMenuEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	m_pWndParent->SendMessage(WM_COMMAND, wParam, lParam);
	PostMessage(WM_CLOSE);
	return TRUE;
}

void CMenuEx::OnKillFocus(CWnd* pNewWnd)
{
	if (NULL == m_pSubMenuShowing)
	{
		ShowMenu();
	}
}

LRESULT CMenuEx::__OnChildMenuHide(WPARAM, LPARAM)// on  WM_MENU_HIDE
{
	if (m_pSubMenuShowing)
	{
		m_pSubMenuShowing = NULL;
		CPoint ptCursor;
		CRect rctWnd;
		GetCursorPos(&ptCursor);
		GetWindowRect(&rctWnd);
		if (!rctWnd.PtInRect(ptCursor))
		{
			ShowMenu();
		}
	}
	return 0;
}

int CMenuEx::__IsPtInItem(CPoint pt) const
{
	int nItemCnt = GetItemCount();
	int nItemHeight = m_rctFirstItem.Height();
	CRect rctItem = m_rctFirstItem;

	for (int i = 0; i<nItemCnt; i++)
	{

		if (rctItem.PtInRect(pt))
		{
			return i;
		}
		rctItem.top += nItemHeight;
		rctItem.bottom += nItemHeight;
	}
	return -1;
}

BOOL CMenuEx::__InsertItem(MENU_ITEM item, int nPos)
{
	int nItemNum = GetItemCount();
	int i = 0;

	if (nPos < 0 || nPos>nItemNum) {
		nPos = nItemNum;
	}

	MENU_NODE node;
	node.item = item;
	m_arrMenuItems.InsertAt(nPos, node);

	__ResizeMenu();

	return TRUE;
}

void CMenuEx::__ResizeMenu()
{
	int nItemCnt = GetItemCount();
	CDC *pDC = GetDC();
	pDC->SelectObject(&m_font);

	BOOL bChildMenu = FALSE;
	int nMaxTextLength = 0;
	CSize sizeText(0, 0);
	for (int i = 0; i<nItemCnt; i++)
	{
		MENU_NODE node = m_arrMenuItems.GetAt(i);

		sizeText = pDC->GetTextExtent(node.item.strText);
		if (node.right != NULL) {
			bChildMenu = TRUE;
		}
		if (sizeText.cx > nMaxTextLength) {
			nMaxTextLength = sizeText.cx;
		}
	}
	sizeText.cx = nMaxTextLength;
	m_rctFirstItem = CRect(CPoint(m_nBorderWidth, m_nBorderWidth), CSize(sizeText.cx + m_nSpacingWidth * 2, sizeText.cy + m_nSpacingWidth));

	if (bChildMenu)
	{
		m_rctFirstItem.right += m_nSpacingWidth * 2;
	}


	CRect rctWnd(0, 0, 0, 0);
	rctWnd.right = m_nBorderWidth * 2 + m_rctFirstItem.Width();
	rctWnd.bottom = m_nBorderWidth * 2 + m_rctFirstItem.Height()*nItemCnt;

	//  	SetWindowPos(NULL, 0, 0, rctWnd.Width(), rctWnd.Height(), SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);

	ReleaseDC(pDC);
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

BOOL CMenuEx::CreatePopupMenu(CWnd* pParentWnd,
	UINT id /* = 0 */,
	unsigned uBorderWidth /* = BORDER_WIDTH_DEFAULT */,
	unsigned uSpacingWidth /* = SPACING_WIDTH_DEFAULT */,
	COLORREF rgbTextEnable /* = RGB_TEXT_ENABLE_DEFAULT */,
	COLORREF rgbTextDisable /* = RGB_TEXT_DISABLE_DEFAULT */,
	COLORREF rgbBackGround /* = RGB_BACKGROUND_DEFAULT */,
	COLORREF rgbBorder /* = RGB_BORDER_DEFAULT */,
	COLORREF rgbCurSel /* = RGB_CURSEL_DEFAULT */,
	BOOL bDStinvertText /* = FALSE */)
{
	m_id = id;
	m_pWndParent = pParentWnd;

	m_nBorderWidth = uBorderWidth;
	m_nSpacingWidth = uSpacingWidth;
	m_rgbTextEnable = rgbTextEnable;
	m_rgbTextDisable = rgbTextDisable;
	m_rgbBackGnd = rgbBackGround;
	m_rgbCursel = rgbCurSel;
	m_rgbBorder = rgbBorder;
	m_bDstinvertText = bDStinvertText;

	m_brushBorder.CreateSolidBrush(rgbBorder);
	m_brushCursel.CreateSolidBrush(rgbCurSel);

	CString className = AfxRegisterWndClass(0);
	// create window
	return CWnd::CreateEx(WS_EX_TRANSPARENT, className, _T("menu"),
		WS_POPUP, CRect(0, 0, 100, 100), m_pWndParent, m_id);
}

void CMenuEx::ShowMenu(CPoint pt/* =0 */, BOOL bShow/* =FALSE */)
{

	if (bShow)
	{
		SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);
		ShowWindow(SW_NORMAL);

	}
	else
	{
		// hide child menu
		if (m_pSubMenuShowing != NULL)
		{
			CMenuEx *tmp = m_pSubMenuShowing;
			m_pSubMenuShowing = NULL;
			tmp->ShowMenu();
		}
		// hide self
		ShowWindow(SW_HIDE), m_nCurSel = -1;

		// send WM_MENU_HIDE to parent if this is a child menu
		if (s_pMainMenu != this)
		{
			m_pWndParent->SendMessage(WM_MENU_HIDE);
		}
	}
}

BOOL CMenuEx::InsertItem(MENU_ITEM item, int nPos, UINT uParentID /*= -1*/)
{
	BOOL bRet = FALSE;

	int nItemNum = GetItemCount();
	int i = 0;
	if (uParentID != -1)
	{
		for (i = 0; i<nItemNum && (!bRet); i++)
		{
			MENU_NODE& var = m_arrMenuItems.GetAt(i);

			if (var.item.uItemID == uParentID)
			{
				if (var.right == NULL) {
					var.right = new CMenuEx;
					var.right->CreatePopupMenu(this, m_id, m_nBorderWidth, m_nSpacingWidth,
						m_rgbTextEnable, m_rgbTextDisable, m_rgbBackGnd, m_rgbBorder, m_rgbCursel);
					if (m_font.m_hObject)
					{
						var.right->SetFont(&m_font);
					}

					bRet = var.right->__InsertItem(item, nPos);
					if (bRet)
					{
#if _MSC_VER <= 1300	// VC6 时 CArray::GetAt 不返回引用类型
						m_arrMenuItems.RemoveAt(i);
						m_arrMenuItems.InsertAt(i, var);
#endif
					}
					else
					{
						delete var.right;
					}
				}

			}
			else if (var.right != NULL)
			{
				bRet = var.right->InsertItem(item, nPos, uParentID);
			}
		}
	}
	else
	{
		bRet = __InsertItem(item, nPos);
	}

	if (bRet)
	{
		__ResizeMenu();
	}

	return bRet;
}

void CMenuEx::EnableItem(MENU_ITEM item)
{
	int nItemNum = GetItemCount();

	int bRet = FALSE;
	for (int i = 0; i<nItemNum && (!bRet); i++)
	{
		MENU_NODE& node = m_arrMenuItems.GetAt(i);
		if (node.item.uItemID == item.uItemID)
		{
			node.item.bEnable = item.bEnable;
#if _MSC_VER < 1300
			m_arrMenuItems.RemoveAt(i);
			m_arrMenuItems.InsertAt(i, node);
#endif
			break;
		}
		else if (node.right != NULL)
		{
			if (node.right->IsItemIDExsist(item.uItemID))
			{
				node.right->EnableItem(item);
				break;
			}
		}
	}
	Invalidate();
}

inline int CMenuEx::GetItemCount() const
{
	return static_cast<int>(m_arrMenuItems.GetSize());
}

BOOL CMenuEx::IsItemIDExsist(UINT uItemID)
{
	int nItemNum = GetItemCount();

	int bRet = FALSE;
	for (int i = 0; i<nItemNum && (!bRet); i++)
	{
		const MENU_NODE node = m_arrMenuItems.GetAt(i);
		if (node.item.uItemID == uItemID) {
			return TRUE;
		}
		else if (node.right != NULL) {
			bRet = node.right->IsItemIDExsist(uItemID);
		}
	}

	return bRet;
}

void CMenuEx::SetFont(CFont* pFont, BOOL bRedraw /* = TRUE */)
{
	LOGFONT logfont;
	if (pFont->GetLogFont(&logfont))
	{
		m_font.DeleteObject();
		m_font.CreateFontIndirect(&logfont);
	}

	int nItemCnt = GetItemCount();
	for (int i = 0; i<nItemCnt; i++)
	{
		MENU_NODE node = m_arrMenuItems.GetAt(i);
		if (node.right)
		{
			node.right->SetFont(pFont, bRedraw);
		}
	}

	if (bRedraw)
	{
		Invalidate();
	}
}