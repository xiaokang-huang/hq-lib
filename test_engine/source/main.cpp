#include <stdio.h>
#include <unistd.h>

#include <hq_engine.h>
#include <resource/hq_meshmanager.h>

int main() {
	printf("function start ...\n");
	HQEngine::Info info = {
		0,
	};
	HQEngine engine(&info);

	HQMeshManager meshmgr(0);

	HQWindow* pwin = engine.GetWindow();
	pwin->Create();
	engine.Initialize(2);

	int cnt = 10;
	while(cnt --) {
		engine.Run(1);
		sleep(1);
	}

	engine.Finalize();
	pwin->Destroy();
	return 0;
}
