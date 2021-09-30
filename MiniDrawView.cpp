
// MiniDrawView.cpp: реализация класса CMiniDrawView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MiniDraw.h"
#endif

#include "MiniDrawDoc.h"
#include "MiniDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniDrawView

IMPLEMENT_DYNCREATE(CMiniDrawView, CView)

BEGIN_MESSAGE_MAP(CMiniDrawView, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMiniDrawView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CMiniDrawView

CMiniDrawView::CMiniDrawView() noexcept
{
	// TODO: добавьте код создания
	m_Dragging = 0;
	m_HCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS); //возращает указатель на объект класса приложения 
}

CMiniDrawView::~CMiniDrawView()
{
}

BOOL CMiniDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs
	m_ClassName = AfxRegisterWndClass(

		CS_HREDRAW | CS_VREDRAW, // стили окна

		0, // без указателя;

		(HBRUSH)::GetStockObject(WHITE_BRUSH), // задать чисто белый фон;

		0); // без значка

	cs.lpszClass = m_ClassName;
	return CView::PreCreateWindow(cs);
}

// Рисование CMiniDrawView

void CMiniDrawView::OnDraw(CDC* pDC)
{
	CMiniDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc); //проверка валидации
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
	int Index = pDoc->GetNumCirs(); //к-во объектов

	while (Index--)
	{
		pDoc->GetCir(Index)->Draw(pDC); //отобразить
	}
}

void CMiniDrawView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMiniDrawView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CMiniDrawView

#ifdef _DEBUG
void CMiniDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CMiniDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMiniDrawDoc* CMiniDrawView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMiniDrawDoc)));
	return (CMiniDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CMiniDrawView


void CMiniDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	m_PointOld = point;
	m_PointOrigin = point;
	SetCapture(); //захват мыши, и все последующие ее сообщения передаются в окно представления
	m_Dragging = 1;
	RECT Cir;
	GetClientRect(&Cir); //текущие координаты окна представления
	ClientToScreen(&Cir); //преобразовывает их в экранные 
	::ClipCursor(&Cir); //ограничивает перемещения указателя мыши в пределах заданных координат
	CView::OnLButtonDown(nFlags, point); //вызов если левая кнопка зажата
}


void CMiniDrawView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	::SetCursor(m_HCross); //установка курсора по дексриптору
	CClientDC ClientDC(this); //класс для рисования в области окна
	if (m_Dragging)
	{
		RedrawWindow(); //обновляет зону рисования
		CPen pen(PS_SOLID, 1, m_color);
		CPen* Origpen = ClientDC.SelectObject(&pen); //границы
		ClientDC.Ellipse(m_PointOrigin.x, m_PointOrigin.y, m_PointOld.x, m_PointOld.y);
		m_PointOld = point; //первая точка уже старая
	}

	CView::OnMouseMove(nFlags, point);
}


void CMiniDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (m_Dragging) {
		m_Dragging = 0;
		::ReleaseCapture(); //завершение захвата мыши
		::ClipCursor(NULL); //отмена ограничений
		CClientDC ClientDC(this);
		CColorDialog cc; //вызов диалогового окна с цветами
		if (cc.DoModal() == IDOK) {
			COLORREF my_color = cc.GetColor();
			m_color = my_color;
		}

		CBrush newbrush;
		newbrush.CreateSolidBrush(m_color);
		CBrush* oldbrush = ClientDC.SelectObject(&newbrush);
		CPen newpen;
		newpen.CreatePen(PS_SOLID, 4, m_color);
		CPen* oldpen = ClientDC.SelectObject(&newpen);

		CMiniDrawDoc* pDoc = GetDocument();
		
		ClientDC.Ellipse(m_PointOrigin.x, m_PointOrigin.y, point.x, point.y);
		pDoc->AddCir(m_PointOrigin.x, m_PointOrigin.y, point.x, point.y, m_color);
	}
	CView::OnLButtonUp(nFlags, point);
}