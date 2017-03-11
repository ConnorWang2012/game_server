#include "network_manager.h"

int main(int argc, char* argv[]) {
	gamer::NetworkManager::GetInstance()->InitSocket();
	return 0;
}