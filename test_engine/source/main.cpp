#include <stdio.h>
#include <unistd.h>

#include <hq_engine.h>
#include <resource/hq_meshmanager.h>
#include <hq_event_def.h>

void test_cb(HQEventType t, HQEventData d) {
	printf("%s %x\n", __FUNCTION__, t);
}

int main() {
	printf("function start ...\n");
	HQWindow win(0);
	HQWindow::Info info = {
		FALSE,
		200,
		200,
		800,
		600,
		test_cb
	};
	win.Create(&info);
	win.Destroy();
	return 0;
}
