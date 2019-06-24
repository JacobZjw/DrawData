
// DrawDataView.cpp: CDrawDataView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DrawData.h"
#endif

#include "DrawDataDoc.h"
#include "DrawDataView.h"
#include "Student.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawDataView

IMPLEMENT_DYNCREATE(CDrawDataView, CView)

BEGIN_MESSAGE_MAP(CDrawDataView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDC_Math, &CDrawDataView::OnMath)
	ON_COMMAND(IDC_MATH_LINE, &CDrawDataView::OnMathLine)
	ON_COMMAND(IDC_PROGRAM_LINE, &CDrawDataView::OnProgramLine)
	ON_COMMAND(IDC_PROGRAM, &CDrawDataView::OnProgram)
	ON_COMMAND(IDC_SUM_LINE, &CDrawDataView::OnSumLine)
	ON_COMMAND(IDC_SUM, &CDrawDataView::OnSum)
END_MESSAGE_MAP()

// CDrawDataView 构造/析构

CDrawDataView::CDrawDataView() noexcept
{
	// TODO: 在此处添加构造代码

}

CDrawDataView::~CDrawDataView()
{
}

BOOL CDrawDataView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDrawDataView 绘图

void CDrawDataView::OnDraw(CDC* pDC)
{
	CDrawDataDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDrawDataView 打印

BOOL CDrawDataView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDrawDataView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDrawDataView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CDrawDataView 诊断

#ifdef _DEBUG
void CDrawDataView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawDataView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawDataDoc* CDrawDataView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawDataDoc)));
	return (CDrawDataDoc*)m_pDocument;
}
#endif //_DEBUG


