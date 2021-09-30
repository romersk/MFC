
// MiniDrawDoc.cpp: реализация класса CMiniDrawDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MiniDraw.h"
#endif

#include "MiniDrawDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMiniDrawDoc

IMPLEMENT_DYNCREATE(CMiniDrawDoc, CDocument)
//объявление функций, которые программа должна ис-пользовать для того, чтобы разобраться в картах всех объектов системы
BEGIN_MESSAGE_MAP(CMiniDrawDoc, CDocument) //начало карты сообщений
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CMiniDrawDoc::OnEditClearAll) //перенаправление обработки команды функции-члену класса CMiniDrawDoc
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, &CMiniDrawDoc::OnUpdateEditClearAll)//перенаправление обработки обновления команды функции-члену класса CMiniDrawDoc
	ON_COMMAND(ID_EDIT_UNDO, &CMiniDrawDoc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMiniDrawDoc::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CMiniDrawDoc::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CMiniDrawDoc::OnUpdateEditRedo)
END_MESSAGE_MAP() //конец


// Создание или уничтожение CMiniDrawDoc

CMiniDrawDoc::CMiniDrawDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CMiniDrawDoc::~CMiniDrawDoc()
{
}

BOOL CMiniDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CMiniDrawDoc

void CMiniDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
		m_CirArray.Serialize(ar);
	}
	else
	{
		// TODO: добавьте код загрузки
		m_CirArray.Serialize(ar);
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMiniDrawDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMiniDrawDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMiniDrawDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CMiniDrawDoc

#ifdef _DEBUG
void CMiniDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMiniDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CMiniDrawDoc
IMPLEMENT_SERIAL(CCircle, CObject, 1)
void CCircle::Draw(CDC* PDC) {

	CBrush newbrush;
	newbrush.CreateSolidBrush(cir_color); 
	CBrush* oldbrush = PDC->SelectObject(&newbrush); //заливка
	CPen newpen;
	newpen.CreatePen(PS_SOLID, 4, cir_color);
	CPen* oldpen = PDC->SelectObject(&newpen); //очертание
	
	PDC->Ellipse(m_X1, m_Y1, m_X2, m_Y2); //рисование окружности
}
void CCircle::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		ar << m_X1 << m_Y1 << m_X2 << m_Y2 << cir_color;
	else
		ar >> m_X1 >> m_Y1 >> m_X2 >> m_Y2 >> cir_color;
}
void CMiniDrawDoc::AddCir(int X1, int Y1, int X2, int Y2, COLORREF color)
{
	CCircle* pCir = new CCircle(X1, Y1, X2, Y2, color);
	m_CirArray.Add(pCir);
	//m_CirArray_redo.Add(pCir);
	SetModifiedFlag(); //сообщение об изменении документа
}

CCircle* CMiniDrawDoc::GetCir(int Index)
{
	if (Index<0 || Index>m_CirArray.GetUpperBound())
		return 0;
	return m_CirArray.GetAt(Index);
}

int CMiniDrawDoc::GetNumCirs()
{
	return (int)m_CirArray.GetSize();
}


void CMiniDrawDoc::DeleteContents()
{
	// TODO: добавьте специализированный код или вызов базового класса
	int Index = (int)m_CirArray.GetSize();

	while (Index--)
	{
		m_CirArray_redo.Add(m_CirArray.GetAt(Index));
		m_CirArray.RemoveAt(Index);
	}

	//m_CirArray.RemoveAll();

	//CDocument::DeleteContents();
}


void CMiniDrawDoc::OnEditClearAll()
{
	// TODO: добавьте свой код обработчика команд
	DeleteContents();
	UpdateAllViews(0);
	SetModifiedFlag();
}


void CMiniDrawDoc::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->Enable((int)m_CirArray.GetSize());
}


void CMiniDrawDoc::OnEditUndo()
{
	// TODO: добавьте свой код обработчика команд
	int Index = (int)m_CirArray.GetUpperBound();
	if (Index > -1)
	{
		m_CirArray_redo.Add(m_CirArray.GetAt(Index));
		m_CirArray.RemoveAt(Index);
	}
	UpdateAllViews(0);
	SetModifiedFlag();
}


void CMiniDrawDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->Enable((int)m_CirArray.GetSize());
}

void CMiniDrawDoc::OnEditRedo()
{
	// TODO: добавьте свой код обработчика команд
	int Index2 = (int)m_CirArray_redo.GetUpperBound();
	m_CirArray.Add(m_CirArray_redo.GetAt(Index2));
	m_CirArray_redo.RemoveAt(Index2);
	UpdateAllViews(0); //уведомляет все представления о том, что документ был изменен. 0 - если все должны быть обновлены
	SetModifiedFlag();
}


void CMiniDrawDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->Enable((int)m_CirArray.GetSize()); //включение пользовательского интерфейсf, т.е. обновить к-во кругов
}