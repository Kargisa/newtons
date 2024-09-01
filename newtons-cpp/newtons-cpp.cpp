// newtons-cpp.cpp : Defines the entry point for the application.
//

#include "Vector3.h"
#include "Vector2.h"

using namespace Newtons;

int main()
{
	Vector3 vector(1.2f, 0, 2.0f);
	Vector3 vector2 = vector;
	vector2 += Vector3(1.2f, 1, 2);

	const Vector2 vec2 = Vector2(2.5f, 0.85f);

	Vector2 n = vec2.Normalized();

	std::cout << vector << std::endl;
	std::cout << vector2 << std::endl;
	std::cout << vector - vector2 << std::endl;
	std::cout << 5.5f * vector << std::endl;
	std::cout << Vector3::Up() << std::endl;
	std::cout << Math::PI() << std::endl;
	std::cout << n << std::endl;
	std::cout << Vector3::Cross(vector2, vector) << std::endl;

	std::cin.get();
}
