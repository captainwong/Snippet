#include <stdio.h>
#include <string.h>

struct ID {
	static constexpr const char Table[] = "0123456789X";
	static constexpr int Length = 18;
	static constexpr int fromChar(char c) { return c != 'X' ? (Table[c] - '0') : 10; }
	static constexpr char toChar(int c) { return Table[c]; }

	char id[Length + 1] = { 0 };


	void setId(const char *id) {
		memcpy(this->id, id, Length);
	}

	int chk() {

	}

	bool valid() {

	}
};

int main()
{

}
