#include <hq_type.h>
#include <unistd.h>

UINT32 GetProcessNum () {
	return (UINT32)sysconf(_SC_NPROCESSORS_CONF);
}
