﻿/**
 * Copyright (C) 2014-2050 SOUI团队
 * All rights reserved.
 *
 * @file       SCaption.h
 * @brief      标题栏控件
 * @version    v1.0
 * @author     soui
 * @date       2014-05-28
 *
 * Describe    此类完成标题栏控件
 */

#ifndef __SCAPTION__H__
#define __SCAPTION__H__
#include "core/SWnd.h"

SNSBEGIN
/**
 * @class      CSCaption
 * @brief      标签类
 *
 * Describe    标签类 只需要继承此类即可
 */
class SOUI_EXP SCaption : public SWindow {
    DEF_SOBJECT(SWindow, L"caption")
  public:
    /**
     * CSCaption::CSCaption
     * @brief    构造函数
     *
     * Describe  CSCaption类的构造函数
     */
    SCaption(void);
    /**
     * CSCaption::~CSCaption
     * @brief    析构函数
     *
     * Describe  ~CSCaption类的构造函数
     */
    virtual ~SCaption(void);

  protected:
    /**
     * CSCaption::OnLButtonDown
     * @brief    左键按下事件
     * @param    UINT nFlags -- 标志
     * @param    CPoint point -- 鼠标坐标
     *
     * Describe  此函数是消息响应函数
     */
    void OnLButtonDown(UINT nFlags, CPoint point);
    /**
     * CSCaption::OnLButtonDblClk
     * @brief    左键双击事件
     * @param    UINT nFlags -- 标志
     * @param    CPoint point -- 鼠标坐标
     *
     * Describe  此函数是消息响应函数
     */
    void OnLButtonDblClk(UINT nFlags, CPoint point);

    /**
     * CSCpation::OnMouseMove
     *
     *
     */
    void OnMouseMove(UINT nFlags, CPoint point);

    /**
     * CSCpation::OnLButtonUp
     *
     *
     */
    void OnLButtonUp(UINT nFlags, CPoint point);

    SOUI_MSG_MAP_BEGIN()
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_LBUTTONUP(OnLButtonUp)
    SOUI_MSG_MAP_END()
  private:
    BOOL m_bIsMaxDown; //最大化时,鼠标左键是否已经按下
};
SNSEND

#endif // __SCAPTION__H__