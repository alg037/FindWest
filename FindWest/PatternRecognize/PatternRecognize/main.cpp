#include <opencv2\opencv.hpp>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace cv;

//date definition
struct pattern_ROI
{
	int center_col;
	int center_row;
	int width;
	int height;
	int angle;	//positive for counterclockwise
};

//function statement
Mat ConvertPiramid(Mat srcimg,int level);
int Testpiramid();
void FindEdge(Mat srcimg,Mat resultimg);
int TestEdge();
void TestROI();
void DrawROI(pattern_ROI ROI,Mat srcimg);
Point Rotate(Point center,Point inpoint,float angle);
void TestPatterRecognise1();
void TestPatterRecognise2();

void findROI(pattern_ROI ROI,int *min_row,int *max_row,int *min_col,int *max_col);
int getMaxPixel(Mat img);
int getMinPixel(Mat img);
void findPattern(Mat srcimg,Mat patternROI,int searchangle,Mat patternresult,int *out_col,int *out_row,int *angle);
void findPattern2(Mat srcimg,Mat patternROI,int searchangle,Mat patternresult,int *out_col,int *out_row,int *angle);
void showtime(char *text);
int getAveGray(Mat srcimg);
int getStdGray(Mat srcimg,int ave_gray);

double starttime,endtime,times;
ofstream out; 
int maximage=7;
int patternlevel=4; 
int searchangle=180;

//int patterncol=70;	//edge1
//int patternrow=225;
//int patternwidth=90;
//int patternheight=200; 
//int patterntangle=0;

int patterncol=433;	//xifen
int patternrow=348;
int patternwidth=150;
int patternheight=80;
int patterntangle=-25;


//int patterncol=223;	//squre
//int patternrow=328;
//int patternwidth=225;
//int patternheight=80; 
//int patterntangle=-55;


