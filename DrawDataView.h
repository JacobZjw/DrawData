
// DrawDataView.h: CDrawDataView 类的接口
//

#pragma once


class CDrawDataView : public CView
{
protected: // 仅从序列化创建
	CDrawDataView() noexcept;
	DECLARE_DYNCREATE(CDrawDataView)

// 特性
public:
	CDrawDataDoc* GetDocument() const;
	CButton m_Math_BUTTON,m_Program_BUTTON,m_Sum_BUTTON;
// 操作
public:
	int m_Num[20];
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDrawDataView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void DrawScore(CDC* pDC, int* fScore, int nNum); //画直方图函数
	void Drawlist(CDC* pDC, int* fScore, int nNum);//画折线图

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMath();
	afx_msg void OnMathLine();
	afx_msg void OnProgramLine();
	afx_msg void OnProgram();
	afx_msg void OnSumLine();
	afx_msg void OnSum();
};

#ifndef _DEBUG  // DrawDataView.cpp 中的调试版本
inline CDrawDataDoc* CDrawDataView::GetDocument() const
   { return reinterpret_cast<CDrawDataDoc*>(m_pDocument); }
#endif

