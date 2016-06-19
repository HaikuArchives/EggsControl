/************************************************************************************
* This function draws an empty BButton into a view. This is useful when you want	*
* to create a BPictureButton that shall look BeOS-like.								*
*																					*
* Syntax:	void DrawButton(	BView *view,										*
*								BRect frame = BRect(0,0,0,0),						*
*								bool pressed = false)								*
*																					*
* The Button is drawn into 'view'. If given it uses the dimensions of 				*
* 'frame', otherwise it uses the views dimensions. 'pressed' describes the look: 	*
* normal button or pressed button.													*
*																					*
* This source code is Public Domain. You can use it at your own risk.				*
*																					*
* Author: Werner Freytag (freytag@gmx.de)											*
* Visit my homepage for more software: http:/www.pecora.de/							*
*************************************************************************************/

#ifndef DRAW_BUTTON_H
#define DRAW_BUTTON_H

#include <View.h>
#include <Rect.h>

void	DrawButton(BView	*view, BRect frame = BRect(0,0,0,0), bool pressed = false );

#endif