int main()
{
	out.open("log.txt",ios::app);
	//Testpiramid();
	//TestEdge();
	//TestROI();
	TestPatterRecognise1();
	cout<<"press any key to start..."<<endl;
	waitKey(0);
	return 0;
}
Mat ConvertPiramid(Mat srcimg,int level)
{
	//level=1:original place,resolution rows*cols
	//level=2:original place,resolution rows/2*cols/2
	//level=3:original place,resolution rows/4*cols/4
	//level=4:original place,resolution rows/8*cols/8
	//level=5:original place,resolution rows/16*cols/16


	Mat level1,level2,level3,level4,level5;
	level1=srcimg.clone();
	if (level!=1 && level!=2 && level!=3 && level!=4 && level!=5) return level1; //return original image while input invalid level

	if (level==1) return level1;
	level2=Mat(srcimg.rows/2,srcimg.cols/2,srcimg.type());
	for (int i = 0; i < level2.rows; i++)
	{
		uchar* pt1=srcimg.ptr<uchar>(2*i);
		uchar* pt2=srcimg.ptr<uchar>(2*i+1);
		uchar* pt3=level2.ptr<uchar>(i);

		for (int j = 0; j < level2.cols; j++)
		{
			pt3[j]=(pt1[2*j]+pt1[2*j+1]+pt2[2*j]+pt2[2*j+1])/4; //piramid pixels average filters
		}
	}
	if (level==2) return level2;

	level3=Mat(level2.rows/2,level2.cols/2,level2.type());
	for (int i = 0; i < level3.rows; i++)
	{
		uchar* pt1=level2.ptr<uchar>(2*i);
		uchar* pt2=level2.ptr<uchar>(2*i+1);
		uchar* pt3=level3.ptr<uchar>(i);

		for (int j = 0; j < level3.cols; j++)
		{
			pt3[j]=(pt1[2*j]+pt1[2*j+1]+pt2[2*j]+pt2[2*j+1])/4; //piramid pixels average filters
		}
	}
	if (level==3) return level3;

	level4=Mat(level3.rows/2,level3.cols/2,level3.type());
	for (int i = 0; i < level4.rows; i++)
	{
		uchar* pt1=level3.ptr<uchar>(2*i);
		uchar* pt2=level3.ptr<uchar>(2*i+1);
		uchar* pt3=level4.ptr<uchar>(i);

		for (int j = 0; j < level4.cols; j++)
		{
			pt3[j]=(pt1[2*j]+pt1[2*j+1]+pt2[2*j]+pt2[2*j+1])/4; //piramid pixels average filters
		}
	}
	if (level==4) return level4;

	level5=Mat(level4.rows/2,level4.cols/2,level4.type());
	for (int i = 0; i < level5.rows; i++)
	{
		uchar* pt1=level4.ptr<uchar>(2*i);
		uchar* pt2=level4.ptr<uchar>(2*i+1);
		uchar* pt3=level5.ptr<uchar>(i);

		for (int j = 0; j < level5.cols; j++)
		{
			pt3[j]=(pt1[2*j]+pt1[2*j+1]+pt2[2*j]+pt2[2*j+1])/4; //piramid pixels average filters
		}
	}
	if (level==5) return level5;


}
int Testpiramid() 
{
	Mat srcimg;
	srcimg=imread("img(1).bmp");
	if (srcimg.empty()) { cout<<"image not found!"<<endl; return 0;}
	imshow("piramidlevel1",srcimg);
	imwrite("piramidlevel1.bmp",srcimg);
	cvtColor(srcimg,srcimg,CV_BGR2GRAY);
	waitKey(0);

	Mat Piramidimg1;
	//Piramidimg1=Mat(srcimg.rows/2,srcimg.cols/2,srcimg.type());
	Piramidimg1=ConvertPiramid(srcimg,2);
	imwrite("piramidlevel2.bmp",Piramidimg1);
	imshow("piramidlevel2",Piramidimg1);
	waitKey(0);

	Mat Piramidimg2;
	//Piramidimg2=Mat(Piramidimg1.rows/2,Piramidimg1.cols/2,Piramidimg1.type());
	Piramidimg2=ConvertPiramid(srcimg,3);
	imwrite("piramidlevel3.bmp",Piramidimg2);
	imshow("piramidlevel3",Piramidimg2);
	waitKey(0);

	Mat Piramidimg3;
	//Piramidimg3=Mat(Piramidimg2.rows/2,Piramidimg2.cols/2,Piramidimg2.type());
	Piramidimg3=ConvertPiramid(srcimg,4);
	imwrite("piramidlevel4.bmp",Piramidimg3);
	imshow("piramidlevel4",Piramidimg3);
	waitKey(0);

}
void FindEdge(Mat srcimg,Mat resultimg)
{
	//calculate gradient of rows and cols
	int grad_row,grad_col;
	grad_row=0;
	grad_col=0;
	int left_col,right_col;
	uchar *pt1,*pt2,*pt3;
	uchar *ptout;
	for (int i = 0; i < srcimg.rows; i++)
	{
		//mirror edge pixels
		if (i==0) pt1=srcimg.ptr<uchar>(i+1);else pt1=srcimg.ptr<uchar>(i-1);	//above rows
		pt2=srcimg.ptr<uchar>(i); //current rows
		if (i==srcimg.rows-1) pt3=srcimg.ptr<uchar>(i-1);else pt3=srcimg.ptr<uchar>(i+1);	//next rows
		ptout=resultimg.ptr<uchar>(i);

		for (int j = 0; j < srcimg.cols; j++)
		{
			if (j==0) left_col=j+1;else left_col=j-1;
			if (j==srcimg.cols-1) right_col=j-1;else right_col=j+1;
			grad_row=(pt1[right_col]-pt1[left_col])/2+2*(pt2[right_col]-pt2[left_col])/2+(pt3[right_col]-pt3[left_col])/2;
			grad_col=(pt3[left_col]-pt1[left_col])/2+2*(pt2[j]-pt1[j])/2+(pt3[right_col]-pt1[right_col])/2;
			ptout[j]=abs(grad_row)+abs(grad_col);
			//ptout[j]=abs(grad_row)>abs(grad_col)?abs(grad_row):abs(grad_col);

		}
	}


}
int TestEdge()
{
	char filename[100],savename[100];
	for (int i = 1; i <=7; i++)
	{
		sprintf(filename,"img(%d).bmp",i);
		sprintf(savename,"piramid(%d).bmp",i);

		Mat srcimg;
		srcimg=imread(filename); 
		if (srcimg.empty()) { cout<<"image not found!"<<endl; return 0;}
		//imshow("srcimg",srcimg);
		//cvtColor(srcimg,srcimg,CV_BGR2GRAY);
		//waitKey(0);

		Mat piramidimg;
		piramidimg=Mat(srcimg.rows/2,srcimg.cols/2,srcimg.type());
		piramidimg=ConvertPiramid(srcimg,2);

		Mat piramidimg2;
		piramidimg2=Mat(srcimg.rows/4,srcimg.cols/4,srcimg.type());
		piramidimg2=ConvertPiramid(piramidimg,3);

		Mat Edgeimg1;
		Edgeimg1=Mat(piramidimg2.rows,piramidimg2.cols,piramidimg2.type());
		FindEdge(piramidimg2,Edgeimg1);
		imshow("edge1",Edgeimg1);
		waitKey(0);
		imwrite(savename,Edgeimg1);

		threshold(Edgeimg1,Edgeimg1,50,255,CV_THRESH_BINARY);
		imshow("edge",Edgeimg1);
		waitKey(0);
	}	
}
void TestROI()
{
	//pattern_ROI roi={430,348,100,80,-25};
	pattern_ROI roi={333,333,100,80,-25};

	Mat srcimg;
	srcimg=imread("img(1).bmp");
	DrawROI(roi,srcimg);
}
void DrawROI(pattern_ROI ROI,Mat srcimg)
{
	Point UL,UR,DL,DR;
	UL.y=ROI.center_row-ROI.height/2;		//cols
	UL.x=ROI.center_col-ROI.width/2;
	UR.y=ROI.center_row-ROI.height/2;
	UR.x=ROI.center_col+ROI.width/2;
	DL.y=ROI.center_row+ROI.height/2;
	DL.x=ROI.center_col-ROI.width/2;
	DR.y=ROI.center_row+ROI.height/2;
	DR.x=ROI.center_col+ROI.width/2;

	Point newUL,newUR,newDL,newDR;
	newUL=Rotate(CvPoint(ROI.center_col,ROI.center_row),UL,ROI.angle);
	newUR=Rotate(CvPoint(ROI.center_col,ROI.center_row),UR,ROI.angle);
	newDL=Rotate(CvPoint(ROI.center_col,ROI.center_row),DL,ROI.angle);
	newDR=Rotate(CvPoint(ROI.center_col,ROI.center_row),DR,ROI.angle);

	cvtColor(srcimg,srcimg,CV_GRAY2BGR);
	line(srcimg,newUL,newUR,Scalar(0,255,0),1,8);
	line(srcimg,newUR,newDR,Scalar(0,255,0),1,8);
	line(srcimg,newDR,newDL,Scalar(0,255,0),1,8);
	line(srcimg,newDL,newUL,Scalar(0,255,0),1,8);
	circle(srcimg,CvPoint(ROI.center_col,ROI.center_row),2,Scalar(0,0,255),4 ,8);

	uchar *pt1;
	Point newpt;
	int row1,col1;
	for (int i = 0; i < ROI.height; i+=1)
	{
		for (int j = 0; j < ROI.width; j+=1)
		{
			row1=ROI.center_row-ROI.height/2+i;
			col1=ROI.center_col-ROI.width/2+j;

			newpt=Rotate(CvPoint(ROI.center_col,ROI.center_row),CvPoint(col1,row1),ROI.angle);
			if (newpt.y<=0) continue;
			if (newpt.y>=srcimg.rows) continue;
			pt1=srcimg.ptr<uchar>(newpt.y);

			if (newpt.x<=0) continue;
			if (newpt.x>=srcimg.cols) continue;
			//pt1[newpt.x*3]=0;	//blue
			//pt1[newpt.x*3+1]=255;	//green
			//pt1[newpt.x*3+2]=0;	//red

		}
	}

	imshow("ROI",srcimg);
	//imwrite("ROI.bmp",srcimg);
	waitKey(0);
}
Point Rotate(Point center,Point inpoint,float CCWangle)
{
	float x,y;
	Point outpoint;
	x=inpoint.x-center.x;
	y=inpoint.y-center.y;
	outpoint.x=x*cos(3.1415*CCWangle/180.0)+y*sin(3.1415*CCWangle/180.0)+center.x;
	outpoint.y=-x*sin(3.1415*CCWangle/180.0)+y*cos(3.1415*CCWangle/180.0)+center.y;
	return outpoint;
}
void TestPatterRecognise1() //binary  statistic recognise
{
	//read pattern
	char filename[100],savename[100];
	sprintf(filename,"img\\img(%d).bmp",1);
	Mat patterimg=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);	//pattern image
	Mat patternROI;
	pattern_ROI roi={patterncol,patternrow,patternwidth,patternheight,patterntangle};

	int min_row,max_row,min_col,max_col;
	findROI(roi,&min_row,&max_row,&min_col,&max_col);
	patternROI=Mat(max_col-min_col,max_row-min_row,CV_8UC1);
	patternROI=patterimg(Rect(min_col,min_row,max_col-min_col,max_row-min_row));
	imshow("pattern",patternROI);
	imwrite("pattern.bmp",patternROI);
	waitKey(0);

	Mat patternROIlevel;
	patternROIlevel=ConvertPiramid(patternROI,patternlevel);
	imwrite("patternlevel.bmp",patternROIlevel);
	//imshow("patternlevel",patternROIlevel);
	//waitKey(0);

	Mat patternROIedge(patternROIlevel.rows,patternROIlevel.cols,CV_8UC1,Scalar(0));
	FindEdge(patternROIlevel,patternROIedge);
	imwrite("patternROIedge.bmp",patternROIedge);
	//imshow("patternROIedge",patternROIedge);
	//waitKey(0);

	Mat patternROIthr;
	patternROIthr=Mat(patternROIlevel.rows,patternROIlevel.cols,patternROIlevel.type());
	threshold(patternROIedge,patternROIthr,80,255,CV_THRESH_BINARY);
	DrawROI(roi,patterimg);
	imwrite("patternROI.bmp",patternROIthr);
	//imshow("patternROI",patternROIthr);
	//waitKey(0);
	//cout<<patternROIthr<<endl;
	int uuu=0;
	for (int i = 0; i < patternROIthr.rows; i++)
	{
		uchar *ppp=patternROIthr.ptr<uchar>(i);
		for (int j= 0; j < patternROIthr.cols; j++)
		{
			if (ppp[j]==255) uuu++;
		}
	}
	cout<<"max white:"<<uuu<<endl;

	for (int i = 1; i <=maximage; i++)
	{
		out<<endl<<"pic"<<i<<endl;
		cout<<endl<<"pic"<<i<<endl;

		//recognise pattern in piramid
		sprintf(filename,"img\\img(%d).bmp",i);
		sprintf(savename,"img\\out(%d).bmp",i);
		Mat targetimg;
		targetimg=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
		starttime=getTickCount();								//start search
		Mat targetimglevel;
		targetimglevel=ConvertPiramid(targetimg,patternlevel);
		showtime("pira:");
		//imshow("targetimglevel",targetimglevel);
		//imwrite("targetimglevel.bmp",targetimglevel);
		//waitKey(0);

		Mat patternresultimg;
		patternresultimg=Mat(targetimglevel.rows,targetimglevel.cols,CV_16UC1,Scalar(0));
		int pattern_row,pattern_col,pattern_angle;
		findPattern(targetimglevel,patternROIthr,searchangle,patternresultimg,&pattern_col,&pattern_row,&pattern_angle);
		out<<"row:"<<pattern_row<<endl<<"col:"<<pattern_col<<endl<<"angle:"<<pattern_angle<<endl;
		cout<<"row:"<<pattern_row<<endl<<"col:"<<pattern_col<<endl<<"angle:"<<pattern_angle<<endl;
		//cvtColor(targetimg,targetimg,CV_GRAY2BGR);
		pattern_ROI result={pattern_col,pattern_row,patternwidth,patternheight,pattern_angle};
		DrawROI(result,targetimg);
		//circle(targetimg,CvPoint(pattern_col,pattern_row),3,Scalar(0,255,0),1,8);
		//imshow("targetimg",targetimg);
		imwrite(savename,patternresultimg);
		//waitKey(0);
		//show result
	}
}
void TestPatterRecognise2() //gray  statistic recognise
{
	//read pattern
	char filename[100],savename[100];
	sprintf(filename,"img\\img(%d).bmp",1);
	Mat patterimg=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);	//pattern image
	Mat patternROI;
	pattern_ROI roi={patterncol,patternrow,patternwidth,patternheight,patterntangle};

	int min_row,max_row,min_col,max_col;
	findROI(roi,&min_row,&max_row,&min_col,&max_col);
	patternROI=Mat(max_col-min_col,max_row-min_row,CV_8UC1);
	patternROI=patterimg(Rect(min_col,min_row,max_col-min_col,max_row-min_row));
	imshow("pattern",patternROI);
	imwrite("pattern.bmp",patternROI);
	waitKey(0);

	Mat patternROIlevel;
	patternROIlevel=ConvertPiramid(patternROI,patternlevel);
	imwrite("patternlevel.bmp",patternROIlevel);
	//blur(patternROIlevel,patternROIlevel,Size(2,2),Point(-1,-1),BORDER_DEFAULT);
	imwrite("patternlevelblur.bmp",patternROIlevel);
	//imshow("patternlevel",patternROIlevel);
	//waitKey(0);

	for (int i = 1; i <=maximage; i++)
	{
		out<<endl<<"pic"<<i<<endl;
		cout<<endl<<"pic"<<i<<endl;

		//recognise pattern in piramid
		sprintf(filename,"img\\img(%d).bmp",i);
		sprintf(savename,"img\\out(%d).bmp",i);
		Mat targetimg;
		targetimg=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
		starttime=getTickCount();								//start search
		Mat targetimglevel;
		targetimglevel=ConvertPiramid(targetimg,patternlevel);
		showtime("pira:");
		//imshow("targetimglevel",targetimglevel);
		imwrite("targetimglevel.bmp",targetimglevel);
		//waitKey(0);

		Mat patternresultimg;
		patternresultimg=Mat(targetimglevel.rows,targetimglevel.cols,CV_16UC1);
		int pattern_row,pattern_col,pattern_angle;
		findPattern2(targetimglevel,patternROIlevel,searchangle,patternresultimg,&pattern_col,&pattern_row,&pattern_angle);
		out<<"row:"<<pattern_row<<endl<<"col:"<<pattern_col<<endl<<"angle:"<<pattern_angle<<endl;
		//cvtColor(targetimg,targetimg,CV_GRAY2BGR);
		pattern_ROI result={pattern_col,pattern_row,patternwidth,patternheight,pattern_angle};
		DrawROI(result,targetimg);
		//circle(targetimg,CvPoint(pattern_col,pattern_row),3,Scalar(0,255,0),1,8);
		imwrite(savename,patternresultimg);
		//imshow("targetimg",targetimg);
		//waitKey(0);
		//show result
	}
} //

