﻿/**
 * Copyright (C) 2014-2050
 * All rights reserved.
 *
 * @file       SwndContainer-i.h
 * @brief
 * @version    v1.0
 * @author     SOUI group
 * @date       2014/08/02
 *
 * Describe    DUI窗口容器接口
 */

#ifndef __SWNDCONTAINER_I__H__
#define __SWNDCONTAINER_I__H__

#include <core/SDefine.h>
#include <interface/SEvtArgs-i.h>
#include <interface/SMsgLoop-i.h>
#include <interface/SScriptModule-i.h>
#include <interface/scaret-i.h>
#include <interface/STimelineHandler-i.h>
#include <interface/SAccelerator-i.h>

SNSBEGIN

enum
{
    ZORDER_MIN = 0,
    ZORDER_MAX = (UINT)-1,
};

typedef enum GrtFlag
{
    GRT_NODRAW = 0,
    GRT_PAINTBKGND,
    GRT_OFFSCREEN,
} GrtFlag;

/**
 * @struct     ISwndContainer
 * @brief      SOUI窗口容器接口
 *
 * Describe
 */

typedef struct IRegionS IRegionS;

#undef INTERFACE
#define INTERFACE ISwndContainer
DECLARE_INTERFACE_(ISwndContainer, ITimelineHandlersMgr)
{
    /**
     * @brief 注册动画帧处理接口
     * @param pHandler ITimelineHandler* -- 动画帧处理接口
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, RegisterTimelineHandler)(THIS_ ITimelineHandler * pHandler) PURE;

    /**
     * @brief 注销动画帧处理接口
     * @param pHandler ITimelineHandler* -- 动画帧处理接口
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, UnregisterTimelineHandler)(THIS_ ITimelineHandler * pHandler) PURE;

    //-----------------------------------------------------------------------------

    /**
     * @brief 向Swnd注册IDropTarget
     * @param swnd SWND -- 目标窗口
     * @param pDropTarget IDropTarget* -- IDropTarget接口
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, RegisterDragDrop)(THIS_ SWND swnd, IDropTarget * pDropTarget) PURE;

    /**
     * @brief 从Swnd注销IDropTarget
     * @param swnd SWND -- 目标窗口
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, UnregisterDragDrop)(THIS_ SWND swnd) PURE;

    /**
     * @brief 响应事件
     * @param evt IEvtArgs* -- 事件指针
     * @return BOOL -- TRUE: 事件被处理
     */
    STDMETHOD_(BOOL, OnFireEvent)(THIS_ IEvtArgs * evt) PURE;

    /**
     * @brief 获取当前的Message Loop
     * @return IMessageLoop* -- 当前的Message Loop
     */
    STDMETHOD_(IMessageLoop *, GetMsgLoop)(THIS) PURE;

    /**
     * @brief 获取脚本模块
     * @return IScriptModule* -- 脚本模块
     */
    STDMETHOD_(IScriptModule *, GetScriptModule)(THIS) PURE;

    /**
     * @brief 获取宿主窗口HWND
     * @return HWND -- 宿主窗口HWND
     */
    STDMETHOD_(HWND, GetHostHwnd)(THIS) PURE;

    /**
     * @brief 获取宿主窗口半透明标志
     * @return BOOL -- TRUE: 宿主窗口半透明
     */
    STDMETHOD_(BOOL, IsTranslucent)(CTHIS) SCONST PURE;

    /**
     * @brief 获取将mouseWheel消息分发到hover窗口的标志
     * @return BOOL -- TRUE: mouseWheel消息发到hover窗口，FALSE: mouseWheel分发到焦点窗口
     */
    STDMETHOD_(BOOL, IsSendWheel2Hover)(CTHIS) SCONST PURE;

    /**
     * @brief 获取当前dpi放大倍数
     * @return int -- 放大倍数
     */
    STDMETHOD_(int, GetScale)(CTHIS) SCONST PURE;

    /**
     * @brief 获取翻译上下文
     * @return LPCWSTR -- 翻译上下文
     */
    STDMETHOD_(LPCWSTR, GetTranslatorContext)(CTHIS) SCONST PURE;

    /**
     * @brief 获取容器显示位置
     * @param ret RECT* -- 容器显示位置
     * @return void
     */
    STDMETHOD_(void, GetContainerRect)(CTHIS_ RECT * ret) SCONST PURE;

    /**
     * @brief 刷新指定区域的内存位图
     * @param rgn IRegionS* -- 刷新位置
     * @return void
     */
    STDMETHOD_(void, UpdateRegion)(THIS_ IRegionS * rgn) PURE;

    /**
     * @brief 请求重绘
     * @param rc LPCRECT -- 重绘位置
     * @param bClip BOOL -- 是否剪裁
     * @return void
     */
    STDMETHOD_(void, OnRedraw)(THIS_ LPCRECT rc, BOOL bClip) PURE;

    /**
     * @brief 获取捕获鼠标的Swnd对象
     * @return SWND -- 捕获鼠标的Swnd对象
     */
    STDMETHOD_(SWND, OnGetSwndCapture)(CTHIS) SCONST PURE;

    /**
     * @brief 设置捕获鼠标的Swnd对象
     * @param swnd SWND -- 捕获鼠标的Swnd对象
     * @return SWND -- 原捕获鼠标的Swnd对象
     */
    STDMETHOD_(SWND, OnSetSwndCapture)(THIS_ SWND swnd) PURE;

    /**
     * @brief 释放捕获鼠标的Swnd
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, OnReleaseSwndCapture)(THIS) PURE;

    /**
     * @brief 获取Hover的Swnd
     * @return SWND -- Hover的Swnd
     */
    STDMETHOD_(SWND, GetHover)(CTHIS) SCONST PURE;

    /**
     * @brief 获取焦点的Swnd
     * @return SWND -- 焦点的Swnd
     */
    STDMETHOD_(SWND, GetFocus)(CTHIS) SCONST PURE;

    /**
     * @brief 设置窗口焦点
     * @param swnd SWND -- 焦点窗口
     * @return void
     */
    STDMETHOD_(void, OnSetSwndFocus)(THIS_ SWND swnd) PURE;

    /**
     * @brief 请求立即更新窗口
     * @param bForce BOOL -- TRUE: 强制刷新，FALSE: 有invalid region才刷新
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, UpdateWindow)(THIS_ BOOL bForce DEF_VAL(TRUE)) PURE;

    /**
     * @brief 请求更新tooltip
     * @return void
     */
    STDMETHOD_(void, UpdateTooltip)(THIS) PURE;

    /**
     * @brief 在指定位置显示tooltip
     * @param rc LPCRECT -- tip显示位置，相对于container
     * @param tipAlign UINT -- tip对齐方式
     * @param pszTip LPCTSTR -- tip内容，为null时退出固定tip
     * @return void
     */
    STDMETHOD_(void, SetToolTip)(THIS_ LPCRECT rc, UINT tipAlign, LPCTSTR pszTip) PURE;

    /**
     * @brief 将当前Frame的矩形坐标转换成最终Host的坐标
     * @param rc RECT* -- 待转换的矩形
     * @return void
     */
    STDMETHOD_(void, FrameToHost)(CTHIS_ RECT * rc) SCONST PURE;

    /**
     * @brief 获取加速键管理器
     * @return IAcceleratorMgr* -- 加速键管理器
     */
    STDMETHOD_(IAcceleratorMgr *, GetAcceleratorMgr)(THIS) PURE;

    /**
     * @brief 将Swnd注册为TrackMouseEvent窗口
     * @param swnd SWND -- 目标窗口
     * @return BOOL -- TRUE: 成功
     * @remark 注册为TrackMouseEvent后，该Swnd被其它窗口挡住仍然可以捕获hover, leave消息
     */
    STDMETHOD_(BOOL, RegisterTrackMouseEvent)(THIS_ SWND swnd) PURE;

    /**
     * @brief 将Swnd注销TrackMouseEvent窗口
     * @param swnd SWND -- 目标窗口
     * @return BOOL -- TRUE: 成功
     * @remark 注册为TrackMouseEvent后，该Swnd被其它窗口挡住仍然可以捕获hover, leave消息
     */
    STDMETHOD_(BOOL, UnregisterTrackMouseEvent)(THIS_ SWND swnd) PURE;

    /**
     * @brief 标记窗口树的zorder失效
     * @return void
     */
    STDMETHOD_(void, MarkWndTreeZorderDirty)(THIS) PURE;

    /**
     * @brief 重建窗口树的zorder
     * @return void
     */
    STDMETHOD_(void, BuildWndTreeZorder)(THIS) PURE;

    /**
     * @brief 启用/关闭输入法
     * @param bEnable BOOL -- 启用/关闭输入法标志
     * @return void
     */
    STDMETHOD_(void, EnableIME)(THIS_ BOOL bEnable) PURE;

    /**
     * @brief 更新Cursor
     * @return void
     */
    STDMETHOD_(void, OnUpdateCursor)(THIS) PURE;

    /**
     * @brief 注册VideoCanvas窗口
     * @param swnd SWND -- VideoCanvas handle
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, RegisterVideoCanvas)(THIS_ SWND swnd) PURE;

    /**
     * @brief 注销VideoCanvas窗口
     * @param swnd SWND -- VideoCanvas handle
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, UnregisterVideoCanvas)(THIS_ SWND swnd) PURE;

    /**
     * @brief 启用/禁用宿主私有UI定义
     * @param bEnable BOOL -- 启用/禁用标志
     * @return void
     */
    STDMETHOD_(void, EnableHostPrivateUiDef)(THIS_ BOOL bEnable) PURE;

    /**
     * @brief 提交任务
     * @param runable IRunnable* -- 任务对象
     * @param bAsync BOOL -- 是否异步执行
     * @return BOOL -- TRUE: 成功
     */
    STDMETHOD_(BOOL, PostTask)(THIS_ IRunnable * runable, BOOL bAsync DEF_VAL(TRUE)) PURE;

    /**
     * @brief 移除对象的任务
     * @param pObj void* -- 对象指针
     * @return int -- 移除的任务数量
     */
    STDMETHOD_(int, RemoveTasksForObject)(THIS_ void *pObj) PURE;
};

SNSEND

#endif // __SWNDCONTAINER_I__H__
