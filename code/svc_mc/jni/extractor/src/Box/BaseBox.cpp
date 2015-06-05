#include "BaseBox.h"

using namespace Boxes;
using namespace std;

BaseBox::BaseBox (uint32_t s, uint64_t t) : size(s), type(t), remained(s-8) {
	if (this->remained < 0) this->remained = 0;
}
BaseBox::~BaseBox () {}

void BaseBox::SetSize (uint32_t s) {
	this->size = s;
}
void BaseBox::SetType (uint64_t t) {
	this->type = t;
}
void BaseBox::IncRemaind(uint32_t v) {
	this->remained += v;
	if (this->remained > this->size - 8) {
		this->remained = this->size - 8;
		cout << "[Warning] - Increasing too much!!" << endl;
	}
}
void BaseBox::DecRemaind(uint32_t v) {
	this->remained -= v;
	if (this->remained < 0) {
		this->remained = 0;
		cout << "[Warning] - Decreasing too much!!" << endl;
	}
}


uint32_t BaseBox::GetSize () {
	return this->size;
}
uint64_t BaseBox::GetType () {
	return this->type;
}
uint32_t BaseBox::HasData() {
	return this->remained;
}