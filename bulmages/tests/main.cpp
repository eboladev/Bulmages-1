#include"blapplication.h"

#include"test_initial.h"

int main(int argc, char **argv) {
	BlApplication *app = new BlApplication(argc, argv);
	// app->exec();

	Initial initTest;
	QTest::qExec(&initTest);	
}
