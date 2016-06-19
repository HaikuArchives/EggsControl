/* --------------- Please read the information in "DrawButton.h"! --------------- */

#include "DrawButton.h"

void	DrawButton(BView *view, BRect frame, bool pressed) {
	char	c_border = 96;
	char	c_hg, c_lefttop, c_rightbottom1, c_rightbottom2;

	if (!pressed)
		{ c_hg = 232; c_lefttop = 255; c_rightbottom1 = 216; c_rightbottom2 = 152; }
	else
		{ c_hg = 23; c_lefttop = 0; c_rightbottom1 = 39; c_rightbottom2 = 103; }
		
	if (frame == BRect(0,0,0,0)) frame = view->Bounds();

	view->SetHighColor(c_hg, c_hg, c_hg);
	view->FillRoundRect(frame, 1.5, 1.5);

	view->SetHighColor(c_border, c_border, c_border);
	view->StrokeRoundRect(frame, 1.5, 1.5);
	view->SetHighColor(c_lefttop, c_lefttop, c_lefttop);
	view->StrokeLine( BPoint(frame.right - 2, frame.top + 2), BPoint(frame.left + 2, frame.top + 2) );
	view->StrokeLine( BPoint(frame.left + 2, frame.bottom - 2) );
	view->StrokeLine( BPoint(frame.right - 3, frame.top + 3), BPoint(frame.left + 3, frame.top + 3) );
	view->StrokeLine( BPoint(frame.left + 3, frame.bottom - 3) );
	
	view->SetHighColor(c_rightbottom1, c_rightbottom1, c_rightbottom1);
	view->StrokeLine( BPoint(frame.left + 2, frame.bottom - 2), BPoint(frame.right - 2, frame.bottom - 2) );
	view->StrokeLine( BPoint(frame.right - 2, frame.top + 2) );

	view->SetHighColor(c_rightbottom2, c_rightbottom2, c_rightbottom2);
	view->StrokeLine( BPoint(frame.left + 2, frame.bottom - 1), BPoint(frame.right - 1, frame.bottom - 1) );
	view->StrokeLine( BPoint(frame.right - 1, frame.top + 2) );
	
};
