
class RectangleMask{
public:
	RectangleMask();
	RectangleMask(float X, float Y, float Width, float Height);
	float x, y;
	float width, height;
	bool in(float X, float Y);
	bool mouseIn();
};

class RectangleButton : public RectangleMask{
public:
	RectangleButton(const RectangleMask &rect, char *caption_ = ""):RectangleMask(rect){
		init();
	};
	RectangleButton(float X, float Y, float Width, float Height):RectangleMask(X, Y, Width, Height){
		init();
	};
	char *caption;
	bool isPressed, wasPressed;
	float captionDx, captionDy;
	void (*onMouseClick)();
	void (*onMouseIn)();
	void (*onMouseOut)();
	void SetCaption(char *caption_);
	DWORD colorCommon;
	DWORD colorActive;
	DWORD colorTextCommon;
	DWORD colorTextActive;
	void Update();
	void Render();
	DWORD GetColor();
	void select();
	void deselect();
	void SetFunctions(void (*OnMouseClick)() = NULL,void (*OnMouseIn)() = NULL,void (*OnMouseOut)() = NULL);
protected:
	DWORD color;
	DWORD colorText;
	void init();
};