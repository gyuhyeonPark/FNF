#include "pch.h"
#include "SongEditorUI.h"
#include "AudioWaveUI.h"

SongEditorUI::SongEditorUI()
	: EditorUI("SongEditorUI")
{
	Ptr<AudioWaveUI> awUI = new AudioWaveUI;
	awUI->SetSizeAsChild(Vec2(1900.f, 500.f));
	AddChildUI(awUI.Get());
}

SongEditorUI::~SongEditorUI()
{
}

void SongEditorUI::Tick_UI()
{

}
