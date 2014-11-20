
#ifndef __TIME_H__
#define __TIME_H__

struct TimingData {
	unsigned lastFrameTimestamp;
	unsigned lastFrameDuration;
	
	static TimingData& get();
	static void update();
	static void init();
	static void deinit();
	static unsigned getTime();

private:
	TimingData() {}
    TimingData(const TimingData &) {}
    TimingData& operator=(const TimingData &) {}
};

#endif // __TIME_H__