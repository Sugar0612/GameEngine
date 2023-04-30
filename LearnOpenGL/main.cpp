#include "./Engine/Manager/CollectionMgr/CollectionMgr.h"

int main(int argc, char** argv) {
	CollectionMgr* mgr = new CollectionMgr();
	mgr->Start();
	return 0;
}