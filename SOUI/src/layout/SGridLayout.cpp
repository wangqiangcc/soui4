﻿#include "souistd.h"
#include "layout/SGridLayout.h"
#include <core/SWnd.h>
SNSBEGIN
GridGravity SGridLayoutParam::parseGridGravity(const SStringW &strValue)
{
    struct ValueMap
    {
        GridGravity gridGravity;
        LPCWSTR pszGravity;
    } map[] = {
        { gLeft, L"left" }, { gTop, L"top" }, { gCenter, L"center" }, { gRight, L"right" }, { gBottom, L"bottom" }, { gFill, L"fill" },
    };

    for (int i = 0; i < ARRAYSIZE(map); i++)
    {
        if (strValue.CompareNoCase(map[i].pszGravity) == 0)
        {
            return map[i].gridGravity;
        }
    }
    return gUndef;
}

HRESULT SGridLayoutParam::OnAttrSize(const SStringW &strValue, BOOL bLoading)
{
    SStringWList szStr;
    if (2 != SplitString(strValue, L',', szStr))
        return E_FAIL;

    OnAttrWidth(szStr[0], bLoading);
    OnAttrHeight(szStr[1], bLoading);
    return S_OK;
}

HRESULT SGridLayoutParam::OnAttrWidth(const SStringW &strValue, BOOL bLoading)
{
    if (strValue.CompareNoCase(L"matchParent") == 0)
        width.setMatchParent();
    else if (strValue.CompareNoCase(L"wrapContent") == 0)
        width.setWrapContent();
    else
        width = GETLAYOUTSIZE(strValue);
    return S_OK;
}

HRESULT SGridLayoutParam::OnAttrHeight(const SStringW &strValue, BOOL bLoading)
{
    if (strValue.CompareNoCase(L"matchParent") == 0)
        height.setMatchParent();
    else if (strValue.CompareNoCase(L"wrapContent") == 0)
        height.setWrapContent();
    else
        height = GETLAYOUTSIZE(strValue);
    return S_OK;
}

SGridLayoutParam::SGridLayoutParam()
{
    Clear();
}

void SGridLayoutParam::Clear()
{
    width.setWrapContent();
    height.setWrapContent();
    nColSpan = nRowSpan = 1;
    layoutGravityX = gUndef;
    layoutGravityY = gUndef;
    fColWeight = 0.0f;
    fRowWeight = 0.0f;
}

void SGridLayoutParam::SetMatchParent(ORIENTATION orientation)
{
    switch (orientation)
    {
    case Horz:
        width.setMatchParent();
        break;
    case Vert:
        height.setMatchParent();
        break;
    case Both:
        width.setMatchParent();
        height.setMatchParent();
        break;
    }
}

void SGridLayoutParam::SetWrapContent(ORIENTATION orientation)
{
    switch (orientation)
    {
    case Horz:
        width.setWrapContent();
        break;
    case Vert:
        height.setWrapContent();
        break;
    case Both:
        width.setWrapContent();
        height.setWrapContent();
        break;
    }
}

void SGridLayoutParam::SetSpecifiedSize(ORIENTATION orientation, const SLayoutSize &layoutSize)
{
    switch (orientation)
    {
    case Horz:
        width = layoutSize;
        break;
    case Vert:
        height = layoutSize;
        break;
    case Both:
        width = height = layoutSize;
        break;
    }
}

BOOL SGridLayoutParam::IsMatchParent(ORIENTATION orientation) const
{
    switch (orientation)
    {
    case Horz:
        return width.isMatchParent();
    case Vert:
        return height.isMatchParent();
    case Any:
        return IsMatchParent(Horz) || IsMatchParent(Vert);
    case Both:
    default:
        return IsMatchParent(Horz) && IsMatchParent(Vert);
    }
}

BOOL SGridLayoutParam::IsWrapContent(ORIENTATION orientation) const
{
    switch (orientation)
    {
    case Horz:
        return width.isWrapContent();
    case Vert:
        return height.isWrapContent();
    case Any:
        return IsWrapContent(Horz) || IsWrapContent(Vert);
    case Both:
    default:
        return IsWrapContent(Horz) && IsWrapContent(Vert);
    }
}

BOOL SGridLayoutParam::IsSpecifiedSize(ORIENTATION orientation) const
{
    switch (orientation)
    {
    case Horz:
        return width.isSpecifiedSize();
    case Vert:
        return height.isSpecifiedSize();
    case Any:
        return IsSpecifiedSize(Horz) || IsSpecifiedSize(Vert);
    case Both:
    default:
        return IsSpecifiedSize(Horz) && IsSpecifiedSize(Vert);
    }
}