void findROI(pattern_ROI ROI,int *min_row,int *max_row,int *min_col,int *max_col)
{
	Point UL,UR,DL,DR;
	UL.y=ROI.center_row-ROI.height/2;		//cols
	UL.x=ROI.center_col-ROI.width/2;
	UR.y=ROI.center_row-ROI.height/2;
	UR.x=ROI.center_col+ROI.width/2;
	DL.y=ROI.center_row+ROI.height/2;
	DL.x=ROI.center_col-ROI.width/2;
	DR.y=ROI.center_row+ROI.height/2;
	DR.x=ROI.center_col+ROI.width/2;

	Point newUL,newUR,newDL,newDR;
	newUL=Rotate(CvPoint(ROI.center_col,ROI.center_row),UL,ROI.angle);
	newUR=Rotate(CvPoint(ROI.center_col,ROI.center_row),UR,ROI.angle);
	newDL=Rotate(CvPoint(ROI.center_col,ROI.center_row),DL,ROI.angle);
	newDR=Rotate(CvPoint(ROI.center_col,ROI.center_row),DR,ROI.angle);

	*min_col=(newUL.x<=newUR.x)?newUL.x:newUR.x;
	*min_col=(*min_col<=newDR.x)?*min_col:newDR.x;
	*min_col=(*min_col<=newDL.x)?*min_col:newDL.x;

	*max_col=(newUL.x>=newUR.x)?newUL.x:newUR.x;
	*max_col=(*max_col>=newDR.x)?*max_col:newDR.x;
	*max_col=(*max_col>=newDL.x)?*max_col:newDL.x;

	*min_row=(newUL.y<=newUR.y)?newUL.y:newUR.y;
	*min_row=(*min_row<=newDR.y)?*min_row:newDR.y;
	*min_row=(*min_row<=newDL.y)?*min_row:newDL.y;

	*max_row=(newUL.y>=newUR.y)?newUL.y:newUR.y;
	*max_row=(*max_row>=newDR.y)?*max_row:newDR.y;
	*max_row=(*max_row>=newDL.y)?*max_row:newDL.y;
}
int getMaxPixel(Mat img)
{
	int max=-1;
	for (int i = 0; i < img.rows; i++)
	{
		uchar *pt=img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			if (max<=pt[j]) max=pt[j];
		}
	}
	return max;
}
int getMinPixel(Mat img)
{
	int min=255;
	for (int i = 0; i < img.rows; i++)
	{
		uchar *pt=img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			if (min>=pt[j]) min=pt[j];
		}
	}
	return min;
}
void findPattern(Mat srcimg,Mat patternROI,int searchangle,Mat patternresult,int *out_col,int *out_row,int *angle)
{
	int summax;
	int row1,col1,row2,col2;
	int stepangle;
	int rotsum,rotsummax;
	Point rotpt;
	uchar *pt11,*pt12,*pt13;
	Mat srcimgedge,srcimgthr;
	srcimgedge=Mat(srcimg.rows,srcimg.cols,srcimg.type());
	FindEdge(srcimg,srcimgedge);
	//imwrite("srcimgedge.bmp",srcimgedge);
	//imshow("srcimgedge",srcimgedge);
	//waitKey(0);
	srcimgthr=Mat(srcimg.rows,srcimg.cols,srcimg.type());
	threshold(srcimgedge,srcimgthr,80,255,CV_THRESH_BINARY);
	//imwrite("srcimgthr.bmp",srcimgthr); 
	//imshow("srcimgthr",srcimgthr);
	//waitKey(0);

	showtime("thre:");
	cout<<"working..."<<endl;
	out<<"working..."<<endl;

	stepangle=pow(2,patternlevel-1);
	summax=-pow(2,30);
	*angle=0;
	for (int i = 0; i < srcimgthr.rows; i+=1)
	{
		ushort *pt2=patternresult.ptr<ushort>(i);
		for (int j = 0; j < srcimgthr.cols; j+=1)
		{
			//calculate difference between pattern and image
			rotsummax=-pow(2,30);
			for (int m = -searchangle; m <= searchangle; m=m+2*stepangle)	//search angle loop
			{
				//cout<<"angle:"<<m<<endl;
				//Mat imgcopy=srcimg.clone();
				rotsum=0;
				for (int k = 0; k < patternROI.rows; k+=3)		//pattern row loop
				{
					row1=i-patternROI.rows/2+k;
					for (int l = 0; l < patternROI.cols; l+=3)		//pattern col loop
					{
						col1=j-patternROI.cols/2+l;
						rotpt=Rotate(CvPoint(j,i),CvPoint(col1,row1),m);	//calculate rotated row and col

						row2=rotpt.y;
						if (row2<0) goto labelm;
						if (row2>=srcimgthr.rows) goto labelm;
						pt11=srcimgthr.ptr<uchar>(row2);
						pt12=patternROI.ptr<uchar>(k);
						//pt13=imgcopy.ptr<uchar>(row2);

						col2=rotpt.x;
						if (col2<0) goto labelm;
						if (col2>=srcimgthr.cols) goto labelm;
						//pt13[col2]=255;
						if (pt12[l]==255 && pt11[col2]==255) rotsum++;
						//if (pt12[l]!=pt11[col2]) rotsum--; 
					}//for l
				}//for k
				//rotsum+=patternROI.rows*patternROI.cols;

				if (rotsummax<rotsum) rotsummax=rotsum;
				rotsum=0;
				if (summax<rotsummax) 
				{
					summax=rotsummax;
					*angle=m;
					*out_row=i*stepangle;
					*out_col=j*stepangle;
					//cout<<"col,row"<<*out_col<<","<<*out_row<<" sum:"<<summax<<endl;
					//imshow("pic",imgcopy);
					//waitKey(1);
				}
labelm:;//goto next angle
			}// for m
				pt2[j]=(rotsummax<=0)?0:rotsummax;	
		}//for j
	}//for i
	cout<<"col,row"<<*out_col<<","<<*out_row<<" sum:"<<summax<<endl;
	showtime("done:");
}
void findPattern2(Mat srcimg,Mat patternROI,int searchangle,Mat patternresult,int *out_col,int *out_row,int *angle)
{
	int summax;
	int row1,col1,row2,col2;
	int stepangle;
	float rotsum,rotsummax;
	int rotangle;
	Point rotpt;
	uchar *pt1,*pt2,*pt3;

	//calculate template parameter
	int temp_ave_gray,temp_std_gray;
	temp_ave_gray=getAveGray(patternROI);
	temp_std_gray=getStdGray(patternROI,temp_ave_gray);

	int img_ave_gray,img_std_gray;
	cout<<"working..."<<endl;
	out<<"working..."<<endl;

	stepangle=pow(2,patternlevel-1);
	summax=-pow(2,30);
	*angle=0;
	for (int i = 0; i < srcimg.rows; i+=1)
	{
		ushort *pt2=patternresult.ptr<ushort>(i);
		for (int j = 0; j < srcimg.cols; j+=1)
		{

			//calculate difference between pattern and image
			pt2[j]=0;
			rotsummax=-pow(2,30);;
			for (int m = -searchangle; m <= searchangle; m=m+1*stepangle)	//search angle loop
			{
				//cout<<"angle:"<<m<<endl;
				//Mat imgcopy=srcimg.clone();
				rotsum=0;
				for (int k = 0; k < patternROI.rows; k+=1)		//pattern row loop
				{
					row1=i-patternROI.rows/2+k;
					for (int l = 0; l < patternROI.cols; l+=1)		//pattern col loop
					{
						col1=j-patternROI.cols/2+l;
						rotpt=Rotate(CvPoint(j,i),CvPoint(col1,row1),m);	//calculate rotated row and col

						row2=rotpt.y;
						if (row2<0) goto labelj;
						if (row2>=srcimg.rows) goto labelj;
						pt1=srcimg.ptr<uchar>(row2);
						pt3=patternROI.ptr<uchar>(k);
						//pt13=imgcopy.ptr<uchar>(row2);

						col2=rotpt.x;
						if (col2<0) goto labelj;
						if (col2>=srcimg.cols) goto labelj;
						//if (pt11[col2]==255 && pt12[l]==255) sum++; 
						//pt13[col2]=255;
						if (pt2[l]==pt1[col2]) rotsum++; 
						if (pt2[l]!=pt1[col2]) rotsum--; 

					}//for l
				}//for k
				rotsum+=patternROI.rows*patternROI.rows;

				if (rotsummax<rotsum) rotsummax=rotsum;
				if (summax<rotsummax) 
				{
					summax=rotsummax;
					*angle=m;
					*out_row=i*stepangle;
					*out_col=j*stepangle;
					cout<<"col,row"<<*out_col<<","<<*out_row<<" sum:"<<summax<<endl;
					//imshow("pic",imgcopy);
					//waitKey(1);
				}

			}// for m
			pt2[j]=(rotsummax<=0)?0:rotsummax;

labelj: ;
		}//for j
	}//for i

	showtime("done:");
}

