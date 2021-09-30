
// MiniDrawDoc.h: интерфейс класса CMiniDrawDoc 
//

#include "Afxtempl.h"
#pragma once

class CCircle : public CObject //Круг
{
protected:
	int m_X1, m_Y1, m_X2, m_Y2; //координаты для построения
	COLORREF cir_color; //цвет заливки
	CCircle() {}
	DECLARE_SERIAL(CCircle)
public:
	CCircle(int X1, int Y1, int X2, int Y2, COLORREF cir_color)
	{
		m_X1 = X1; m_Y1 = Y1; m_X2 = X2; m_Y2 = Y2;
		this->cir_color = cir_color;
	}
	void Draw(CDC* PDC);
	virtual void Serialize(CArchive& ar); //функция сохранении-восстановлении данных документа
};

class CMiniDrawDoc : public CDocument
{
protected:
	CTypedPtrArray<CObArray, CCircle*> m_CirArray; //для хранения кругов. CObrArray - один классов общего назначения
	CTypedPtrArray<CObArray, CCircle*> m_CirArray_redo; //для хранения кругов, которые были отменены
public:

	void AddCir(int X1, int Y1, int X2, int Y2, COLORREF color); //добавить круг
	CCircle* GetCir(int Index);
	int GetNumCirs();
protected: // создать только из сериализации
	CMiniDrawDoc() noexcept;
	DECLARE_DYNCREATE(CMiniDrawDoc)
// Атрибуты
public:

// Операции
public:
// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CMiniDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	virtual void DeleteContents();
	afx_msg void OnEditClearAll(); //очистить все
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI); //когда обновлено
	afx_msg void OnEditUndo(); //ctrl + z
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo(); //ctrl + y
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
};