SLayoutSize SGridLayoutParam::GetSpecifiedSize(ORIENTATION orientation) const
{
    switch (orientation)
    {
    case Horz:
        return width;
    case Vert:
        return height;
    case Any:
    case Both:
    default:
        SASSERT_MSGA(FALSE, "GetSpecifiedSize can only be applied for Horz or Vert");
        return SLayoutSize();
    }
}

void *SGridLayoutParam::GetRawData()
{
    return (SGridLayoutParamStruct *)this;
}

ILayoutParam *SGridLayoutParam::Clone() const
{
    SGridLayoutParam *pRet = new SGridLayoutParam();
    memcpy(pRet->GetRawData(), (SGridLayoutParamStruct *)this, sizeof(SGridLayoutParamStruct));
    return pRet;
}

//////////////////////////////////////////////////////////////////////////
SGridLayout::SGridLayout(void)
    : m_GravityX(gCenter)
    , m_GravityY(gCenter)
    , m_nCols(-1)
    , m_nRows(-1)
{
}

SGridLayout::~SGridLayout(void)
{
}

BOOL SGridLayout::IsParamAcceptable(const ILayoutParam *pLayoutParam) const
{
    return pLayoutParam->IsClass(SGridLayoutParam::GetClassName());
}

ILayoutParam *SGridLayout::CreateLayoutParam() const
{
    return new SGridLayoutParam();
}

/*
 * MeasureChildren 计算gridlayout的子窗口大小
 */
SIZE SGridLayout::MeasureChildren(const IWindow *pParent, int nWidth, int nHeight) const
{
    SUNUSED(nWidth);
    SUNUSED(nHeight);
    if (m_nCols == -1 && m_nRows == -1)
        return CSize();

    int nCols = m_nCols;
    int nRows = m_nRows;

    int nCells = CalcCells(pParent);
    if (nRows == -1)
        nRows = (nCells + nCols - 1) / nCols;
    else if (nCols == -1)
        nCols = (nCells + nRows - 1) / nRows;

    int cells = nCols * nRows;
    CSize *pCellsSize = new CSize[cells];
    bool *pCellsOccupy = new bool[cells];

    for (int i = 0; i < cells; i++)
    {
        pCellsOccupy[i] = false;
    }

    int iRow = 0, iCol = 0;
    const IWindow *pCell = pParent->GetNextLayoutIChild(NULL);
    while (pCell)
    {
        const SGridLayoutParam *pLayoutParam = (const SGridLayoutParam *)pCell->GetLayoutParam();
        SASSERT(pLayoutParam);
        //将当前网络所占用的空间位置清0
        int colSpan = pLayoutParam->nColSpan;
        int rowSpan = pLayoutParam->nRowSpan;

        colSpan = smin(colSpan, nCols - iCol);
        rowSpan = smin(rowSpan, nRows - iRow);
        SASSERT(colSpan >= 1);
        SASSERT(rowSpan >= 1);
        SASSERT(pCellsOccupy[iRow * nCols + iCol] == false); //保证至少有一个空间可用
        //计算可占用空间
        for (int y = 0; y < rowSpan; y++)
            for (int x = 0; x < colSpan; x++)
            {
                int iCell = (iRow + y) * nCols + iCol + x;
                if (pCellsOccupy[iCell])
                { // colSpan优先
                    rowSpan = y + 1;
                    if (y == 0)
                        colSpan = x + 1;
                    break;
                }
            }

        //计算出网络大小
        CSize szCell;
        ((IWindow *)pCell)->GetDesiredSize(&szCell, SIZE_WRAP_CONTENT, SIZE_WRAP_CONTENT);
        //填充网格,把大小平均分散到网格中。
        szCell.cx /= colSpan;
        szCell.cy /= rowSpan;
        for (int y = 0; y < rowSpan; y++)
            for (int x = 0; x < colSpan; x++)
            {
                int iCell = (iRow + y) * nCols + iCol + x;
                pCellsOccupy[iCell] = true;
                pCellsSize[iCell] = szCell;
            }

        //计算下一个网络的排列位置(先在当前行查找，再到下面行从0开始查找)
        bool bFind = false;
        for (int x = iCol + 1; x < nCols; x++)
        {
            if (!pCellsOccupy[iRow * nCols + x])
            {
                bFind = true;
                iCol = x;
                break;
            }
        }
        for (int y = iRow + 1; y < nRows && !bFind; y++)
            for (int x = 0; x < nCols; x++)
            {
                if (!pCellsOccupy[y * nCols + x])
                {
                    iRow = y;
                    iCol = x;
                    bFind = true;
                    break;
                }
            }
        if (!bFind)
            break;
        pCell = pParent->GetNextLayoutIChild(pCell);
    }

    CSize szRet;
    //计算列宽
    for (int x = 0; x < nCols; x++)
    {
        int maxWid = 0;
        for (int y = 0; y < nRows; y++)
        {
            int iCell = y * nCols + x;
            maxWid = smax(pCellsSize[iCell].cx, maxWid);
        }
        szRet.cx += maxWid;
    }
    //计算列高
    for (int y = 0; y < nRows; y++)
    {
        int maxHei = 0;
        for (int x = 0; x < nCols; x++)
        {
            int iCell = y * nCols + x;
            maxHei = smax(pCellsSize[iCell].cy, maxHei);
        }
        szRet.cy += maxHei;
    }

    delete[] pCellsSize;
    delete[] pCellsOccupy;

    int xInter = m_xInterval.toPixelSize(pParent->GetScale());
    int yInter = m_yInterval.toPixelSize(pParent->GetScale());
    if (xInter < 0)
        xInter = 0;
    if (yInter < 0)
        yInter = 0;
    szRet.cx += xInter * (nCols - 1);
    szRet.cy += yInter * (nRows - 1);
    return szRet;
}

