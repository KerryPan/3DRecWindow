#if !defined(__MENUEX_H__)
#define __MENUEX_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxtempl.h>

#define RGB_BORDER_DEFAULT			RGB(0x93, 0xCC, 0xFB)
#define RGB_TEXT_ENABLE_DEFAULT		RGB(0x00, 0x00, 0x00)
#define RGB_TEXT_DISABLE_DEFAULT	RGB(0xD2, 0xD2, 0xD2)
#define RGB_BACKGROUND_DEFAULT		RGB(0xFF, 0xFF, 0xFF)
#define RGB_CURSEL_DEFAULT			RGB(0xE5, 0xF6, 0xFD)

#define BORDER_WIDTH_DEFAULT		2
#define SPACING_WIDTH_DEFAULT		4

#define IDC_ITEM_ID_BEGIN		2560
#define IDC_ITEM_ID_END			4095

// user message
#define WM_MENU_HIDE		(WM_USER+2013)

namespace MENU_DEF
{
	typedef struct _MENU_ITEM {
		CString	strText;
		UINT	uItemID;
		BOOL	bEnable;

		_MENU_ITEM & operator = (_MENU_ITEM in)
		{
			strText = in.strText;
			uItemID = in.uItemID;
			bEnable = in.bEnable;	// no used

			return *this;
		}
		_MENU_ITEM()
		{
			strText = "";
			uItemID = 0;
			bEnable = TRUE;
		}
		_MENU_ITEM(LPCTSTR strText, UINT uID, BOOL bEnable)
		{
			this->strText = strText;
			this->uItemID = uID;
			this->bEnable = bEnable;
		}
	}MENU_ITEM;
} // namespace MENU_DEF

using namespace MENU_DEF;

class CMenuEx;
typedef struct _MENU_NODE
{
	MENU_ITEM	item;
	CMenuEx*	right;

	_MENU_NODE()
	{
		right = NULL;
	}
}MENU_NODE;

class CMenuEx : public CWnd
{
	CArray<MENU_NODE, MENU_NODE&>	m_arrMenuItems;

	// 构造
public:
	CMenuEx();
	~CMenuEx();

protected:
	UINT	m_id;
	CWnd*	m_pWndParent;

	CFont		m_font;
	COLORREF	m_rgbBorder;
	COLORREF	m_rgbTextEnable;
	COLORREF	m_rgbTextDisable;
	COLORREF	m_rgbBackGnd;
	COLORREF	m_rgbCursel;
	BOOL		m_bDstinvertText;

	CBrush		m_brushBorder;
	CBrush		m_brushCursel;

	int		m_nBorderWidth;
	int		m_nSpacingWidth;

	int		m_nCurSel;
	CMenuEx* m_pSubMenuShowing;

	//CSize	m_sizeItem;
	CRect	m_rctFirstItem;

	virtual BOOL __InsertItem(MENU_ITEM item, int nPos);
	virtual void __ResizeMenu();
	virtual	int __IsPtInItem(CPoint pt) const;

	static CWnd* s_pMainMenu;

public:
	// user interface
	virtual BOOL CreatePopupMenu(CWnd* pParentWnd, UINT id = 0,
		unsigned uBorderWidth = BORDER_WIDTH_DEFAULT,
		unsigned uSpacingWidth = SPACING_WIDTH_DEFAULT,
		COLORREF rgbTextEnable = RGB_TEXT_ENABLE_DEFAULT,
		COLORREF rgbTextDisable = RGB_TEXT_DISABLE_DEFAULT,
		COLORREF rgbBackGround = RGB_BACKGROUND_DEFAULT,
		COLORREF rgbBorder = RGB_BORDER_DEFAULT,
		COLORREF rgbCurSel = RGB_CURSEL_DEFAULT,
		BOOL	bDStinvertText = FALSE
		);
	virtual void ShowMenu(CPoint pt = 0, BOOL bShow = FALSE);

	virtual inline BOOL IsItemIDExsist(UINT uItemID);
	virtual inline int GetItemCount(void) const;
	virtual void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	virtual BOOL InsertItem(MENU_ITEM item, int nPos, UINT uParentID = -1);
	virtual void EnableItem(MENU_ITEM item);

protected:
	afx_msg virtual LRESULT __OnChildMenuHide(WPARAM, LPARAM);

	// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
	afx_msg virtual void OnPaint();
	afx_msg virtual BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg virtual void OnKillFocus(CWnd* pNewWnd);
	afx_msg virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg virtual void OnLButtonUp(UINT nFlags, CPoint point);
};


#endif // __MENUEX_H__