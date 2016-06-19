#include "SoundPanel.h"
#include <Button.h>
#include <Entry.h>
#include <StringView.h>
#include <Path.h>
#include <FileGameSound.h>
#include <OS.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <Window.h>
#include <Slider.h>
#include <Picture.h>
#include <PictureButton.h>

static int32 buf;



SoundPanel::SoundPanel(BEntry *entry) : BView(BRect(0,0,150,70),"SoundPanel", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW){
	BPath path;
	BMessage	*msg;
	
	kentry=entry;
	kentry->GetPath(&path);
	
	SetViewColor((rgb_color){216,216,216,255});
//	msg=new BMessage(SP_PLAY);
//	msg->AddPointer("from", this);
//	playB=new BButton(BRect(5,36,45,40), "playB", "PLAY", msg);
//	msg=new BMessage(SP_STOP);
//	msg->AddPointer("from", this);
//	stopB=new BButton(BRect(50,36,95,40), "stopB", "STOP", msg);
//	msg=new BMessage(SP_PAUSE);
//	msg->AddPointer("from", this);
//	pauseB=new BButton(BRect(100,36,145,40), "pauseB", "PAUSE", msg);
	msg=new BMessage(SP_LOOP);
	msg->AddPointer("from", this);
	loopCB=new BCheckBox(BRect(9,38,54,58),"loopCB", "loop", msg);

	cmenu=new BPopUpMenu("cmenu", false, false);
	msg=new BMessage(SP_DELETE);
	msg->AddPointer("from", this);
	BMenuItem	*mi=new BMenuItem("remove sample", msg);
	cmenu->AddItem(mi);
	cmenu->SetTargetForItems(this->Window());

	msg=new BMessage(SP_UPDATEVOLUME);
	msg->AddPointer("from", this);
	volumeSL=new BSlider(BRect(1, 3,Bounds().right-1,12), "volumeSL","", msg, 0,100);
	BString s=path.Leaf();
	while (StringWidth(s.String())<volumeSL->Bounds().Width()) s.Append(" ").Prepend(" ");
	volumeSL->SetLabel(s.String());
	msg=new BMessage(SP_UPDATEVOLUME);
	msg->AddPointer("from", this);
	volumeSL->SetModificationMessage(msg);
	AddChild(volumeSL);
//	AddChild(playB);
//	AddChild(stopB);
//	AddChild(pauseB);
	AddChild(loopCB);
//	BStringView *sv=new BStringView(BRect(1,Bounds().top+1, Bounds().right-1,Bounds().top+16),"StringView", path.Leaf() );
//	sv->SetAlignment(B_ALIGN_CENTER);
	//AddChild(sv);
	buffersize=1024;
	bufferlevel=0;
	firstfree=0;

}

void SoundPanel::AllAttached() {
	volumeSL->SetLimitLabels("","");
	volumeSL->SetHashMarks(B_HASH_MARKS_NONE);
	volumeSL->SetHashMarkCount(0);
	volumeSL->SetBarThickness(5.0);
	volumeSL->SetStyle(B_TRIANGLE_THUMB);
	volumeSL->ResizeTo(Bounds().right-2,32);
	volumeSL->SetValue(90);
	volumeSL->SetTarget(this);
	//volumeSL->SetModificationMessage(msg);
	cmenu->SetTargetForItems(this);
	
	BMessage *msg;

	msg=new BMessage(SP_PLAY);
	msg->AddPointer("from", this);
	b1=new PlayButton(BRect(73,38,93,58),"pb4",this,msg,B_ONE_STATE_BUTTON);
	msg=new BMessage(SP_STOP);
	msg->AddPointer("from", this);
	b2=new StopButton(BRect(98,38,118,58),"pb4",this,msg,B_ONE_STATE_BUTTON);
	msg=new BMessage(SP_PAUSE);
	msg->AddPointer("from", this);
	b3=new PauseButton(BRect(123,38,143,58),"pb4",this,msg,B_ONE_STATE_BUTTON);
	BView::AllAttached();
}


void SoundPanel::AddThread(thread_id id) {
	if (bufferlevel<buffersize) {
		thread[firstfree]=id;
		firstfree=(firstfree+1);
		bufferlevel++;
	}
}


thread_id SoundPanel::RemoveThread(int32 index) {
	thread_id	id;
	
	
	if ((bufferlevel>0)){
		if (index==-1) index=firstfree-1;
		id=thread[index];
		// if the removed entry was not the last one,
		// mark it as B_BAD_THREAD_ID
		if (index<firstfree-1)
			thread[index]=B_BAD_THREAD_ID; 
		bufferlevel--;
		firstfree--;		
	} else id=B_ERROR;
	return id;
}





void SoundPanel::AllDetached(void) {
	BView::AllDetached();
}


SoundPanel::~SoundPanel() {
	RemoveSelf();
	delete kentry;
	delete loopCB;
	delete cmenu;
	delete volumeSL;
	delete b1;
	delete b2;
	delete b3;
}


void SoundPanel::StopAll() {
	thread_id	id;
	
	while (bufferlevel>0) {
		id=RemoveThread();
		if ((id!=B_BAD_THREAD_ID) &&(id!=B_ERROR) && (id > 0))
			send_data(id, 'stop', &buf, 4);
	}
}

