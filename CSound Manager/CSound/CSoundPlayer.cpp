﻿#include "CSoundPlayer.h"

#include <iostream>

#include <include/utils.h>
#include <include/csound.h>

#include <CSound/CSoundScore.h>
#include <CSound/CSoundInstrument.h>

uintptr_t csThread(void *clientData);

CSoundPlayer::CSoundPlayer(CSoundScore * score)
{
	this->score = score;
	csound = nullptr;
	ThreadID = nullptr;
	PERF_STATUS = false;
}

CSoundPlayer::~CSoundPlayer() {
	Clean();
}

bool CSoundPlayer::Reload()
{
	bool state = PERF_STATUS;
	bool rez = Init();
	if (state && rez)
		Play();
	return rez;
}

bool CSoundPlayer::Init()
{
	Clean();
	csound = new Csound();
	string filename = "Resources//CSound//Scores//" + string(score->GetName()) + ".csd";
	int error = csound->Compile((char*)filename.c_str());

	if (error) {
		Clean();
		return false;
	}

	InitControlChannels();
	return true;
}

void CSoundPlayer::Clean()
{
	PERF_STATUS = false;
	if (ThreadID) {
		csoundJoinThread(ThreadID);
		ThreadID = nullptr;
	}
	if (csound) {
		csound->Cleanup();
		SAFE_FREE(csound);
	}
	//perfThread->Stop();
	//perfThread->Join();
}

void CSoundPlayer::Play()
{
	if (csound) {
		ThreadID = csoundCreateThread(csThread, this);
	}
	//perfThread->Play();
}

bool CSoundPlayer::IsPlaying()
{
	return PERF_STATUS;
}

void CSoundPlayer::Pause()
{
	//perfThread->Pause();
}

void CSoundPlayer::Stop()
{
	//perfThread->Stop();
	if (PERF_STATUS) {
		PERF_STATUS = false;
		cout << "Sound Score Time: " << csound->GetScoreTime() << endl;
	}
}
	
void CSoundPlayer::InitControlChannels()
{
	if (!csound) return;

	for (auto instr : score->GetEntries()) {
		for (auto &chn : instr->GetControlChannels()) {
			channels[chn];
		}
	}

	for (auto &channel : channels) {
		if (csound->GetChannelPtr(channel.second, channel.first.c_str(), CSOUND_INPUT_CHANNEL | CSOUND_CONTROL_CHANNEL) == 0)
			*channel.second = 1;
	}
}

void CSoundPlayer::SetControl(const char* channelName, float value)
{
	if (!PERF_STATUS || !csound) return;

	if (channels.find(channelName) != channels.end())
		*channels[channelName] = value;
}

uintptr_t csThread(void *data)
{	
	CSoundPlayer* score = (CSoundPlayer*)data;

	score->PERF_STATUS = true;

	// Play sound
	while ((score->csound->PerformKsmps() == 0)	&& score->PERF_STATUS) {}

	score->PERF_STATUS = false;

	return 0;
}