#include <stdio.h>
#include <unistd.h>

#include <hq_engine.h>
#include <resource/hq_meshmanager.h>
#include <hq_event_def.h>

int main() {
	HQEngine engine;
	engine.Initialize();
	engine.Start();
	engine.Finalize();
	return 0;
}
