"""Typed scratch rendering of the frozen OwnerDraw control-proc export."""
import hashlib
from types import SimpleNamespace
from seed_guarded_cfg import guarded_graph, require
ENTRY=0x5ee460
RAW_SHA256='e8e582a25861f7806892f3cdbfd30e6bde0f0244c5d0b0f8bbe19fc453d3706e'
BODY_SHA256='d1b1900d473568c47f7075ba81e79cbabec75bdc8d3e2ac704efa1d7b45202ba'
PROVIDERS={5385088: ('?Unknown_Remove_Modeless_Dialog_00522b80@@YIXPAUHWND__@@@Z', 0), 6154640: ('??8UnknownPairKey@OwnerDraw@@QBE_NABU01@@Z', 4), 6169264: ('?unknown_5e22b0@OwnerDraw@@YIPBDIPAUHWND__@@@Z', 0), 6185392: ('?unknown_show_dialog_5e61b0@OwnerDraw@@YI_NPAUHWND__@@@Z', 0), 6218000: ('?unknown_show_tooltip_5ee110@OwnerDraw@@YI_N_N@Z', 0), 6292304: ('?Release_00600350@UnknownListOwner_00600350@@QAEXXZ', 0), 6297936: ('?Remove_Entry@?$WolDictionary@PAUHWND__@@P6GJPAU1@IIJ@Z@@QAE_NABQAUHWND__@@PAP6GJPAU2@IIJ@Z@Z', 8), 6298672: ('?Key_Hash@?$WolDictionary@PAUHWND__@@P6GJPAU1@IIJ@Z@@QBEIABQAUHWND__@@@Z', 4), 6298704: ('?Remove_Entry@?$WolDictionary@IUUnknownControlRecord@OwnerDraw@@@@QAE_NABIPAUUnknownControlRecord@OwnerDraw@@@Z', 8), 6300320: ('?Grow_Table@?$WolDictionary@UUnknownPairKey@OwnerDraw@@H@@QAEXXZ', 0), 6300480: ('?Key_Hash@?$WolDictionary@UUnknownPairKey@OwnerDraw@@_N@@QBEIABUUnknownPairKey@OwnerDraw@@@Z', 4), 6301840: ('?Shrink_Table@?$WolDictionary@UUnknownPairKey@OwnerDraw@@H@@QAEXXZ', 0), 7281456: ('?Unknown_Rect_Shrink_006F1B30@@YIXPAVRect@@HH@Z', 4), 7315696: ('?Unknown_006FA0F0@@YIXPADPBGH@Z', 4), 7558800: ('?Window_Rect_In_Main_Client@@YIHPAUHWND__@@PAURECT@@@Z', 0), 7568784: ('?IME_Reset@@YAXXZ', 0), 7569344: ('?IME_Process_Message@@YIHPAUHWND__@@IIJ@Z', 8), 7569584: ('?IME_Get_Default_Result_007380b0@@YIJXZ', 0), 7797168: ('??0WideString@@QAE@XZ', 0), 7797264: ('??0WideString@@QAE@ABV0@@Z', 4), 7797328: ('??1WideString@@QAE@XZ', 0), 7797712: ('??4WideString@@QAEAAV0@ABV0@@Z', 4), 7799792: ('?Get@WideString@@QBEPBGXZ', 0), 7799856: ('?get@Leaf_770430@@QAEIXZ', 0), 7871803: ('?FUN_00781d3b@seed_00781d3b@@YAXI@Z', 0), 6299712: ('?Remove_Entry@?$WolDictionary@UUnknownPairKey@OwnerDraw@@_N@@QAE_NABUUnknownPairKey@OwnerDraw@@PA_N@Z', 8), 6302016: ('?Grow_Vector@?$WolArrayList@PAUHWND__@@@@QAE_NXZ', 0), 7315136: ('?Load_String@StringTable@@SIPBGPBDPAD0H@Z', 8), 7797184: ('??0WideString@@QAE@PBG@Z', 4), 6154624: ('?FUN_005de980@seed_005de980@@YIHPAH@Z', 0), 6168928: ('?unknown_5e2160@OwnerDraw@@YI?AVWideString@@PAUHWND__@@H@Z', 4), 6169024: ('?unknown_5e21c0@OwnerDraw@@YI?AVWideString@@PAUHWND__@@0HH@Z', 12)}
CANONICAL_EXTENTS={6299712: (607, 'dca9d035fcf231c45d8d24282f9fa37bc41a4fa8d4b86705f0df21f686121f15', 8), 6302016: (143, '778e96b8e7df949a85fc4d867c0022e6fb0bc6a973121778243cd9b7dae294fe', 0), 7315136: (180, 'f3f3cf5fb48d439d60e4815caee87c72cc81270d1ce2d76dec0cc0a49e53215f', 8), 7797184: (76, '2d7d6f38a477a4d3e156b6c0bbc03659bad26137b74990ad8382f6c2f6738980', 4), 6154624: (7, '57c8f1a24fda258e951a9c45872008c4d0b22bf6aacdc0d2b401ac70233a5582', 0), 6168928: (84, '067d4b1b0060ec859ae495a5798af6bad9da34075c0409655884ebbefab81683', 4), 6169024: (152, 'e24072de9644e9496524af8c5aba601bbb2982712ff836908cb3e36e96f10e10', 12)}
STATIC_ADDRESSES=(0xa73398,0xa73918,0x7e8d40,0x7e8d44,0xa76404,0xa76400,0xa7640c)

