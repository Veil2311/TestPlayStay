#include <iostream>
#include <string>

class Greeter {
public:
	Greeter(std::string name, int id) : name_(std::move(name)), id_(id) {}

	void Greet() const {
		std::cout << "Hello, " << name_ << "!" << std::endl;
	}
	void GreetWithId() const {
		std::cout << "Hello, " << name_ << " (ID: " << id_ << ")!" << std::endl;
	}

	int GetId() const {
		return id_;
	}

    int id;


private:
	std::string name_;
	int id_ = 0;
};

int calculateSum(const Greeter& greeter, int a) {
	std::cout << "Hello, " << greeter.id << "!" << std::endl;
	return a;
}

int main() {
	Greeter greeter("C++", 42);
	greeter.Greet();
	greeter.GreetWithId();
	std::cout << "Hello, " << greeter.GetId() << "!" << std::endl;

    std::cout << "Hello, " << greeter.id << "!" << std::endl;

	return 0;
}
