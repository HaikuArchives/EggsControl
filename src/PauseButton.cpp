#include "PauseButton.h"

PauseButton::PauseButton(BRect frame, const char *name, BView *owner, BMessage *message,
		uint32 behaviour,
		uint32 resizingMode,
		uint32 flags) {
	
	on=new BPicture(); // PAUSE BUTTON
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
	owner->FillRect(BRect(4,4,8,16),B_SOLID_HIGH);
	owner->FillRect(BRect(12,4,16,16),B_SOLID_HIGH);
	on=owner->EndPicture();
	off=new BPicture(); // PAUSE BUTTON
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
	owner->FillRect(BRect(4,4,8,16),B_SOLID_HIGH);
	owner->FillRect(BRect(12,4,16,16),B_SOLID_HIGH);
	off=owner->EndPicture();
	// create button
	ownerV=owner;
	button=new BPictureButton(frame, name, off, on, message, behaviour, resizingMode, flags);
	owner->AddChild(button);
}

PauseButton::~PauseButton() {
	ownerV->RemoveChild(button);
	delete button;
	delete on;
	delete off;
}