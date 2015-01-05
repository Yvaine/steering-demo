
#include <windows.h>
#include <mmsystem.h>
#include "time.h"

// Internal time and clock access functions
unsigned systemTime() {
	return unsigned(timeGetTime());
}

unsigned TimingData::getTime() {
	return systemTime();
}

// Holds the global frame time that is passed around
static TimingData *timingData = NULL;

// Retrieves the global frame info instance
TimingData& TimingData::get() {
	return (TimingData&)*timingData;
}

// Updates the global frame information. Should be called once per frame.
void TimingData::update() {
	if (!timingData) return;

	// Update the timing information.
	unsigned thisTime = systemTime();
	timingData->lastFrameDuration = thisTime - timingData->lastFrameTimestamp;
	timingData->lastFrameTimestamp = thisTime;
}

void TimingData::init() {
	// Create the frame info object
    if (!timingData) timingData = new TimingData();

    timingData->lastFrameTimestamp = systemTime();
    timingData->lastFrameDuration = 0;
}

void TimingData::deinit() {
	delete timingData;
	timingData = NULL;
}
