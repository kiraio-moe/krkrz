
#ifndef __DRAWER_H__
#define __DRAWER_H__

#include <windows.h>
#include <vfw.h>
#include "tjsCommHead.h"

class tTVPPassThroughDrawDevice;
struct tTVPRect;
//---------------------------------------------------------------------------
//! @brief	PassThrough で用いる描画方法用インターフェース
//---------------------------------------------------------------------------
class tTVPDrawer
{
protected:
	tTVPPassThroughDrawDevice * Device;
	tjs_int DestLeft;
	tjs_int DestTop;
	tjs_int DestWidth;
	tjs_int DestHeight;
	tjs_int SrcWidth;
	tjs_int SrcHeight;
	HWND TargetWindow;
	HDRAWDIB DrawDibHandle;
	bool DrawUpdateRectangle;
public:
	tTVPDrawer(tTVPPassThroughDrawDevice * device)
	{
		Device = device;
		SrcWidth = 0;
		SrcHeight = 0;
		DestLeft = DestTop = DestWidth = DestHeight = 0;
		TargetWindow = NULL;
		DrawDibHandle = NULL;
		DrawUpdateRectangle = NULL;
	} 
	virtual ~tTVPDrawer()
	{
		if(DrawDibHandle) DrawDibClose(DrawDibHandle), DrawDibHandle = NULL;
	}
	virtual ttstr GetName() = 0;

	virtual bool SetDestPos(tjs_int left, tjs_int top)
		{ DestLeft = left; DestTop = top; return true; }
	virtual bool SetDestSize(tjs_int width, tjs_int height)
		{ DestWidth = width; DestHeight = height; return true; }
	void GetDestSize(tjs_int &width, tjs_int &height) const
		{ width = DestWidth; height = DestHeight; }
	virtual bool NotifyLayerResize(tjs_int w, tjs_int h)
		{ SrcWidth = w; SrcHeight = h; return true; }
	void GetSrcSize(tjs_int &w, tjs_int &h) const
		{ w = SrcWidth; h = SrcHeight; }
	virtual bool SetDestSizeAndNotifyLayerResize(tjs_int width, tjs_int height, tjs_int w, tjs_int h)
	{
		if(!SetDestSize(width, height)) return false;
		if(!NotifyLayerResize(w, h)) return false;
		return true;
	}
	virtual void SetTargetWindow(HWND wnd)
	{
		if(DrawDibHandle) DrawDibClose(DrawDibHandle), DrawDibHandle = NULL;
		TargetWindow = wnd;
		DrawDibHandle = DrawDibOpen();
	}
	virtual void StartBitmapCompletion() = 0;
	virtual void NotifyBitmapCompleted(tjs_int x, tjs_int y,
		const void * bits, const BITMAPINFO * bitmapinfo,
		const tTVPRect &cliprect) = 0;
	virtual void EndBitmapCompletion() = 0;
	virtual void Show() {;}
	virtual void SetShowUpdateRect(bool b)  { DrawUpdateRectangle = b; }
	virtual int GetInterpolationCapability() { return 3; }
		// bit 0 for point-on-point, bit 1 for bilinear interpolation

	virtual void InitTimings() {;} // notifies begining of benchmark
	virtual void ReportTimings() {;} // notifies end of benchmark
};
//---------------------------------------------------------------------------

#endif // __DRAWER_H__

