
// DigitalImageProcessingDoc.h : интерфейс класса CDigitalImageProcessingDoc
//


#pragma once
#include <vector>
#include "imagedata.h"

#include "DataProcess.h"
#include "atltypes.h"

using namespace std;

class CDigitalImageProcessingDoc : public CDocument
{
protected: // создать только из сериализации
	CDigitalImageProcessingDoc();
	DECLARE_DYNCREATE(CDigitalImageProcessingDoc)

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
	virtual ~CDigitalImageProcessingDoc();
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
	// Список выбранных регионов
	vector<CRectTracker> m_trackers;
	// Основная обрабатывающая функция
	CDataProcess* m_pTotalImageProcess;

	//CRectTracker m_tracker;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	// Загруженное изображение
	CImageData m_LoadedImage;
	virtual void OnCloseDocument();
	// обработанное изображение
	CImageData* m_pProcessedImage;
	// обработать изображение
	void ProcessImage(void);
	// добавить обрабатываемый регион
	void AddRegion(CRectTracker& tracker);
	// вектор классов, характеризующих обработку выделенных регионов
	vector<CDataProcess*> m_regionProcess;
	// установить новый обработчик яркости изображения
	void ChangeMainDataProcess(CDataProcess* dataProcess);
	// Очистить сведения об обработке данных
	void ClearProcessData(void);
	// размер отображаемого изображения. Используется для определения масштабных коэффициентов, применяемых к выделенным регионам
	CSize m_rectScale;
	virtual void DeleteContents();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
