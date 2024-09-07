// xll_demo.cpp - Demonstrations
#include "bondxll.h"
#include "xll24/include/macrofun.h"
#include "xll24/include/type.h"

void DoEvents(int ms);

using namespace xll;

//AddIn xai_instrument(Macro(L"type_instrument", L"TYPE.INSTRUMENT"));
AddIn xai_type_instrument(Macro(L"type_instrument", L"TI"));
int WINAPI type_instrument()
{
#pragma XLLEXPORT
	try {
		int pause = 100, wait = 1000;
		Select sel;

		Excel(xlcColumnWidth, 3);
		sel.Offset(1, 1);

		Excel(xlcColumnWidth, 8.75);
		Type("Ctrl-Shift-A", pause, wait);
		sel.Down();
		Type("=\\TMX.INSTRUMENT", pause, 500);
		Excel(xlSet, sel.selection, Nil);
		Type("=\\TMX.INSTRUMENT(time, cash)", 0, wait);
		//DoEvents(10000);
		
		sel.Offset(-1, 1);
		Excel(xlcColumnWidth, 3);

		sel.Offset(0, 1);
		Excel(xlcColumnWidth, 8.75);
		Type("Ctrl-Shift-B", pause, wait);
		
		sel.Down();
		Type("=\\TMX.INSTRUMENT", pause, wait);
		Excel(xlcRun, L"XLL.PASTEB");
		DoEvents(wait);	
		sel.Down();
		Type("=TMX.INSTRUMENT(R[-1]C[0])", pause, wait);
		sel.Offset(-1, 0);
		DoEvents(wait);

		sel.Offset(-1, 3);
		Excel(xlcColumnWidth, 3);
		sel.Offset(0, 1);

		Excel(xlcColumnWidth, 8.75);
		Type("Ctrl-Shift-C", pause, wait);
		sel.Down();
		Type("=\\TMX.INSTRUMENT", pause, wait);
		DoEvents(wait);
		Excel(xlcRun, L"XLL.PASTEC");
		sel.Offset(3, 0);
		Type("=TMX.INSTRUMENT(R[-3]C[0])", pause, wait);
		//Excel(xlcSelectSpecial, 6); // current array
		//Excel(xlcApplyStyle, L"Output");

		sel.Offset(-4, 3);
		Excel(xlcColumnWidth, 3);
		sel.Offset(0, 1);

		Excel(xlcColumnWidth, 8.75);
		Type("Ctrl-Shift-D", pause, wait);
		sel.Down();
		Type("=\\TMX.INSTRUMENT", pause, wait);
		DoEvents(wait);
		Excel(xlcRun, L"XLL.PASTED");
		sel.Offset(3, 1);
		Type("=TMX.INSTRUMENT(R[-3]C[0])", pause, wait);
		sel.Offset(0, 0, 2, 3);
		Excel(xlcApplyStyle, L"Output");
		sel.Offset(-2, 3, 1, 1);
		Type("Change an input cell...", pause, wait);
		sel.Offset(0, -1);
		Type("=4", pause, wait);
		sel.Offset(2, 1);
		Type("...see the output change.", pause, wait);

		sel.Offset(2, -4);
		Type("=\\TMX.INSTRUMENT(time, cash)", pause, wait);
		sel.Offset(0, 1);
		Type("=TMX.INSTRUMENT(R[0]C[-1])", pause, wait);
		sel.Offset(0, -1);
		//Excel(xlcSelectSpecial, 6, sel.selection); // current array
		//Excel(xlcApplyStyle, L"Output");
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
}