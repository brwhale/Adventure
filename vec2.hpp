#ifndef vec2Hpp
#define vec2Hpp

struct vec2 { 
	int x;
	int y;
	vec2() : x(0), y(0) {}
	vec2(int x_) : x(x_), y(x_) {}
	vec2(int x_, int y_) : x(x_), y(y_) {}
	vec2(const vec2& v) {
		x = v.x;
		y = v.y;
	}
	vec2 operator + (const vec2& v) const {
		return vec2(x+v.x, y+v.y);
	}
	vec2 operator - (const vec2& v) const {
		return vec2(x-v.x, y-v.y);
	}
	vec2 operator + (const int& i) const {
		return vec2(i) + *this;
	}
	vec2 operator - (const int& i) const {
		return *this - vec2(i);
	}
	void operator += (const vec2& v) {
		x+=v.x;
		y+=v.y;
	}
	void operator -= (const vec2& v) {
		x-=v.x;
		y-=v.y;
	}
	void operator += (const int& i) {
		*this += vec2(i);
	}
	void operator -= (const int& i) {
		*this -= vec2(i);
	}
	bool inside(const vec2& min, const vec2& max) const {
		if (x < min.x) return false;
		if (x > max.x) return false;
		if (y < min.y) return false;
		if (y > max.y) return false;

		return true;
	}
};

#endif