void SGridLayout::LayoutChildren(IWindow *pParent)
{
    if (m_nCols == -1 && m_nRows == -1)
        return;

    int nCols = m_nCols;
    int nRows = m_nRows;

    int nCells = CalcCells(pParent);
    if (nRows == -1)
        nRows = (nCells + nCols - 1) / nCols;
    else if (nCols == -1)
        nCols = (nCells + nRows - 1) / nRows;

    CRect rcParent;
    pParent->GetChildrenLayoutRect(&rcParent);

    //先计算出每个格子的大小,算法和MeasureChildren一样，后面再考虑如何优化
    int cells = nCols * nRows;
    CSize *pCellsSize = new CSize[cells];
    bool *pCellsOccupy = new bool[cells];
    float *pCellsColWeight = new float[cells];
    float *pCellsRowWeight = new float[cells];
    IWindow **pCellsChild = new IWindow *[cells];
    CPoint *pCellsSpan = new CPoint[cells];
    bool *pCellsSpanFlagX = new bool[cells];
    bool *pCellsSpanFlagY = new bool[cells];

    for (int i = 0; i < cells; i++)
    {
        pCellsOccupy[i] = false;
        pCellsChild[i] = NULL;
    }

    int iRow = 0, iCol = 0;
    IWindow *pCell = pParent->GetNextLayoutIChild(NULL);
    while (pCell)
    {
        pCellsChild[iRow * nCols + iCol] = pCell;
        SGridLayoutParam *pLayoutParam = (SGridLayoutParam *)pCell->GetLayoutParam();
        SASSERT(pLayoutParam);
        //将当前网络所占用的空间位置清0
        int colSpan = pLayoutParam->nColSpan;
        int rowSpan = pLayoutParam->nRowSpan;

        colSpan = smin(colSpan, nCols - iCol);
        rowSpan = smin(rowSpan, nRows - iRow);
        SASSERT(colSpan >= 1);
        SASSERT(rowSpan >= 1);
        SASSERT(pCellsOccupy[iRow * nCols + iCol] == false); //保证至少有一个空间可用
        //计算可占用空间
        for (int y = 0; y < rowSpan; y++)
            for (int x = 0; x < colSpan; x++)
            {
                int iCell = (iRow + y) * nCols + iCol + x;
                if (pCellsOccupy[iCell])
                { // colSpan优先
                    rowSpan = y + 1;
                    if (y == 0)
                        colSpan = x + 1;
                    break;
                }
            }

        //计算出网络大小,强制使用-1,-1代表自适应大小
        CSize szCell;
        pCell->GetDesiredSize(&szCell, SIZE_WRAP_CONTENT, SIZE_WRAP_CONTENT);
        //填充网格,把大小平均分散到网格中。
        szCell.cx /= colSpan;
        szCell.cy /= rowSpan;

        float colWeight = pLayoutParam->fColWeight / colSpan;
        float rowWeight = pLayoutParam->fRowWeight / rowSpan;
        for (int y = 0; y < rowSpan; y++)
            for (int x = 0; x < colSpan; x++)
            {
                int iCell = (iRow + y) * nCols + iCol + x;
                pCellsOccupy[iCell] = true;
                pCellsSize[iCell] = szCell;
                pCellsColWeight[iCell] = colWeight;
                pCellsRowWeight[iCell] = rowWeight;
                pCellsSpan[iCell].x = 0;
                pCellsSpan[iCell].y = 0;
                pCellsSpanFlagX[iCell] = colSpan > 1;
                pCellsSpanFlagY[iCell] = rowSpan > 1;
            }
        pCellsSpan[iRow * nCols + iCol] = CPoint(colSpan, rowSpan);

        //计算下一个网络的排列位置(先在当前行查找，再到下面行从0开始查找)
        bool bFind = false;
        for (int x = iCol + 1; x < nCols; x++)
        {
            if (!pCellsOccupy[iRow * nCols + x])
            {
                bFind = true;
                iCol = x;
                break;
            }
        }
        for (int y = iRow + 1; y < nRows && !bFind; y++)
            for (int x = 0; x < nCols; x++)
            {
                if (!pCellsOccupy[y * nCols + x])
                {
                    iRow = y;
                    iCol = x;
                    bFind = true;
                    break;
                }
            }
        if (!bFind)
            break;
        pCell = pParent->GetNextLayoutIChild(pCell);
    }

    int *pCellsWidth = new int[nCols];
    int nTotalWidth = 0;
    float *pColsWeight = new float[nCols];
    float totalColsWeight = 0.0f;
    //计算列宽及相应的weight
    for (int x = 0; x < nCols; x++)
    {
        int maxWid = 0;
        float maxWeight = 0.0f;
        for (int y = 0; y < nRows; y++)
        {
            int iCell = y * nCols + x;
            if (pCellsSpanFlagX[iCell])
                continue;
            maxWid = smax(pCellsSize[iCell].cx, maxWid);
            maxWeight = smax(pCellsColWeight[iCell], maxWeight);
        }
        pCellsWidth[x] = maxWid;
        nTotalWidth += maxWid;
        pColsWeight[x] = maxWeight;
        totalColsWeight += maxWeight;
    }
    //计算列高
    int *pCellsHeight = new int[nRows];
    int nTotalHeight = 0;
    float *pRowsWeight = new float[nRows];
    float totalRowsWeight = 0.0f;
    for (int y = 0; y < nRows; y++)
    {
        int maxHei = 0;
        float maxWeight = 0.0f;
        for (int x = 0; x < nCols; x++)
        {
            int iCell = y * nCols + x;
            if (pCellsSpanFlagY[iCell])
                continue;
            maxHei = smax(pCellsSize[iCell].cy, maxHei);
            maxWeight = smax(pCellsRowWeight[iCell], maxWeight);
        }
        pCellsHeight[y] = maxHei;
        nTotalHeight += maxHei;
        pRowsWeight[y] = maxWeight;
        totalRowsWeight += maxWeight;
    }

    delete[] pCellsOccupy;

    delete[] pCellsColWeight;
    delete[] pCellsRowWeight;
    delete[] pCellsSpanFlagY;
    delete[] pCellsSpanFlagX;

    int xInter = m_xInterval.toPixelSize(pParent->GetScale());
    int yInter = m_yInterval.toPixelSize(pParent->GetScale());
    //分配weight
    if (totalColsWeight > 0.0f)
    {
        int netParentWid = rcParent.Width() - (nCols - 1) * (xInter > 0 ? xInter : 0);
        if (nTotalWidth < netParentWid)
        {
            int nRemain = netParentWid - nTotalWidth;
            for (int i = 0; i < nCols; i++)
            { //采用逐行4舍5入的方式解决不能整除的问题.
                if (SLayoutSize::fequal(totalColsWeight, 0.0f))
                    break;
                int extra = int(nRemain * pColsWeight[i] / totalColsWeight + 0.5f);
                pCellsWidth[i] += extra;
                nRemain -= extra;
                totalColsWeight -= pColsWeight[i];
            }
        }
    }
    else if (xInter < 0)
    {
        int nRemain = rcParent.Width() - nTotalWidth;
        if (xInter == SIZE_WRAP_CONTENT && nCols > 1)
            xInter = nRemain / (nCols - 1);
        else if (xInter == SIZE_MATCH_PARENT)
        {
            xInter = nRemain / (nCols + 1);
            rcParent.DeflateRect(xInter, 0, xInter, 0);
        }
        else
            xInter = 0;
    }
    if (totalRowsWeight > 0.0f)
    {
        int netParentHei = rcParent.Height() - (nRows - 1) * (yInter > 0 ? yInter : 0);
        if (nTotalHeight < netParentHei)
        {
            int nRemain = netParentHei - nTotalHeight;
            for (int i = 0; i < nRows; i++)
            { //采用逐行4舍5入的方式解决不能整除的问题.
                if (SLayoutSize::fequal(totalRowsWeight, 0.0f))
                    break;
                int extra = int(nRemain * pRowsWeight[i] / totalRowsWeight + 0.5f);
                pCellsHeight[i] += extra;
                nRemain -= extra;
                totalRowsWeight -= pRowsWeight[i];
            }
        }
    }
    else if (yInter < 0)
    {
        int nRemain = rcParent.Height() - nTotalHeight;
        if (yInter == SIZE_WRAP_CONTENT && nRows > 1)
            yInter = nRemain / (nRows - 1);
        else if (yInter == SIZE_MATCH_PARENT)
        {
            yInter = nRemain / (nRows + 1);
            rcParent.DeflateRect(0, yInter, 0, yInter);
        }
        else
            yInter = 0;
    }

    delete[] pColsWeight;
    delete[] pRowsWeight;

    //计算子窗口位置
    CPoint pt = rcParent.TopLeft();
    for (int y = 0; y < nRows; y++)
    {
        for (int x = 0; x < nCols; x++)
        {
            int iCell = y * nCols + x;
            if (pCellsSpan[iCell].x == 0 || pCellsSpan[iCell].y == 0)
            {
                pt.x += pCellsWidth[x] + xInter;
                continue;
            }
            SWindow *pCell = (SWindow *)pCellsChild[iCell];
            if (!pCell)
                break;

            SGridLayoutParam *pLayoutParam = (SGridLayoutParam *)pCell->GetLayoutParam();

            CSize szCell;
            for (int xx = 0; xx < pCellsSpan[iCell].x; xx++)
                szCell.cx += pCellsWidth[x + xx];
            for (int yy = 0; yy < pCellsSpan[iCell].y; yy++)
                szCell.cy += pCellsHeight[y + yy];
            szCell.cx += xInter * (pCellsSpan[iCell].x - 1);
            szCell.cy += yInter * (pCellsSpan[iCell].y - 1);

            CSize szDesired = pCellsSize[iCell];
            szDesired.cx *= pCellsSpan[iCell].x;
            szDesired.cy *= pCellsSpan[iCell].y;

            CPoint pt2 = pt;
            GridGravity gx = pLayoutParam->layoutGravityX;
            if (gx == gUndef)
                gx = m_GravityX;
            switch (gx)
            {
            case gUndef:
            case gLeft:
                break;
            case gCenter:
                pt2.x += (szCell.cx - szDesired.cx) / 2;
                break;
            case gRight:
                pt2.x += (szCell.cx - szDesired.cx);
                break;
            case gFill:
                szDesired.cx = szCell.cx;
                break;
            }
            GridGravity gy = pLayoutParam->layoutGravityY;
            if (gy == gUndef)
                gy = m_GravityY;
            switch (gy)
            {
            case gUndef:
            case gTop:
                break;
            case gCenter:
                pt2.y += (szCell.cy - szDesired.cy) / 2;
                break;
            case gBottom:
                pt2.y += (szCell.cy - szDesired.cy);
                break;
            case gFill:
                szDesired.cy = szCell.cy;
                break;
            }
            CRect rcCell(pt2, szDesired);
            pCell->OnRelayout(rcCell);

            pt.x += pCellsWidth[x] + xInter;
        }
        pt.x = rcParent.left;
        pt.y += pCellsHeight[y] + yInter;
    }

    delete[] pCellsSize;
    delete[] pCellsWidth;
    delete[] pCellsHeight;

    delete[] pCellsChild;
    delete[] pCellsSpan;
}

int SGridLayout::CalcCells(const IWindow *pParent) const
{
    int nCells = 0;
    const IWindow *pCell = pParent->GetNextLayoutIChild(NULL);
    while (pCell)
    {
        const SGridLayoutParam *pParam = (const SGridLayoutParam *)pCell->GetLayoutParam();
        SASSERT(pParam);
        int nColSpan = pParam->nColSpan;
        int nRowSpan = pParam->nRowSpan;
        nCells += nColSpan * nRowSpan;
        pCell = pParent->GetNextLayoutIChild(pCell);
    }
    return nCells;
}

SNSEND