// CDrawDataView 消息处理程序
void CDrawDataView::DrawScore(CDC* pDC, int* fScore, int nNum) //画直方图函数
{
	// fScore是成绩数组指针，nNum是学生人数
	int nScoreNum[] = { 0, 0, 0, 0, 0 };  // 各成绩段的人数的初 始值 
	// 下面是用来统计各分数段的人数   
	for (int i = 0; i < nNum; i++)
	{
		int nSeg = (int)(fScore[i]) / 10; // 取数的"十"位上的值   
		if (nSeg < 6) nSeg = 5; // <60分    
		if (nSeg >= 10) nSeg = 9;// 当为100分，算为>90分数段   
		nScoreNum[nSeg - 5] ++;   // 各分数段计数   
	}
	int nSegNum = sizeof(nScoreNum) / sizeof(int); // 计算有多少个分数段

	// 求分数段上最大的人数 
	int nNumMax = nScoreNum[0];
	for (int i = 1; i < nSegNum; i++)
	{
		if (nNumMax < nScoreNum[i])
			nNumMax = nScoreNum[i];
	}
	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(180, 40);      // 缩小矩形大小  
	int nSegWidth = rc.Width() / nSegNum;   // 计算每段的宽度  
	int nSegHeight = rc.Height() / nNumMax;   // 计算每段的单位高度  

	COLORREF crSeg = RGB(0, 0, 192);  // 定义一个颜色变量   
	CBrush brush1(HS_FDIAGONAL, crSeg);
	CBrush brush2(HS_BDIAGONAL, crSeg);

	CPen   pen(PS_INSIDEFRAME, 2, crSeg);
	CBrush* oldBrush = pDC->SelectObject(&brush1);   // 将brush1选入设备环境  

	CPen* oldPen = pDC->SelectObject(&pen);  // 将pen选 入设备环境  
	CRect rcSeg(rc);

	rcSeg.right = rcSeg.left + nSegWidth;   // 使每段的矩形宽度等于nSegWidth  
	CString strSeg[] = { (CString)"<60", (CString)"60-70", (CString)"70-80", (CString)"80-90", (CString)">=90" };
	CRect rcStr;
	for (int i = 0; i < nSegNum; i++)
	{  // 保证相邻的矩形填充样式不相同    
		if (i % 2)
			pDC->SelectObject(&brush2);
		else
			pDC->SelectObject(&brush1);
		rcSeg.top = rcSeg.bottom - nScoreNum[i] * nSegHeight - 2;  // 计算每段矩形的高度   
		pDC->Rectangle(rcSeg);
		if (nScoreNum[i] > 0)
		{
			CString str;
			str.Format((CString)"%d人", nScoreNum[i]);
			pDC->DrawText(str, rcSeg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		rcStr = rcSeg;
		rcStr.top = rcStr.bottom + 2;
		rcStr.bottom += 20;
		pDC->DrawText(strSeg[i], rcStr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rcSeg.OffsetRect(nSegWidth, 0);  // 右移矩形   
	}
	pDC->SelectObject(oldBrush);  // 恢复原来的画刷属性   
	pDC->SelectObject(oldPen);    // 恢复原来的画笔属性 

	CDC* pControlDC = pDC;
	pControlDC->SelectStockObject(BLACK_BRUSH);//设置画刷
	CString str;
	//pControlDC->MoveTo(180, 40);  //画y轴
	//pControlDC->LineTo(180, 380);
	//pDC->TextOut(180, 400, _T("人数"));
	//pControlDC->MoveTo(180, 380);//画x轴
	//pControlDC->LineTo(900, 380);
	//pDC->TextOut(950, 380, _T("成绩/分"));

}

void CDrawDataView::Drawlist(CDC* pDC, int* fScore, int nNum)//画折线图
{
	// fScore是成绩数组指针，nNum是学生人数
	int nScoreNum[] = { 0, 0, 0, 0, 0 };  // 各成绩段的人数的初 始值 
	// 下面是用来统计各分数段的人数   
	for (int i = 0; i < nNum; i++)
	{
		int nSeg = (int)(fScore[i]) / 10; // 取数的"十"位上的值   
		if (nSeg < 6) nSeg = 5; // <60分    
		if (nSeg >= 10) nSeg = 9;// 当为100分，算为>90分数段   
		nScoreNum[nSeg - 5] ++;   // 各分数段计数   
	}
	//CDC* pControlDC = pDC;
	//pControlDC->SelectStockObject(BLACK_BRUSH);//设置画刷
	
	

	CString str;
	CString strSeg[] = { (CString)"<60", (CString)"60-70", (CString)"70-80", (CString)"80-90", (CString)">=90" };

	//绘制X/Y轴
	CPen pen;
	pen.CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	CPen* oldpen = pDC->SelectObject(&pen);
	//Y轴
	pDC->MoveTo(70, 30);//画线的开始位置
	pDC->LineTo(70, 450);
	pDC->TextOut(2, 40, _T("人数/人"));
	//绘制箭头
	pDC->MoveTo(60, 40);
	pDC->LineTo(70, 30);
	pDC->LineTo(80, 40);

	//X轴
	pDC->MoveTo(70, 450);//画线的开始位置
	pDC->LineTo(700, 450);
	pDC->TextOut(700, 455, _T("成绩/分"));
	//绘制箭头
	pDC->MoveTo(690,440);
	pDC->LineTo(700,450);
	pDC->LineTo(690,460);

	//绘制折线
	CPen pen1;
	pen1.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	CPen* newpen = pDC->SelectObject(&pen1);
	CDC* pControlDC = pDC;

	pControlDC->MoveTo(70, 450);
	
	for (int i = 0; i < 5; i++)
	{
		pControlDC->LineTo(i * 120+120, 450 - (450 * nScoreNum[i] / nNum));//换两点之间的线
		str.Format("%d人", nScoreNum[i]);
		pControlDC->TextOut(i * 120 + 120, 450 - (450 * nScoreNum[i] / nNum) - 30, str);//在线的上方输出文字
		pControlDC->TextOut(i * 120 + 120, 460, strSeg[i]);
	}
}

void CDrawDataView::OnMath()
{
	// TODO: 在此添加命令处理程序代码
	CFile file;
	if (!file.Open("C:\\Users\\17810\\source\\repos\\课程设计—学生信息管理系统\\Student Information Management System\\studentfile.dat", CFile::modeReadWrite | CFile::shareDenyNone))
	{
		MessageBox(_T("文件不存在！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	Student u;
	int i = 0;
	while (file.Read(&u, sizeof(u)) == sizeof(u))
	{
		m_Num[i] = _ttoi((CString)u.math);
		i++;
	}
	InvalidateRect(NULL);
	UpdateWindow();
	DrawScore(GetDC(), m_Num, i);
	file.Close();
}


void CDrawDataView::OnMathLine()
{
	// TODO: 在此添加命令处理程序代码
	CFile file;
	if (!file.Open("C:\\Users\\17810\\source\\repos\\课程设计—学生信息管理系统\\Student Information Management System\\studentfile.dat", CFile::modeReadWrite | CFile::shareDenyNone))
	{
		MessageBox(_T("文件不存在！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	Student u;
	int i = 0;
	while (file.Read(&u, sizeof(u)) == sizeof(u))
	{
		m_Num[i] = _ttoi((CString)u.math);
		i++;
	}
	InvalidateRect(NULL);
	UpdateWindow();
	Drawlist(GetDC(), m_Num, i);
	file.Close();

}


void CDrawDataView::OnProgramLine()
{
	// TODO: 在此添加命令处理程序代码
	CFile file;
	if (!file.Open("C:\\Users\\17810\\source\\repos\\课程设计—学生信息管理系统\\Student Information Management System\\studentfile.dat", CFile::modeReadWrite | CFile::shareDenyNone))
	{
		MessageBox(_T("文件不存在！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	Student u;
	int i = 0;
	while (file.Read(&u, sizeof(u)) == sizeof(u))
	{
		m_Num[i] = _ttoi((CString)u.program);
		i++;
	}
	InvalidateRect(NULL);
	UpdateWindow();
	Drawlist(GetDC(), m_Num, i);
	file.Close();
}


void CDrawDataView::OnProgram()
{
	// TODO: 在此添加命令处理程序代码
	CFile file;
	if (!file.Open("C:\\Users\\17810\\source\\repos\\课程设计—学生信息管理系统\\Student Information Management System\\studentfile.dat", CFile::modeReadWrite | CFile::shareDenyNone))
	{
		MessageBox(_T("文件不存在！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	Student u;
	int i = 0;
	while (file.Read(&u, sizeof(u)) == sizeof(u))
	{
		m_Num[i] = _ttoi((CString)u.program);
		i++;
	}
	InvalidateRect(NULL);
	UpdateWindow();
	DrawScore(GetDC(), m_Num, i);
	file.Close();
}


void CDrawDataView::OnSumLine()
{
	// TODO: 在此添加命令处理程序代码
	CFile file;
	if (!file.Open("C:\\Users\\17810\\source\\repos\\课程设计—学生信息管理系统\\Student Information Management System\\studentfile.dat", CFile::modeReadWrite | CFile::shareDenyNone))
	{
		MessageBox(_T("文件不存在！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	Student u;
	u.Sum();
	int i = 0;
	while (file.Read(&u, sizeof(u)) == sizeof(u))
	{
		m_Num[i] = _ttoi((CString)u.sum);
		i++;
	}
	InvalidateRect(NULL);
	UpdateWindow();
	Drawlist(GetDC(), m_Num, i);
	file.Close();
}


void CDrawDataView::OnSum()
{
	// TODO: 在此添加命令处理程序代码
	CFile file;
	if (!file.Open("C:\\Users\\17810\\source\\repos\\课程设计—学生信息管理系统\\Student Information Management System\\studentfile.dat", CFile::modeReadWrite | CFile::shareDenyNone))
	{
		MessageBox(_T("文件不存在！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	Student u;
	int i = 0;
	while (file.Read(&u, sizeof(u)) == sizeof(u))
	{
		m_Num[i] = _ttoi(u.sum);
		i++;
	}
	InvalidateRect(NULL);
	UpdateWindow();
	DrawScore(GetDC(), m_Num, i);
	file.Close();
}
