#include "framework/service/runtime/server.h"

int main(int argc, char* argv[]) {
	gamer::Server::GetInstance()->Start();
	return 0;
}