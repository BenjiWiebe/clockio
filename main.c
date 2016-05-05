#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <lctp.h>
#include "resource.h"
#include "datafile.h"
#include "find_datafiles.h"

#ifdef _unused
#undef _unused
#endif // _unused
#define _unused __attribute__((unused))

HWND dlg;
HINSTANCE hInst;
HWND listbox, buttonin, buttonout, buttonview, status;
int currentemployee = -1;

char **names;

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM _unused lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			dlg = hwndDlg;
			listbox = GetDlgItem(dlg, IDC_LIST1);
			buttonin = GetDlgItem(dlg, INBUTTON);
			buttonout = GetDlgItem(dlg, OUTBUTTON);
			buttonview = GetDlgItem(dlg, VIEWHOURS);
			status = CreateWindowEx(0, STATUSCLASSNAME, (PCTSTR) NULL, WS_CHILD|WS_VISIBLE, 0, 0, 0, 0, dlg, NULL, hInst, NULL);
			for(int i = 0; names[i] != NULL; i++)
			{
				int pos = (int)SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)names[i]);
				SendMessage(listbox, LB_SETITEMDATA, pos, (LPARAM)i);
			}
			EnableWindow(buttonin, FALSE);
			EnableWindow(buttonout, FALSE);
			EnableWindow(buttonview, FALSE);
			return TRUE;

		case WM_CLOSE:
			EndDialog(hwndDlg, 0);
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case INBUTTON:
					if(write_time_entry(names[currentemployee], "IN ") < 0)
						MessageBox(hwndDlg, strerror(errno), "Error", MB_OK|MB_ICONERROR);
					return TRUE;

				case OUTBUTTON:
					if(write_time_entry(names[currentemployee], "OUT") < 0)
						MessageBox(hwndDlg, strerror(errno), "Error", MB_OK|MB_ICONERROR);
					return TRUE;

				case VIEWHOURS:
					MessageBox(hwndDlg, "Sorry, this feature has not been implemented yet.", "Not Implemented", MB_OK|MB_ICONEXCLAMATION);
					return TRUE;

				case IDC_LIST1:
					switch(HIWORD(wParam))
					{
						case LBN_SELCHANGE:
							currentemployee = SendMessage(listbox, LB_GETITEMDATA, SendMessage(listbox, LB_GETCURSEL, 0, 0), 0);
							if(currentemployee < 0)
								return TRUE;
							EnableWindow(buttonin, TRUE);
							EnableWindow(buttonout, TRUE);
							return TRUE;
					}
			}
	}

	return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE _unused hPrevInstance, LPSTR _unused lpCmdLine, int _unused nShowCmd)
{
	hInst = hInstance;
	INITCOMMONCONTROLSEX ic;
	ic.dwICC = ICC_BAR_CLASSES|ICC_STANDARD_CLASSES;
	ic.dwSize = sizeof ic;
	if(!InitCommonControlsEx(&ic))
		return -1;
	names = find_datafiles("C:\\ProgramData\\Clockio\\data\\");
	if(names == NULL)
	{
		MessageBox(NULL, "Could not find employee data files!", "Error", MB_OK|MB_ICONERROR);
		return 1;
	}
	// The user interface is a modal dialog box
	INT_PTR p = DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DialogProc);
	return p;
}
