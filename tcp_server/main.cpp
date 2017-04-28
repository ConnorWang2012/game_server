#include "framework/service/runtime/server.h"

int main(int argc, char* argv[]) {
	gamer::Server::instance()->Start();
	return 0;
}