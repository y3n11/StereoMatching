#include "StereoMatching.h"

using namespace std;
using namespace cv;




Mat
G2C(Mat disp,int param)
{
	Mat Hsv = Mat::zeros(disp.rows,disp.cols,CV_8UC3);
	for(int i = 0 ; i < disp.rows; i++)
		for(int j = 0; j < disp.cols; j++){
			if(disp.at<uchar>(i,j) != 0){
				Hsv.at<Vec3b>(i,j)[0] = disp.at<uchar>(i,j) * param;
				Hsv.at<Vec3b>(i,j)[1] = 255;
				Hsv.at<Vec3b>(i,j)[2] = 255;
			}
		}
	Mat dst;
	cvtColor(Hsv,dst,CV_HSV2BGR_FULL);
	return dst;
}


void
Display(Mat *img,char* m)
{
	int key;
	if(img->empty())
		return;
	imwrite("../images/disp.jpg", *img);
	namedWindow( "window", WINDOW_NORMAL );
    imshow( "window", *img );
	for(;;){
		key = cvWaitKey(0);
		if (key == -1)
			break;
	}
}

int
SAD(Mat imgl,Mat imgr,Point2i pl,Point2i pr,int w_size)
{
	int start = -(w_size )/2;
	int stop  = w_size -1;
	int sad = 0;
	for(int i = start; i < stop; i++)
		for(int j = start; j < stop; j++)
			sad += abs((int)imgl.at<uchar>(pl.y + j,pl.x + i) - (int)imgr.at<uchar>(pr.y + j,pr.x + i));
	return sad;
}

int
SSD(Mat imgl,Mat imgr,Point2i pl,Point2i pr,int w_size)
{
	int start = -(w_size )/2;
	int stop  = w_size -1;
	int ssd = 0;
	for(int i = start; i < stop; i++)
		for(int j = start; j < stop; j++)
			ssd += pow((int)imgl.at<uchar>(pl.y + j,pl.x + i) - (int)imgr.at<uchar>(pr.y + j,pr.x + i),2);
	return ssd;
}

int
getMin(Mat row,int col)
{
	int vmin = row.at<int>(0,col);
	int imin = 0;
	for(int i = 1; i < DISP ; i++){
		if (vmin > row.at<int>(i,col)){
			vmin = row.at<int>(i,col);
			imin = i;
		}
	}
	return imin;
}

int*
LBP(Mat img, Point2i p, int w_size)
{
	int start,stop,i,j,n = 0;
	int* bits = new int[w_size * w_size - 1];
	while(w_size > 0){
		start = -(w_size )/2;
		stop  = w_size /2 ;

		i = start;
		j = start;
		for(i = start; i < stop; i++)
			if((int)img.at<uchar>(p.y,p.x) > (int)img.at<uchar>(j + p.y,i + p.x))
				*(bits + n++) = 0;
			else
				*(bits + n++) = 1;

		for(j = start; j < stop; j++)
			if((int)img.at<uchar>(p.y,p.x) > (int)img.at<uchar>(j + p.y,i + p.x))
				*(bits + n++) = 0;
			else
				*(bits + n++) = 1;

		for(i = stop; i > start; i--)
			if((int)img.at<uchar>(p.y,p.x) > (int)img.at<uchar>(j + p.y,i + p.x))
				*(bits + n++) = 0;
			else
				*(bits + n++) = 1;

		for(j = stop; j > start; j--)
			if((int)img.at<uchar>(p.y,p.x) > (int)img.at<uchar>(j + p.y,i + p.x))
				*(bits + n++) = 0;
			else
				*(bits + n++) = 1;
		w_size -= 2;
	}
	return bits;
}

int
LBP(Mat imgl,Mat imgr,Point2i pl,Point2i pr,int w_size)
{
	int *tl = LBP(imgl,pl,w_size);
	int *tr = LBP(imgr,pr,w_size);
	int diff = 0;
	int vl = 0;
	int vr = 0;
	for(int i = 0; i < w_size * w_size ; i ++){
		vl += *(tl + i)*pow(2,i);
		vr += *(tr + i)*pow(2,i);
	}
	diff = abs(vl - vr);
	/*for(int i = 0; i < w_size * w_size - 1; i++)
	{
		if(*(tl + i) != *(tr + i))
			diff++;
	}*/
	delete[] tl;
	delete[] tr;
	return diff;
}

Mat
crop(Mat img,Point2i p, int w_size)
{
    return img(Rect(p.x - w_size/2,p.y - w_size/2,w_size,w_size)).clone();
}

int
Ideal(Mat img,Point2i p)
{
	iW = 1;
    int window_size = 3;
    Scalar mean,stddev,bstddev;
	Mat b = crop(img,p,window_size);
    meanStdDev(b,mean,bstddev,cv::Mat());
    meanStdDev(b,mean,stddev,cv::Mat());
	b.release();
    while (abs(stddev[0] - bstddev[0]) <= 40/*25*/){
        if(window_size/2 >= p.x || window_size/2 >= p.y || window_size >= WINDOW_SIZE)
            return window_size;
        window_size += 2;
        Mat window = crop(img,p,window_size);
        meanStdDev(window,mean,stddev,cv::Mat());
        window.release();
    }
    return window_size - 2 ;
}