void showtime(char *text)
{
	endtime=getTickCount();
	times=1000*(endtime-starttime)/getTickFrequency();
	cout<<text<<setprecision(3)<<times<<"ms"<<endl;
	out<<text<<setprecision(3)<<times<<"ms"<<endl;

}

int getAveGray(Mat srcimg)
{
	int temp_ave_gray;
	int temp_sum=0;
	uchar *pt;
	for (int i = 0; i < srcimg.rows; i++)
	{
		pt=srcimg.ptr<uchar>(i);
		for (int j = 0; j < srcimg.cols; j++)
		{
			temp_sum+=pt[j];
		}
	}
	temp_ave_gray=temp_sum/(srcimg.rows*srcimg.cols);
	return temp_ave_gray;
}
int getStdGray(Mat srcimg,int ave_gray)
{
	int temp_std_gray;
	int temp_sum=0;
	uchar *pt;
	for (int i = 0; i < srcimg.rows; i++)
	{
		pt=srcimg.ptr<uchar>(i);
		for (int j = 0; j < srcimg.cols; j++)
		{
			temp_sum+=(pt[j]-ave_gray)*(pt[j]-ave_gray);
		}
	}
	temp_std_gray=sqrt(temp_sum/(srcimg.rows*srcimg.cols));
	return temp_std_gray;
}
