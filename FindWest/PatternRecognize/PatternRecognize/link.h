using namespace std;

struct pixel
{
	int col;
	int row;
};

struct Node
{
	pixel value;
	pixel edgept;
	Node *next;
};

struct pattern_ROI
{
	int center_col;
	int center_row;
	int width;
	int height;
	int angle;	//positive for counterclockwise
};

void insertNode(Node *p,pixel value);
void printNode(Node *p);