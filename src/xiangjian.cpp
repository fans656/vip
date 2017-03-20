#include "xiangjian.h"
#include "cmath"
XiangJian::XiangJian()
{
	//��ʼ���㷨����
	arithName = "ͼƬ���"; 
}

void XiangJian::setParam(const QList<QImage> &dataList, 
					  const QList<QString> &paramList)
{    
	//��ʼ������
	sourceImageA = dataList[0]; 
	sourceImageB = dataList[1];

	//��ʼ������
}

void XiangJian::dealData()
{
	QImage image(sourceImageA.size(), sourceImageA.format());

	int height = image.height();
	int width = image.width();
	for ( int w = 0; w != width; ++w)
	{
		for ( int h = 0; h != height; ++h)
		{
			int pixelA = sourceImageA.pixel(w,h);
			int pixelB = sourceImageB.pixel(w,h);
			int finalPixel =abs( pixelA - pixelB);
			//int gray = qGray( pixel);
			//gray = gray + (gray<<8) + (gray<<16) + (qAlpha(pixel)<<24);
			image.setPixel(w, h, finalPixel); 
		}
	}

	finalDataList.push_back(image); 
}
