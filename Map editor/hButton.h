#pragma warning(disable : 4006)

enum ID{
	idStart,
	idRestart,
	idExit
};

const int BUTTONS_COUNT = 3;

class hButton{
public:
	float x;
	ID id;
	bool selected;
	bool enabled;
	hButton(float x_, char *caption_, ID id_);
	char *caption;
	void update();
	void render();
};