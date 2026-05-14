#include <vector>

class Road {
public:
	Road(float length, float width)
		: length_(length), width_(width) {}

	void SetSize(float length, float width) {
		length_ = length;
		width_ = width;
	}

	float GetLength() const { return length_; }
	float GetWidth() const { return width_; }

	const std::vector<float>& GetVertices() const { return vertices_; }

	void BuildFlatMesh() {
		vertices_.clear();
		vertices_.reserve(18);

		const float half_width = width_ * 0.5f;

		vertices_.insert(vertices_.end(), {
			-half_width, 0.0f, 0.0f,
			 half_width, 0.0f, 0.0f,
			 half_width, 0.0f, length_,
			-half_width, 0.0f, 0.0f,
			 half_width, 0.0f, length_,
			-half_width, 0.0f, length_
		});
	}

private:
	float length_ = 1.0f;
	float width_ = 1.0f;
	std::vector<float> vertices_;
};
int main(){
    Road r(10.0f, 2.0f);
    SetSize(r);

}