def canonical_cleanup(receivers,entry):
    # These seven pass7 inventory rows omit reachable epilogues. Their complete
    # extents are already independently accepted in the canonical catalog.
    # This seed-local path neither infers bounds from a candidate nor changes
    # inventory, generic receiver checks, or acceptance tooling.
    size,digest,expected=CANONICAL_EXTENTS[entry]
    row=receivers.accepted(entry)
    require(row['body_bytes']==size and row['body_sha256']==digest,
            'Accepted OwnerDraw provider extent differs')
    raw=receivers.audit.original(entry,size)
    require(hashlib.sha256(raw).hexdigest()==digest,'Accepted OwnerDraw provider bytes differ')
    bound=dict(contiguous=True,ranges=[dict(start=f'{entry:08x}',
        end_inclusive=f'{entry+size-1:08x}',bytes_hex=raw.hex())])
    evidence=SimpleNamespace(function=lambda address:(None,bound) if address==entry else receivers.inventory.function(address))
    graph=guarded_graph(receivers.audit,evidence,entry)
    widths={int(i.operands[0].imm) if i.operands else 0 for i in graph['nodes'].values() if i.mnemonic=='ret'}
    require(widths=={expected},'Accepted OwnerDraw provider cleanup differs')
    return expected

def verify_graph(graph,receivers):
    require(graph['body_sha256']==BODY_SHA256,'Frozen OwnerDraw exit body differs')
    for target,(symbol,cleanup) in PROVIDERS.items():
        require(receivers.accepted(target)['symbol']==symbol,'OwnerDraw exit provider symbol differs')
        actual=canonical_cleanup(receivers,target) if target in CANONICAL_EXTENTS else receivers.cleanup(target)
        require(actual==cleanup,'OwnerDraw exit provider cleanup differs')

def verify_native(inventory,repository):
    import catalog_io
    from seed_capture_receivers import Receivers
    from verify_manifest import Audit
    audit=Audit(repository,'private/original-target-20261020/inputs/game.exe')
    graph=guarded_graph(audit,inventory,ENTRY)
    from seed_guarded_cfg import X
    addresses=set(STATIC_ADDRESSES)
    found=set()
    for ins in graph['nodes'].values():
        for op in ins.operands:
            value=(op.imm & 0xffffffff) if op.type==X.X86_OP_IMM else (op.mem.disp & 0xffffffff) if op.type==X.X86_OP_MEM and not op.mem.base and not op.mem.index else None
            if value in addresses:found.add(value)
    require(found==addresses,'OwnerDraw local-static references differ')

    catalog=catalog_io.load(catalog_io.locate(repository),references=False)
    receivers=Receivers(repository,catalog,audit,inventory,graph,None)
    verify_graph(graph,receivers)

def optional_owner_draw_seed(raw,inventory,repository):
    if hashlib.sha256(raw.encode()).hexdigest()!=RAW_SHA256:
        return None
    try:
        verify_native(inventory,repository)
    except (ValueError,KeyError,TypeError,OSError):
        return None
    return CANDIDATE