void SoundPanel::Draw(BRect updateRect) {
	
	//StrokeRect(Bounds());
	rgb_color highcolor=HighColor();
	
	SetHighColor((rgb_color){255, 255, 255, 255});
	StrokeLine(Bounds().LeftTop(), Bounds().RightTop());
	SetHighColor((rgb_color){245, 245, 245, 255});
	StrokeLine(Bounds().LeftTop(), Bounds().LeftBottom());
	SetHighColor((rgb_color){175, 175, 175, 255});
	StrokeLine(Bounds().RightTop(), Bounds().RightBottom());
	SetHighColor((rgb_color){165, 165, 165, 255});
	StrokeLine(Bounds().LeftBottom(), Bounds().RightBottom());
	
	SetHighColor(highcolor);
	BView::Draw(updateRect);
}

void SoundPanel::ToggleLoop() {
	thread_id	id;

	// remove all trailing invalid thread_ids
	while ((id=thread[firstfree-1])==B_BAD_THREAD_ID) RemoveThread();
	// now we may change the currently playing thread's loop flag
	if (id!=B_ERROR)
		send_data(id, 'loop', &buf, 4);
}

void SoundPanel::UpdateVolume() {
	
	buf=volumeSL->Value();
	thread_id	id;

	// remove all trailing invalid thread_ids
	while ((id=thread[firstfree-1])==B_BAD_THREAD_ID) RemoveThread();
	// now we may change the currently playing thread's volume
	if (id!=B_ERROR)
		send_data(id, 'volu', &buf, 4);
}

void SoundPanel::Play() {
	thread_id	id;
	entry_ref	ref;
	
	// remove all trailing invalid thread_ids
	while ((id=thread[firstfree-1])==B_BAD_THREAD_ID) RemoveThread();
	kentry->GetRef(&ref);
	data=new BMessage();
	data->AddRef("ref", &ref);
	data->AddInt32("volume", volumeSL->Value());
	data->AddBool("loop", loopCB->Value()==B_CONTROL_ON);
	data->AddPointer("launcher",this);
	data->AddInt32("threadnumber",firstfree);
	id=spawn_thread((thread_func)PlaySoundBuffer,"SoundPanel Thread",(int32)20,data);
	AddThread(id);
	resume_thread(id);
}

void SoundPanel::Stop() {
	thread_id	id;

	// remove all trailing invalid thread_ids
	while ((id=thread[firstfree-1])==B_BAD_THREAD_ID) RemoveThread();
	// now we may stop the currently playing thread
	if (id!=B_ERROR)
		send_data(id, 'stop', &buf, 4);
}

void SoundPanel::Pause() {
	thread_id	id;

	// remove all trailing invalid thread_ids
	while ((id=thread[firstfree-1])==B_BAD_THREAD_ID) RemoveThread();
	// now we may pause/unpause the currently playing thread
	if (id!=B_ERROR)
		send_data(id, 'paus', &buf, 4);
}


int32 SoundPanel::PlaySoundBuffer(void *data)
{
		entry_ref ref;
		BFileGameSound *st;
		BMessage	*msg=(BMessage *)data;
		int32	code;
		thread_id	sender;
		bool	loop;
		int32 	volume=100;
		int32	buffer;
		void	*launcher;
		int32	threadnumber;
		
		
		msg->FindBool("loop", &loop);
		msg->FindInt32("volume", &volume);
		msg->FindRef("ref", &ref);
		msg->FindPointer("launcher",&launcher);
		msg->FindInt32("threadnumber", &threadnumber);
		
		st=new BFileGameSound(&ref,loop,NULL);
		if (st->Preload() != B_OK) return 1;
		st->SetGain((float)volume/100.0, 0);
		st->StartPlaying();
			while ((st->IsPlaying()) || (st->IsPaused()==BFileGameSound::B_PAUSED)) {
				if (has_data(find_thread(NULL))) {
					code=receive_data(&sender, &buffer, 4);
					switch(code) {
						case 'paus':	if (st->IsPaused()==BFileGameSound::B_NOT_PAUSED) {
								st->SetPaused(true, 0); 
								st->Preload();
								} else
								st->SetPaused(false, 0);
								break;
						case 'stop':	st->SetPaused(false, 0);
								st->StopPlaying();
								break;	
						case 'loop':	st->SetPaused(false, 0);
								st->StopPlaying();								
								break;
						case 'volu':	st->SetGain((float)(buffer)/100.0, 0);
								break;
					}
				}
				snooze(10000);
			}
		st->StopPlaying();
		delete st;
		
		// remove thread_id-entry from launcher's thread list
		((SoundPanel *)launcher)->RemoveThread(threadnumber);
		//delete msg;
				
		return 0;
};



void SoundPanel::MouseDown(BPoint point){

	BMessage *currentMsg = Window()->CurrentMessage();
	if (currentMsg->what == B_MOUSE_DOWN) {
		uint32 buttons = 0;
		currentMsg->FindInt32("buttons", (int32 *)&buttons);
		if ((buttons & B_SECONDARY_MOUSE_BUTTON)) {
			ConvertToScreen(&point);
			cmenu->Go(point, true);
		}
	}
	BView::MouseDown(point);
}

