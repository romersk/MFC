
// MiniDrawView.h: интерфейс класса CMiniDrawView
//

#pragma once


class CMiniDrawView : public CView
{

protected:
	CString m_ClassName;
	int m_Dragging; //переменная о сообщении процесса рисования
	HCURSOR m_HCross; //дескриптор указателя мыши
	CPoint m_PointOld; //старая точка
	CPoint m_PointOrigin; //исходная точка
protected: // создать только из сериализации
	CMiniDrawView() noexcept;
	DECLARE_DYNCREATE(CMiniDrawView)

// Атрибуты
public:
	CMiniDrawDoc* GetDocument() const;
	COLORREF m_color; //цвет

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// Реализация
public:
	virtual ~CMiniDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point); //ф-ция если левая кнопка мыши отпущена
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); //ф-ция если указатель мыши перемещается
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); //ф-ция если левая кнопка мыши зажата
};

#ifndef _DEBUG  // версия отладки в MiniDrawView.cpp
inline CMiniDrawDoc* CMiniDrawView::GetDocument() const
   { return reinterpret_cast<CMiniDrawDoc*>(m_pDocument); }
#endif