CANDIDATE = r"""#define RA2_OWNERDRAW_COPY_LIST_SOURCE
#define PCX_CACHE_DECLARE_DRAWS
#define RA2_OWNERDRAW_BUTTON_FIELDS
#define RA2_OWNERDRAW_CHECK_FIELDS
#define RA2_OWNERDRAW_EDIT_FIELDS
#define RA2_WOL_DICTIONARY_KEY_SPECIALIZATIONS
#define RA2_WOL_DICTIONARY_REMOVE_SPECIALIZATION
#define RA2_WOL_ARRAY_LIST_ASSIGN
#define RA2_OWNERDRAW_RECORD_DTOR
#define RA2_WOL_DICTIONARY_SHRINK_ACCESSORS
#define RA2_WOL_DICTIONARY_WLIB_STATEMENTS
#define RA2_OWNERDRAW_WNDPROC_DICTIONARY
#define RA2_OWNERDRAW_CONTROL_DICTIONARY
// Header-static dynamic initializers of the unit at 0x005DE660: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005DE660-0x005DE841); the other
// drafts of this plan are emitted by these includes.
#define RA2_COMBO_DROP_UNIT
// Provisional typed OwnerDraw control-proc seed, original005EE460; no exact-body claim.
// origin lane: Scout1045 (build/class-work/Scout1045/w/y1_a2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// @ref _?hwndarray@?9??unknown_control_proc_5ee460@OwnerDraw@@YGJPAUHWND__@@IIJ@Z@4V?$WolArrayList@PAUHWND__@@@@A=00a73918
// @ref ?IME_Get_Default_Result_007380b0@@YIJXZ=007380b0
// @ref _?in_programmatic_reorder@?1??unknown_control_proc_5ee460@OwnerDraw@@YGJPAUHWND__@@IIJ@Z@4DA=00a7640c
// @ref _?max_update_rect@?1??unknown_control_proc_5ee460@OwnerDraw@@YGJPAUHWND__@@IIJ@Z@4UPOINT@@A=00a76404
// @ref _?min_update_rect@?1??unknown_control_proc_5ee460@OwnerDraw@@YGJPAUHWND__@@IIJ@Z@4UPOINT@@A=007e8d44
// @ref _?num_rect_updates@?1??unknown_control_proc_5ee460@OwnerDraw@@YGJPAUHWND__@@IIJ@Z@4HA=00a76400
// @ref _?was_in_focus@?1??unknown_control_proc_5ee460@OwnerDraw@@YGJPAUHWND__@@IIJ@Z@4_NA=007e8d40
// @ref __imp__WindowFromPoint@8=0079a394
/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Adapted from TibSun ownrdraw.cpp CtrlProc; RA2 native branches, types and extents are authoritative.
#include "shared/win32_api.hpp"
#include "shared/win32_gdi.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/wol_dictionary.hpp"
#include "shared/wol_array_list.hpp"
#include "shared/win_stub.hpp"
#include "shared/wide_string.hpp"
#include "shared/string_table.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_string.hpp"
/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/


// @ref _?ctrlmessages@?9??unknown_control_proc_5ee460@OwnerDraw@@YGJPAUHWND__@@IIJ@Z@4V?$WolDictionary@UUnknownPairKey@OwnerDraw@@_N@@A=00a73398
/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/


// From TibSun arraylist.h; release assertions omitted; exact-target callback ABI.
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"
void *__cdecl operator new(unsigned int,void *);
extern "C" double __cdecl floor(double);




LRESULT __fastcall IME_Get_Default_Result_007380b0();
void __fastcall Unknown_006FA0F0(char *dst, unsigned short const *src, int limit);
void __fastcall Unknown_Rect_Shrink_006F1B30(Rect *rect, int x, int y);
WIN32_IMPORT BOOL WINAPI GetUpdateRect(HWND, RECT *, BOOL);
WIN32_IMPORT BOOL WINAPI IntersectRect(RECT *, RECT const *, RECT const *);
WIN32_IMPORT HWND WINAPI GetWindow(HWND, UINT);
WIN32_IMPORT HWND WINAPI WindowFromPoint(POINT);
WIN32_IMPORT int WINAPI GetBkMode(HDC);
WIN32_IMPORT COLORREF WINAPI GetBkColor(HDC);
WIN32_IMPORT COLORREF WINAPI GetTextColor(HDC);
extern WolDictionary<HWND, WNDPROC> unknown_window_table_a73400;
extern WolDictionary<HWND, WNDPROC> unknown_window_proc_table_a73688;
// Pinned VC98 SDK WINDEF.H: DECLARE_HANDLE(HFONT).
typedef struct HFONT__ *HFONT;
extern "C" int __cdecl wcscmp(wchar_t const *,wchar_t const *);
namespace seed_005de980 { int __fastcall FUN_005de980(int *); }
namespace OwnerDraw {
WideString __fastcall unknown_5e2160(HWND window,int a);
WideString __fastcall unknown_5e21c0(HWND window,HWND item,int x,int y);
// unknown_window_table_a73400 is defined by this population's initializer005DEB90.
extern HWND unknown_dropdown_window_a763e4;
extern HWND unknown_dropdown_owner_a763e8;
LRESULT __stdcall unknown_button_proc_5f0320(HWND, UINT, WPARAM, LPARAM);
LRESULT __stdcall unknown_list_proc_5f6690(HWND, UINT, WPARAM, LPARAM);
BOOL __stdcall unknown_restore_child_5edd30(HWND, LPARAM);
class UnknownWindowList;
BOOL __stdcall unknown_add_window_5ff430(HWND, UnknownWindowList *);
bool __fastcall unknown_show_tooltip_5ee110(bool);
LRESULT __stdcall unknown_control_proc_5ee460(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	static POINT min_update_rect = {0xFFFFFF, 0xFFFFFF};
	static POINT max_update_rect;
	static int num_rect_updates;

	static bool was_in_focus = true;

	static char in_programmatic_reorder;

	if (message == 0x20) {
		return(1);
	}
    if (IME_Process_Message(window, message, wparam, lparam)) return IME_Get_Default_Result_007380b0();

	// Reference generic layouts: Dictionary56/pack8, ArrayList12. Existing globals remain canonical.
    WNDPROC specific_proc = 0;
	::unknown_window_table_a73400.Get_Value(window, specific_proc);

	WNDPROC original_proc = 0;
	::unknown_window_proc_table_a73688.Get_Value(window, original_proc);

	if (message == 0x105 && wparam == 0x9) {
		SendMessageA(MainWindow, 0x105, 0x9, lparam);
	}

	LRESULT result = 0;
	BOOL show_tooltip = 0;
    BOOL allow_proc = 1;
	int anim_state = 0;

	static WolDictionary<UnknownPairKey, bool> ctrlmessages((unsigned int (__fastcall *)(UnknownPairKey const &))seed_005de980::FUN_005de980);

	UnknownPairKey key;
	key.unknown_04 = (int)window;
	key.unknown_00 = message;

	RECT display_rect;
	Window_Rect_In_Main_Client(window, &display_rect);
	RECT window_rect;
	GetWindowRect(window, &window_rect);
	int offset_x = window_rect.left - display_rect.left;
	int offset_y = window_rect.top - display_rect.top;


	static WolArrayList<HWND> hwndarray;

	HWND owner;
	UnknownControlRecord *ownerdata;
	int state;

	if (hwndarray.Length() != 0) {
		HWND topwindow = 0;
		hwndarray.Get(topwindow, hwndarray.Length() - 1);

		BOOL allow = 0;
		BOOL forward = 0;
		if (GetParent(window) == 0) {
			allow = 1;
		}
		if (GetWindowLongA(window, -12) <= 0) {
			allow = 1;
		}

		HWND parent = window;
		while (parent != 0) {
			if (parent == topwindow) {
				allow = 1;
				break;
			}
			parent = GetParent(parent);
		}

		if (message < 0x200 || message > 0x209) {
			forward = 1;
		}
		if (message >= 0xa0 && message <= 0xa9) {
			forward = 0;
		}
		if (message >= 0x100 && message <= 0x108) {
			forward = 0;
		}
		if (message == 0x105 || message == 0x104 || message == 0x112 || message == 0x106) {
			forward = 1;
		}
		if (message == 0x49b || message == 0x113 || message == 0x4ad) {
			forward = 0;
		}
		if (!allow && !forward) {
			return(0);
		}
	}

	if (ctrlmessages.Contains(key)) {
		if (message != 0x111 && message != 0x104 && message != 0x105
			&& message != 0x112 && message != 0x106) {
			return(0);
		}
	}

	bool processing = true;
	ctrlmessages.Remove(key);
	ctrlmessages.Add(key, processing);

	RECT client_rect;
	GetClientRect(window, &client_rect);
	RECT disp_rect;
	Window_Rect_In_Main_Client(window, &disp_rect);

	bool in_focus = unknown_flag_a40d28;

	bool is_paint = false;
	if (message == 0xf) {
		is_paint = true;
		if (!in_focus) {
			ValidateRect(window, 0);
			ctrlmessages.Remove(key);
			return(0);
		}
	}

	if (in_focus == true && !was_in_focus) {
		InvalidateRect(MainWindow, 0, 0);
		in_focus = unknown_flag_a40d28;
	}
	was_in_focus = in_focus;

	RECT update_rect;
	if (is_paint) {
		++num_rect_updates;
		GetUpdateRect(window, &update_rect, 0);
		update_rect.left += disp_rect.left;
		update_rect.right += disp_rect.left;
	}

	UnknownControlRecord *data = Get_Record(window);

	if (message == 0x4a0) {
		result = (data && data->attached_0c != 0);
		goto cleanup;
	}

	if (message == 0x49f && data) {
		HDC hdc = (HDC)lparam;
		HGDIOBJ old = SelectObject(hdc, GetStockObject(0xd));
		(*(HFONT *)((char *)data + 0x1E8)) = (HFONT)old;
		SelectObject(hdc, old);
		(*(int *)((char *)data + 0x1EC)) = GetBkMode(hdc);
		(*(COLORREF *)((char *)data + 0x1F0)) = GetBkColor(hdc);
		(*(COLORREF *)((char *)data + 0x1F4)) = GetTextColor(hdc);
		result = 1;
		goto cleanup;
	}

	if (message == 0x49e && data) {
		HDC hdc = (HDC)lparam;
		SelectObject(hdc, (*(HFONT *)((char *)data + 0x1E8)));
		SetBkMode(hdc, (*(int *)((char *)data + 0x1EC)));
		SetBkColor(hdc, (*(COLORREF *)((char *)data + 0x1F0)));
		SetTextColor(hdc, (*(COLORREF *)((char *)data + 0x1F4)));
		result = 1;
		goto cleanup;
	}

	if (message == 0x49d && data) {
		{
			HWND linked = (HWND)data->attached_0c;
			result = data->unknown_20;
			data->unknown_20 = lparam;
			if (linked != 0) {
				UnknownControlRecord *linkeddata = 0;
				::unknown_control_table_a73640.Get_Pointer((unsigned int const &)data->attached_0c, &linkeddata);
				if (linkeddata != 0) {
					linkeddata->unknown_20 = lparam;
				}
			}
		}

		goto call_custom_proc;
	}

	if (message == 0x4a9) {
		HWND topwindow = 0;
		hwndarray.Get(topwindow, hwndarray.Length() - 1);
		result = (LRESULT)topwindow;

		HWND target = window;
		if (wparam) {
			target = (HWND)wparam;
		}

		int scan = 0;
		for (int index = 0; index < hwndarray.Length(); index++) {
			hwndarray.Get(topwindow, scan);
			if (topwindow == target) {
				hwndarray.Remove(scan);
			} else {
				scan++;
			}
		}

		if (lparam) {
			hwndarray.Add_Tail(target);
			in_programmatic_reorder = 1;
			SetWindowPos(target, 0, 0, 0, 0, 0, 0x1 | 0x2);
			in_programmatic_reorder = 0;
		}
		goto cleanup;
	}

	if (hwndarray.Length() != 0) {

		if (message == 0x46) {
			if (in_programmatic_reorder != 1) {
				HWND found = 0;
				for (int index = 0; index < hwndarray.Length(); index++) {
					hwndarray.Get(found, index);
					if (found == window) {
						int *wp = (int *)lparam;
						if (index == hwndarray.Length() - 1 && wp[1] != 0 && (wp[6] & 0x4) == 0) {
							wp[1] = 0;
							InvalidateRect(window, 0, 0);
							result = 0;
						} else {
							wp[6] |= 0x200 | 0x4;
							InvalidateRect(window, 0, 0);
							result = 0;
						}
						goto cleanup;
					}
				}
			}
			goto call_custom_proc;
		}

		if (message == 0x2) {
			HWND found = 0;
			int scan = 0;
			for (int index = 0; index < hwndarray.Length(); index++) {
				hwndarray.Get(found, scan);
				if (found == window) {
					hwndarray.Remove(scan);
				} else {
					scan++;
				}
			}
		}
	}

	if (window == (HWND)unknown_tooltip_a737f0.unknown_11c
		&& (message == 0x82 || message == 0x18 || message == 0x8)
		&& unknown_tooltip_a737f0.shown_114) {
		unknown_5ee3b0();
	}

	if (message == 0x14) {
		result = 1;
		goto cleanup;
	} else if (message == 0x7) {
		if (specific_proc == unknown_button_proc_5f0320 || specific_proc == unknown_list_proc_5f6690) {
			SetFocus((HWND)wparam);
		}
        if (data && !data->unknown_38) { IME_Reset(); data->unknown_38 = 1; }
    } else if (message == 8) {
        data->unknown_38 = 0;
	} else if (message == 0x18) {
		if (wparam == 0 && data) {
			(*(int *)((char *)data + 0x1FC)) = 0;
		}
	}
	if (data) {
		if (message == 0x49c) {
			result = (LRESULT)data->image_14;
			data->image_14 = (Surface *)lparam;
			goto cleanup;
		} else if (message == 0x4aa) {
			result = (LRESULT)(*(Surface **)((char *)data + 0x18));
			(*(Surface **)((char *)data + 0x18)) = (Surface *)lparam;
			goto cleanup;
		} else if (message == 0x49a) {
			result = (*(int *)((char *)data + 0x24));
			(*(int *)((char *)data + 0x24)) = lparam;
			goto cleanup;
		}
		if ((*(int *)((char *)data + 0x24)) && (message == 0x113 || (message >= 0x200 && message <= 0x209))) {
			RECT corner;
			GetWindowRect(window, &corner);
			WindowFromPoint(*(POINT *)&corner);
		}
	}
    if (message == 0xC) allow_proc = false;
    else if (message == 0xD) { allow_proc = false; result = 0; }
    else if (message == 0x189) { allow_proc = false; result = 0; }
    else if (message == 0x143 || message == 0x14C || message == 0x158 || message == 0x14D || message == 0x14A || message == 0x148 || message == 0x180 || message == 0x18F || message == 0x1A2 || message == 0x18C || message == 0x181) { allow_proc = false; result = -1; }
    if (data) {
        if (message == 0x4CE) { allow_proc = false; result = data->unknown_2c == 0; }
        else if (message == 0x4B5) {
            char *destination = (char *)lparam;
            if (destination) {
                *destination = 0;
                if (data->text_28) {
                    Unknown_006FA0F0(destination, data->text_28, wparam);
                    if (wparam > 0) destination[wparam - 1] = 0;
                }
            }
        } else if (message == 0x4B3) {
            wchar_t *destination = (wchar_t *)lparam;
            if (destination) {
                *destination = 0;
                if (data->text_28) {
                    wcsncpy(destination, data->text_28, wparam);
                    if ((int)wparam > 0) destination[wparam - 1] = 0;
                }
            }
        } else if (message == 0x4EB) {
            *(int *)((char *)data + 0x50) = lparam;
        } else if (message == 0x4B4) {
            data->Set_Text_600560((char const *)lparam);
            data->unknown_2c = 1;
        } else if (message == 0x4D1) {
            result = *(int *)((char *)data + 0x30);
            *(int *)((char *)data + 0x30) = wparam;
            allow_proc = false;
        } else if (message == 0x4B2) {
            wchar_t const *text = data->text_28;
            wchar_t const *new_text = (wchar_t const *)lparam;
            bool either = text != 0 || new_text != 0;
            bool added = text == 0 && new_text != 0;
            bool removed = text != 0 && new_text == 0;
            bool changed = added || removed || (either && wcscmp(text, new_text) != 0);
            data->Set_Wide_Text_600510((wchar_t const *)lparam);
            data->unknown_2c = 0;
            if (changed && data->unknown_70 == 1 && data->unknown_94[0xA8 - 0x94]) {
                KillTimer(window, 0);
                data->unknown_94[0xA8 - 0x94] = 0;
                SendMessageA(window, 0x4EE, 0, 0);
            }
        }
    }
    if (message == 0x200) {
        HWND parent = GetParent(window);
        HWND status = GetDlgItem(parent, 0x695);
        if (status) {
            WideString text;
            unsigned int mx = (unsigned short)lparam;
            unsigned int my = (unsigned short)((unsigned int)lparam >> 16);
            text = unknown_5e21c0(parent, window, mx, my);
            if (wcscmp(text.Get(), (wchar_t const *)L"") == 0) {
                text = unknown_5e2160(parent, (int)window);
                if (wcscmp(text.Get(), (wchar_t const *)L"") == 0) {
                    char const *label = unknown_5e22b0((unsigned int)parent, window);
                    WideString name(label ? StringTable::Load_String(label, 0, "D:\\Ra2\\ownrdraw.cpp", 0x76E) : (wchar_t const *)L"");
                    text = name;
                }
            }
            SendMessageA(status, 0x4B2, 0, (LPARAM)text.unknown_text_770430());
        }
    }

	if (is_paint && data) {

		if (data->unknown_20) {
			ValidateRect(window, 0);
			result = CallWindowProcA(specific_proc, window, 0x4a5, wparam, lparam);
		} else {
			Rect sibling_rect;
			RECT intersect;

			{
				RECT tooltip_rect;
				tooltip_rect.left = unknown_tooltip_a737f0.rect_00.X;
				tooltip_rect.right = unknown_tooltip_a737f0.rect_00.Width + unknown_tooltip_a737f0.rect_00.X + 1;
				tooltip_rect.top = unknown_tooltip_a737f0.rect_00.Y;
				tooltip_rect.bottom = unknown_tooltip_a737f0.rect_00.Height + unknown_tooltip_a737f0.rect_00.Y + 1;
				if (num_rect_updates == 1) {
					RECT intersect;
					if (IntersectRect(&intersect, &disp_rect, &tooltip_rect)) {
						if (unknown_tooltip_a737f0.shown_114 && unknown_tooltip_a737f0.drawn_118 != 1 && unknown_tooltip_a737f0.surface_10 != 0) {
							Rect drect = unknown_tooltip_a737f0.rect_00;
							Rect srect(0, 0, unknown_tooltip_a737f0.rect_00.Width, unknown_tooltip_a737f0.rect_00.Height);
							unknown_surface_839950->Copy_From(drect, *unknown_tooltip_a737f0.surface_10, srect, false, true);
							unknown_tooltip_a737f0.drawn_118 = true;
							show_tooltip = true;
						}
					}
				}
			}

			if (min_update_rect.x >= disp_rect.left) {
				min_update_rect.x = disp_rect.left;
			}
			if (min_update_rect.y >= disp_rect.top) {
				min_update_rect.y = disp_rect.top;
			}
			if (max_update_rect.x <= disp_rect.right) {
				max_update_rect.x = disp_rect.right;
			}
			if (max_update_rect.y <= disp_rect.bottom) {
				max_update_rect.y = disp_rect.bottom;
			}

			owner = window;
			while (owner != 0) {
				if (GetWindowLongA(owner, 0x4) != 0) {
					break;
				}
				owner = GetParent(owner);
			}

			ownerdata = 0;
			if (owner != 0) {
				unknown_control_table_a73640.Get_Pointer((unsigned int const &)owner, &ownerdata);
			}

			if (owner == window) {
				if ((*(int *)((char *)ownerdata + 0x1FC)) < 1) {
					state = 1;
					anim_state = 1;
					goto call_proc_paint;
				}
				state = (*(int *)((char *)ownerdata + 0x1FC));
			} else {
				if (ownerdata == 0) {
					state = anim_state;
					ValidateRect(window, 0);
					result = 1;
					goto finalize_paint;
				}
				state = (*(int *)((char *)ownerdata + 0x1FC));
			}

			anim_state = state;
			if (state < 1) {
				ValidateRect(window, 0);
				result = 1;
			} else {

			call_proc_paint:
				result = CallWindowProcA(specific_proc, window, message, wparam, lparam);
				if (ownerdata != 0) {
					(*(int *)((char *)ownerdata + 0x1FC)) = state;
				}

				{
					WolArrayList<HWND> children;
					EnumChildWindows(window, (WNDENUMPROC)unknown_add_window_5ff430, (LPARAM)&children);

					HWND combo_owner = 0;
					HWND child = 0;
					for (int index = 0; index < children.Length(); index++) {
						children.Get(child, index);

						WNDPROC childproc = 0;
						::unknown_window_proc_table_a73688.Get_Value(child, childproc);

						if (childproc != (WNDPROC)unknown_combo_drop_proc_5ead20) {
							InvalidateRect(child, 0, 0);
							UpdateWindow(child);
						} else {
							combo_owner = child;
						}
					}

					if (combo_owner != 0) {
						InvalidateRect(combo_owner, 0, 0);
						UpdateWindow(combo_owner);
					} else if (unknown_dropdown_window_a763e4 != 0) {
						if (unknown_dropdown_owner_a763e8 == owner) {
							Rect drop_rect;
							Window_Rect_In_Main_Client(unknown_dropdown_window_a763e4, (RECT *)&drop_rect);
							RECT intersect;
							if (IntersectRect(&intersect, &disp_rect, (const RECT *)&drop_rect)) {
								InvalidateRect(unknown_dropdown_window_a763e4, 0, 0);
								UpdateWindow(unknown_dropdown_window_a763e4);
							}
						}
					}
				}

				state = anim_state;
			}

		finalize_paint:
			if (state > 0) {
				HWND sibling = owner;
				while (window == owner || num_rect_updates == 1) {
					if (sibling == 0) {
						break;
					}
					sibling = GetWindow(sibling, 0x3);
					if (sibling == 0) {
						break;
					}
					if (GetWindowLongA(sibling, 0x4)) {
						Window_Rect_In_Main_Client(sibling, (RECT *)&sibling_rect);
						if (IntersectRect(&intersect, &disp_rect, (const RECT *)&sibling_rect)) {
							InvalidateRect(sibling, 0, 0);
							UpdateWindow(sibling);
							break;
						}
					}
				}
			}
		}
		goto after_proc;
	} else {
        if (allow_proc) goto call_custom_proc;
        goto after_proc;
	}

call_custom_proc:
	if (specific_proc != 0) {
		result = CallWindowProcA(specific_proc, window, message, wparam, lparam);
	}

after_proc:
	if (message == 0x82) {
		HWND destroyed = window;
		UnknownControlRecord * destroydata;
		unsigned int destroyed_key=(unsigned int)destroyed;
		if (::unknown_control_table_a73640.Get_Pointer(destroyed_key, &destroydata)) {
			if (destroydata != 0 && destroydata->surface_10 != 0) {
				delete destroydata->surface_10;
				destroydata->surface_10 = 0;
				unknown_surface_count_a763d8--;
			}
		}
		unknown_restore_child_5edd30(window, 0);
	}

cleanup:
    ctrlmessages.Remove(key);
    if (data && is_paint) {
        if (GetWindowLongA(window, 4) && num_rect_updates > 1) *(int *)((char *)data + 0x1FC) = 2;
        if (--num_rect_updates == 0) {
            if (!data->unknown_20 && anim_state >= 1) {
                Rect source(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
                Rect destination(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
                if (*(int *)((char *)data + 0x1FC) == 1) {
                    *(int *)((char *)data + 0x1FC) = 2;
                    if (GetWindowLongA(window, 4) && unknown_show_dialog_5e61b0(window)) *(int *)((char *)data + 0x1FC) = 3;
                    Rect whole(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
                    destination = Offset_Rect(whole, offset_x, offset_y);
                    unknown_surface_839950->Lock();
                    unknown_surface_839958->Lock();
                    unknown_surface_839950->Copy_From(destination, *unknown_surface_839958, whole, false, true);
                    unknown_surface_839958->Unlock();
                    unknown_surface_839950->Unlock();
                    WolArrayList<HWND> children;
                    EnumChildWindows(window, (WNDENUMPROC)unknown_add_window_5ff430, (LPARAM)&children);
                    HWND child = 0;
                    for (int index = 0; index < children.Length(); index++) {
                        children.Get(child, index);
                        SendMessageA(child, 0x4AF, 0, 0);
                    }
                } else {
                    destination = Offset_Rect(source, offset_x, offset_y);
                    char class_name[128];
                    GetClassNameA(window, class_name, sizeof(class_name));
                    if (strcmp(class_name, "ComboBox") == 0) {
                        Unknown_Rect_Shrink_006F1B30(&source, -1, -1);
                        Unknown_Rect_Shrink_006F1B30(&destination, -1, -1);
                    }
                    unknown_surface_839950->Lock();
                    unknown_surface_839958->Lock();
                    unknown_surface_839950->Copy_From(destination, *unknown_surface_839958, source, false, true);
                    unknown_surface_839958->Unlock();
                    unknown_surface_839950->Unlock();
                }
            }
            min_update_rect.x = 0xFFFFFF;
            min_update_rect.y = 0xFFFFFF;
            max_update_rect.x = 0;
            max_update_rect.y = 0;
        }
    }
    if (show_tooltip) unknown_show_tooltip_5ee110(true);
    if (message == 0x110) {
        result = 0;
    }
    return result;
}

}
"""
