#include "PlayButton.h"

BPoint polygon[3]={BPoint(4,4), BPoint(4,16), BPoint(16,10)};

PlayButton::PlayButton(BRect frame, const char *name, BView *owner, BMessage *message,
		uint32 behaviour,
		uint32 resizingMode,
		uint32 flags) {
	
	on=new BPicture(); // STOP BUTTON
	owner->BeginPicture(on);	
	owner->SetHighColor(216,216,216,255);
	owner->FillRect(frame,B_SOLID_HIGH);
	owner->SetHighColor(116,116,116,255);
	owner->StrokeLine(BPoint(1,0),BPoint(19,0));
	owner->StrokeLine(BPoint(0,1),BPoint(0,19));
	owner->SetHighColor(252,252,252,255);
	owner->StrokeLine(BPoint(2,20),BPoint(19,20));
	owner->StrokeLine(BPoint(20,19),BPoint(20,2));
	owner->SetHighColor(214,32,32,255);
	owner->FillPolygon(new BPolygon(polygon, 3),B_SOLID_HIGH);
	on=owner->EndPicture();
	off=new BPicture(); // STOP BUTTON
	owner->BeginPicture(off);
	owner->SetHighColor(216,216,216,255);
	owner->FillRect(frame,B_SOLID_HIGH);
	owner->SetHighColor(252,252,252,255);
	owner->StrokeLine(BPoint(1,0),BPoint(19,0));
	owner->StrokeLine(BPoint(0,1),BPoint(0,19));
	owner->SetHighColor(116,116,116,255);
	owner->StrokeLine(BPoint(2,20),BPoint(19,20));
	owner->StrokeLine(BPoint(20,19),BPoint(20,2));
	owner->SetHighColor(0,0,0,255);
	owner->FillPolygon(new BPolygon(polygon, 3),B_SOLID_HIGH);
	off=owner->EndPicture();
	// create button
	ownerV=owner;
	button=new BPictureButton(frame, name, off, on, message, behaviour, resizingMode, flags);
	owner->AddChild(button);
}

PlayButton::~PlayButton() {
	ownerV->RemoveChild(button);
	delete button;
	delete on;
	delete off;
}