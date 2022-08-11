#include <fstream>
#include <cmath>
#include <iostream>

class Vec2
{
	public:
		double x, y;

		Vec2(): x(0), y(0) {};
		Vec2(double x, double y) : x(x), y(y) {}
		Vec2 operator+(const Vec2 &v) const { return Vec2(x + v.x, y + v.y); }
		Vec2 operator-(const Vec2 &v) const { return Vec2(x - v.x, y - v.y); }
		Vec2 operator*(double d) const { return Vec2(x * d, y * d); }
		Vec2 operator*(const Vec2 &v) const { return Vec2(x * v.x, y * v.y); }
		Vec2 operator/(const Vec2 &v) const { return Vec2(x / v.x, y / v.y); }
		Vec2 operator/(double d) const { return Vec2(x / d, y / d); }
		double mag() const { return sqrt(x * x + y * y); }
		Vec2 normalize() const
		{
			double mg = sqrt(x * x + y * y);
			return Vec2(x / mg, y / mg);
		}
};


class Vec3
{
	public:
		double x, y, z;

		Vec3(): x(0), y(0), z(0) {};
		Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
		Vec3 operator+(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
		Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
		Vec3 operator*(double d) const { return Vec3(x * d, y * d, z * d); }
		Vec3 operator*(const Vec3 &v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
		Vec3 operator/(double d) const { return Vec3(x / d, y / d, z / d); }
		Vec3 operator/(const Vec3 &v) const { return Vec3(x / v.x, y / v.y, z / v.z); }
		Vec3 normalize() const
		{
			double mg = sqrt(x * x + y * y + z * z);
			return Vec3(x / mg, y / mg, z / mg);
		}
};

inline double dot(const Vec3 &a, const Vec3 &b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

class Color
{
	private:
		double x, y, z;
	public:
		Color(double x, double y, double z) : x(x), y(y), z(z) {}
		Color(const Vec3 &v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		Color operator+(const Color &v) const { return Color(x + v.x, y + v.y, z + v.z); }
		Color operator-(const Color &v) const { return Color(x - v.x, y - v.y, z - v.z); }
		Color operator*(double d) const { return Color(x * d, y * d, z * d); }
		Color operator/(double d) const { return Color(x / d, y / d, z / d); }
};

class Ray
{
	public:
		Vec3 origin, direction;
		Ray(const Vec3 &origin, const Vec3 &direction) : origin(origin), direction(direction) {}
};

class Sphere
{
	private:
		Vec3 origin;
		double radius;
	public:
		Sphere(const Vec3 &origin, double radius) : origin(origin), radius(radius) {}
		Vec3 getNormal(const Vec3 &pi) const { return (pi - origin).normalize(); }
		bool intersection(Ray r);
};

bool Sphere::intersection(Ray ray){
	const Vec3 oc = ray.origin - origin;
	const double b = 2 * dot(oc, ray.direction);
	const double c = dot(oc, oc) - radius*radius;
	double disc = b*b - 4 * c;
	if (disc < 1e-4) return false;
	return true;
};

class Plane{
	public:
		Vec3 origin;
		Vec3 normal;
	private:
		Plane(const Vec3& origin, const Vec3& normal) : origin(origin), normal(normal) {};
		Vec3 getNormal() { return normal; }
};

void clamp255(Color &col)
{
	col.x = (col.x > 255) ? 255 : (col.x < 0) ? 0
		: col.x;
	col.y = (col.y > 255) ? 255 : (col.y < 0) ? 0
		: col.y;
	col.z = (col.z > 255) ? 255 : (col.z < 0) ? 0
		: col.z;
}

class Renderer{
	Renderer() = default;
	void perPixel();
};

int main()
{

	const Vec2 iResolution(1920,1080);

	Vec2 fragCoord(0,0);
	Vec2 uv(0,0);
	Color col = Color(0,0,0);
	std::ofstream out("out.ppm");
	out << "P3\n"
		<< iResolution.x << ' ' << iResolution.y << ' ' << "255\n";

	Sphere s = Sphere(Vec3(iResolution.x/2, iResolution.y/2, 0), 200);
	for (int y = 0; y < iResolution.y; ++y)
	{
		for(int x = 0; x < iResolution.x; ++x){
			fragCoord = Vec2(x,y);
			Ray r = Ray(Vec3(x,y,0), Vec3(0,0,1));

			uv = fragCoord/iResolution;
			col = Color(255*uv.x, 255*uv.y, 255);
			if(s.intersection(r)){
				out
					<< (int)col.x << ' '
					<< (int)col.y << ' '
					<< (int)col.z << '\n';
			}
			else{
				out
					<< 0 << ' '
					<< 0 << ' '
					<< 0 << '\n';
			}
		}
	}

