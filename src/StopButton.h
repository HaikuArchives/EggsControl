#ifndef _STOPBUTTON_H_
#define _STOPBUTTON_H_

#include <PictureButton.h>
#include <Rect.h>
#include <Picture.h>

class StopButton {
public:
	StopButton(BRect frame, const char *name, BView *owner, BMessage *message,
		uint32 behaviour = B_ONE_STATE_BUTTON,
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		uint32 flags = B_WILL_DRAW | B_NAVIGABLE);
	~StopButton();
private: 
	BPicture		*on, *off;
	BPictureButton	*button;
	BView			*ownerV;
};

#endif