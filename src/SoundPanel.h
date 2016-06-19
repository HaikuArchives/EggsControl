#ifndef _SOUNDPANEL_H_
#define _SOUNDPANEL_H_

#include <View.h>
#include <String.h>
#include <CheckBox.h>
#include "StopButton.h"
#include "PauseButton.h"
#include "PlayButton.h"


#define SP_PLAY	'SPpl'
#define SP_STOP	'SPst'
#define SP_PAUSE	'SPps'
#define SP_LOOP	'SPlp'
#define SP_DELETE	'SPdl'
#define SP_UPDATEVOLUME	'SPuv'
#define SP_STOPALL	'SPsa'
#define SP_STARTALL	'SPpa'

class SoundPanel : public BView {
	public:
		SoundPanel(BEntry *entry);
		virtual ~SoundPanel();
		
		virtual void Draw(BRect updateRect);
		virtual void MouseDown(BPoint point);
		void Play();
		void Stop();
		void StopAll();
		void Pause();
		void ToggleLoop();
		void UpdateVolume();
		virtual void AllAttached(void);
		virtual void AllDetached(void);
		void AddThread(thread_id id);
		thread_id RemoveThread(int32 index=-1);
		
	private:
		static int32 PlaySoundBuffer(void *data);
		
		BEntry		*kentry;
		thread_id	thread[1024];
//		BButton	*playB,
//				*stopB,
//				*pauseB;
		BCheckBox	*loopCB;
		int		firstfree;
		int		buffersize, bufferlevel;
		BPopUpMenu	*cmenu;
		BSlider		*volumeSL;
		BMessage	*data;
		bool		paused, stopped;
		PlayButton	*b1;
		StopButton	*b2;
		PauseButton	*b3;
};


#endif