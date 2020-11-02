int c_DrawControlFrame (HWND _handle, HDC _hdc, int _control_id, COLORREF _frame_color)
{
	RECT 	rect_dlg;
	RECT 	rect_window;
	HDC 	hdc;
	RECT	rect_size;
	
	GetWindowRect((GetDlgItem(_handle,_control_id)),&rect_dlg);
	
	//---get relative position of the control to our window---
	GetWindowRect(_handle,&rect_window);
	
	rect_dlg.left -= rect_window.left;
	rect_dlg.top  -= rect_window.top;
	
	//---get size of our control---
	GetClientRect((GetDlgItem(_handle,_control_id)),&rect_size);
	
	rect_dlg.bottom = rect_size.bottom;
	rect_dlg.right = rect_size.right;
	
	//---add position to dlg rect struct---
	rect_dlg.bottom += rect_dlg.top;
	rect_dlg.right += rect_dlg.left;
	
	//---make rect bigger---
	--rect_dlg.left;
	--rect_dlg.top;
	++rect_dlg.right;
	++rect_dlg.bottom;
	
	//---draw frame---
	FrameRect(_hdc,&rect_dlg,(CreateSolidBrush(_frame_color)));
	
	return TRUE;	
}


int c_DrawWindowColors (HWND _handle,HDC _hdc,COLORREF _background_color,COLORREF _frame_color)
{	
	RECT rect;
	HDC hdc;
	
	GetClientRect(_handle,&rect);
	FillRect(_hdc,&rect,(CreateSolidBrush(_background_color)));
	FrameRect(_hdc,&rect,(CreateSolidBrush(_frame_color)));
	
	return TRUE;
}	


int c_DrawControlColor (WPARAM _wparam,COLORREF _background_color,COLORREF _text_color)
{	
	//---set custom colors of the EDIT boxes---
	SetTextColor((HDC)_wparam,_text_color);				//TextColor
	SetBkMode((HDC)_wparam,TRANSPARENT);				//Background of Text or SetBkColor
	return (LONG)((HBRUSH)CreateSolidBrush(_background_color));	//BackgroundColor
}


int c_MakeOwnerDraw (HWND _dlghandle,int _id)
{
	SendMessage((GetDlgItem(_dlghandle,_id)),BM_SETSTYLE,BS_OWNERDRAW,TRUE);
	return TRUE;
}	


int c_DrawButtonColor (HWND _dialoghandle,LPARAM _lparam,COLORREF _background_color,COLORREF _text_color,COLORREF _frame_color)
{	
	char sBtnText[128];
	DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT*)_lparam;
	
	if (dis->CtlType==ODT_BUTTON)
	{
		//---Button Colors---
		SelectObject(dis->hDC,(CreatePen(PS_INSIDEFRAME,1,_frame_color)));	//framecolor
		SelectObject(dis->hDC,(CreateSolidBrush(_background_color)));		//Background Color
		
		//---draw frame---
		RoundRect(dis->hDC,dis->rcItem.left,dis->rcItem.top,dis->rcItem.right,dis->rcItem.bottom,5,5);
		
		if (dis->itemState & ODS_SELECTED)
		{
			OffsetRect(&dis->rcItem,1,1);
		}
		
		//---write the text--
		GetDlgItemText(_dialoghandle,dis->CtlID,sBtnText,sizeof sBtnText);
		SetBkMode(dis->hDC,TRANSPARENT);
		SetTextColor(dis->hDC,_text_color);
		DrawText(dis->hDC,sBtnText,-1,&dis->rcItem,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		
		//---move text when button pressed---
		if (dis->itemState & ODS_SELECTED)
		{
			OffsetRect(&dis->rcItem,-1,-1);
		}
	}
	
	return TRUE;
}

