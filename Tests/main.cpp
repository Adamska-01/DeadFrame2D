#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>


int main(int argc, char** argv)
{
	doctest::Context context;

	context.setOption("success", true);
	context.setOption("verbosity", "high");

	return context.run();
}