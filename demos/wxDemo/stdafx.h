﻿#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/SouiCtrls.h>
#include <res.mgr/SObjDefAttr.h>
#include <com-cfg.h>
#include <SouiFactory.h>
#include "resource.h"
#define R_IN_CPP	
#include "res/resource.h"
using namespace SOUI;

#include "trayicon/SShellTray.h"
#ifdef _WIN32
#pragma comment(lib, "shlwapi.lib")

#include "FileHelper.h"

#include <fstream>

#include <Imm.h>
#pragma comment(lib, "imm32.lib")

#endif//_WIN32

#endif//_STDAFX_H_
